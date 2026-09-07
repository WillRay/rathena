#!/usr/bin/perl
#
# quest_shop_barter.pl -- generate the Quest Shop barter shops.
#
# The recipe table in npc/custom/quest_shop.txt is the single source of truth.
# The map server cannot read barter data from script, so the native barter
# shops are generated from that table instead of being maintained by hand.
#
#   perl tools/quest_shop_barter.pl           regenerate the yml
#   perl tools/quest_shop_barter.pl --check   exit 1 if the yml is stale
#
# Re-run after editing any Add(...) line or the .Shops$ list.

use strict;
use warnings;
use File::Basename qw(dirname);
use File::Spec;

my $check = ( @ARGV && $ARGV[0] eq '--check' ) ? 1 : 0;

my $root = File::Spec->rel2abs( File::Spec->catdir( dirname(__FILE__), '..' ) );
my $src  = "$root/npc/custom/quest_shop.txt";
my $out  = "$root/npc/custom/quest_shop_barters.yml";

# ---------------------------------------------------------------- item db ---
# Barter yml addresses items by AegisName only, so build id -> name here.
# Refineable defaults to FALSE in rAthena (itemdb.cpp: flag.no_refine = true),
# and emitting "Refine: 0" for a non-refineable item is a hard parse error,
# so track that flag too.
my ( %aegis, %refineable );
for my $f ( glob("$root/db/pre-re/item_db*.yml"), glob("$root/db/import/item_db*.yml") ) {
    open my $fh, '<', $f or die "cannot read $f: $!\n";
    my $id;
    while ( my $l = <$fh> ) {
        if    ( $l =~ /^  - Id:\s*(\d+)/ )            { $id = $1; next }
        next unless defined $id;
        if    ( $l =~ /^    AegisName:\s*(\S+)/ )     { $aegis{$id}      = $1 }
        elsif ( $l =~ /^    Refineable:\s*(\w+)/ )    { $refineable{$id} = ( lc($1) eq 'true' ) ? 1 : 0 }
    }
    close $fh;
}
die "no items loaded from db/ -- wrong root?\n" unless %aegis;

# ------------------------------------------------------------- quest_shop ---
open my $fh, '<', $src or die "cannot read $src: $!\n";
my @lines = <$fh>;
close $fh;

# Enabled shops. Anything after a // on the .Shops$ line is deliberately off.
my @shopname;
for my $l (@lines) {
    my $rest = $l;
    $rest =~ s{//.*}{};    # the header doc block contains a commented example
    next unless $rest =~ /setarray\s+\.Shops\$\[1\]\s*,(.*)/;
    $rest = $1;
    my $i = 1;
    $shopname[ $i++ ] = $1 while $rest =~ /"([^"]*)"/g;
    last;
}
die "could not find the .Shops\$ array in $src\n" unless @shopname > 1;

my ( %recipe, @warn );
my $lineno = 0;
for my $l (@lines) {
    $lineno++;
    my $c = $l;
    $c =~ s{//.*}{};
    next unless $c =~ /^\s*Add\s*\(\s*(.*?)\s*\)\s*;/;

    my @a = map { my $t = $_; $t =~ s/^\s+|\s+$//g; $t } split /,/, $1;
    if ( @a < 5 || ( @a - 5 ) % 2 ) {
        push @warn, "line $lineno: malformed Add() -- skipped";
        next;
    }
    my ( $shop, $reward, $amount, $zeny, $points ) = @a[ 0 .. 4 ];
    my @reqs;
    for ( my $i = 5 ; $i < @a ; $i += 2 ) { push @reqs, [ $a[$i], $a[ $i + 1 ] ] }

    next unless defined $shopname[$shop];    # tier not enabled

    if ( !@reqs && !$zeny && !$points ) {
        push @warn, "line $lineno: reward $reward has NO cost at all -- excluded from the barter shop";
        next;
    }
    if ($points) {
        push @warn, "line $lineno: reward $reward costs points; barter cannot express that -- crafter only";
        next;
    }
    if ( @reqs > 6 ) {    # MAX_BARTER_REQUIREMENTS
        push @warn, "line $lineno: reward $reward has " . scalar(@reqs) . " requirements, barter allows 6 -- crafter only";
        next;
    }
    for my $id ( $reward, map { $_->[0] } @reqs ) {
        die "line $lineno: item $id is not in the item database\n" unless $aegis{$id};
    }

    push @{ $recipe{$shop} }, { reward => $reward, amount => $amount, zeny => $zeny, reqs => \@reqs };
}

# ------------------------------------------------------------------ emit ---
my $yml = <<"HEAD";
###########################################################################
# Quest Shop barter shops -- GENERATED FILE, DO NOT EDIT BY HAND.
#
# Source:    npc/custom/quest_shop.txt  (the Add(...) recipe table)
# Generator: tools/quest_shop_barter.pl
#
# Edit the recipes in quest_shop.txt, then re-run:
#   perl tools/quest_shop_barter.pl
#
# These NPCs have no Map, so they exist only to be opened from script via
# callshop -- the Rare Hat Merchant does that. Requirements carry Refine: 0
# wherever the item is refineable, so a player's +7 hat is never eaten as an
# ingredient.
###########################################################################

Header:
  Type: BARTER_DB
  Version: 2

Body:
HEAD

for my $shop ( sort { $a <=> $b } keys %recipe ) {
    $yml .= "  # $shopname[$shop]\n";
    $yml .= "  - Name: qshop_t$shop\n";
    $yml .= "    Items:\n";
    my $idx = 0;
    for my $r ( @{ $recipe{$shop} } ) {
        $yml .= "      - Index: $idx\n";
        $yml .= "        Item: $aegis{$r->{reward}}\n";
        $yml .= "        Zeny: $r->{zeny}\n" if $r->{zeny};
        if ( @{ $r->{reqs} } ) {
            $yml .= "        RequiredItems:\n";
            my $ridx = 0;
            for my $q ( @{ $r->{reqs} } ) {
                $yml .= "          - Index: $ridx\n";
                $yml .= "            Item: $aegis{$q->[0]}\n";
                $yml .= "            Amount: $q->[1]\n";
                $yml .= "            Refine: 0\n" if $refineable{ $q->[0] };
                $ridx++;
            }
        }
        $idx++;
    }
    $yml .= "\n";
}

warn "  ! $_\n" for @warn;

if ($check) {
    my $cur = '';
    if ( open my $c, '<', $out ) { local $/; $cur = <$c>; close $c }
    if ( $cur ne $yml ) {
        print STDERR "STALE: $out does not match quest_shop.txt -- run: perl tools/quest_shop_barter.pl\n";
        exit 1;
    }
    printf "ok: barter shops match quest_shop.txt (%d shops, %d entries)\n",
      scalar( keys %recipe ), scalar( map { @$_ } values %recipe );
    exit 0;
}

open my $o, '>', $out or die "cannot write $out: $!\n";
print $o $yml;
close $o;

printf "wrote %s -- %d shops, %d entries%s\n", $out,
  scalar( keys %recipe ),
  scalar( map { @$_ } values %recipe ),
  ( @warn ? sprintf( ", %d skipped", scalar @warn ) : '' );
