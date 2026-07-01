# SERVER BALANCING

## CORE BALANCING CHANGES

We recognize that the original game is nearly two decades old, and parts of the
combat experience have aged less gracefully than others. Pre-Renewal gameplay
still feels truest to the original game, but it can also feel dated — particular
classes have skills that simply do not pull their weight, and a handful of
quality-of-life decisions never made it back into the engine. The skill changes
below were discussed inside the team, tested in PvE and PvP, and are tracked in
this file so that the difference between this server and stock rAthena is always
explicit.

This changelog currently covers the **Swordsman, Knight, Crusader, Thief,
Assassin and Rogue** trees. Other classes are untouched so far and will be
added in future passes.

---

## Class Changes

### Novice Skill Changes

- **First Aid:** Now restores `2% of Max HP` and costs `2% of Max SP` instead of
  the flat `5 HP for 3 SP`. *Developer Comments: The old flat numbers stop
  mattering the moment a Novice has more than a few hundred HP, so the skill
  quietly retires the moment it should start being useful. Scaling both sides to
  a percentage keeps it relevant on any character that still has the skill in
  their bar — and the matching SP cost stops it from being a free heal-spam
  button.*

### Swordsman Skill Changes

- **Bash:** SP cost reduced to a smooth `3 + 1 per skill level` (4 SP at level 1
  up to 13 SP at level 10). *Developer Comments: Previously the skill had only
  two SP costs — one for levels 1–5 and another for 6–10. Because damage scales
  linearly with skill level, SP cost should scale with it too, and players who
  prefer a mid-level Bash should not be punished for stopping short of 10.*

- **Magnum Break:** Splash radius increased to 4 cells (9×9 area) and SP cost
  lowered from 30 to 18. *Developer Comments: With the lower SP cost and the
  larger area, Swordsmen actually have a viable mobbing tool at low levels.
  Compared to second-class AoEs it is still on the modest side, but it should
  finally feel worth pressing.*

- **Moving HP Recovery:** Can be learned at Job Level 15 and the quest now asks
  for 50 Empty Bottles instead of 200. *Developer Comments: As with other
  Platinum-skill reworks, the requirement is dropped in line with the lower
  level the skill becomes available at. (Verified — already in line with this
  rebalance on the live tree.)*

### Knight Skill Changes

- **Two-Hand Quicken:** In addition to its existing ASPD bonus, now grants
  `+1 FLEE per skill level` and `+0.8% CRIT per skill level`. The previous HIT
  bonus has been removed. *Developer Comments: Auto-attack Knights have always
  felt a step behind Bowling-Bash and Spiral-Pierce builds. A modest survival
  and crit bonus on the buff they were going to press anyway brings the agility
  line closer to its peers without making it the new optimum.*

- **Bowling Bash:** The gutter-line bug has been removed entirely (the chain
  reaction now uses a 5×5 area around the caster, independent of where on the
  map you stand) and the skill now hits twice per cast, with each hit dealing up
  to 400% damage at level 10 (800% total). *Developer Comments: The official
  workaround for gutter-lines was simply to inflate damage and tell players to
  edit their GRFs or memorize map cells. That is not a fix. We've set the
  per-hit damage a little lower than the inflated 800% the official game ended
  up with, but the skill now hits twice and behaves the same way on every map,
  every cell.*

### Crusader Skill Changes

- **Holy Cross:** Damage raised from `100% + 35% per level` to
  `300% + 25% per level`. The skill now also has a `+2% accuracy bonus per skill
  level` and an explicit range of 3 cells. SP cost is now `6 + 1 per level`
  (16 SP at level 10). *Developer Comments: Holy Cross was sitting in the
  shadow of Grand Cross to the point that nobody picked it. A real damage
  floor, a small accuracy bump and a kinder SP curve give Spear- and
  Sword-Crusaders a single-target option that is worth the cast.*

- **Shield Charge:** Damage raised from `100% + 20% per level` to
  `200% + 20% per level`. *Developer Comments: The skill is built around stun
  and knockback utility, but the damage was so low that it felt like a
  punishment to press it. A doubled base ratio means it is now a respectable
  hit on top of its crowd-control behavior.*

- **Spear Quicken:** Now reduces weapon delay by `20% + 1.5% per skill level`
  with two-handed spears and `7.5% + 0.5% per skill level` with one-handed
  spears. *Developer Comments: Spear builds were not in a great place compared
  to GC builds for PvE DPS. Splitting the ASPD bonus by weapon type — and
  giving 1H spear users something at all — rewards the heavy skill-point
  investment without dethroning Grand Cross.*

- **Spear Mastery:** Now grants `+5 ATK per level` while not mounted and
  `+7 ATK per level` while riding a Peco. *Developer Comments: As with the
  other mastery reworks, this exists to make the skill points feel earned.
  (Verified — already in line with this rebalance on the live tree.)*

### Thief Skill Changes

- **Double Attack:** Activation chance raised from `5% per skill level` to
  `7% per skill level` (70% at level 10). *Developer Comments: Thieves level
  noticeably slower than the other first classes, and the bonus carries
  forward into Assassin and Rogue builds that lean on auto-attacks.
  (Verified — already in line with this rebalance on the live tree.)*


- **Increase Dodge — Opportunist:** A successful FLEE dodge in PvE has
  a `5%` chance to refund `8 SP` and grant `Opportunist`, an opening
  that halves the SP cost of your next damaging skill (`8 second`
  duration). Fully disabled in PvP and WoE. *Developer Comments: Auto-attacking
  evasion-tanks now get a small, occasional resource payoff instead of
  the passive being pure stats. The `5%` rate keeps it from demanding
  attention — it's a pleasant surprise, not a rotation. PvP and WoE are
  gated entirely off because the existing reduced multi-attacker FLEE
  penalty already softens the swarm-targeting concern there, and adding
  SP sustain on top of high evasion would warp duels.*

- **Envenom:** SP cost reduced to a flat `8 SP`. Damage now scales with
  weapon ATK at `100% + 15% per skill level` (250% at level 10) instead of
  the old flat `+15 per level` mastery-style bonus. *Developer Comments:
  Envenom pairs well with the reworked poison status, but the old SP cost
  made it punishing to spam and the flat damage stopped mattering as soon as
  players upgraded gear. The lower SP cost and ATK-based scaling fix both.
  (Verified — already in line with this rebalance on the live tree.)*

- **Envenom — Element Floor:** The cumulative elemental modifier applied to
  Envenom's damage is now floored at `50%`. Against Poison-element targets
  (normally `0%` in the defense-element table) the skill now lands for half
  damage instead of being shut out entirely; matchups that already resolve
  above `50%` are unchanged. *Developer Comments: With the new weapon-ATK-based
  scaling, a hard `0%` against Poison defenders turned the most thematic
  matchup (poisoning the poisonous) into a no-op. `50%` keeps poison-weak
  targets like Plants clearly the better place to press Envenom, so
  elemental advantage is still a real design knob — `25%` was rejected as
  too low and a higher floor would have erased immunity as a lever.*

- **Envenom — Auto-Attack Resume:** If Envenom is cast while the player is
  already auto-attacking, the auto-attack now resumes on the same target
  the moment the skill finishes resolving. *Developer Comments: Envenom is
  the canonical "weave it into your auto-attacks" Thief skill, but the
  client's cast handling cleared the lock-on so players had to right-click
  the target a second time to keep swinging. Restoring the lock keeps the
  intended rhythm of the skill — one keystroke, no babysitting — and lines
  up with how Double Attack's Momentum stacks are meant to be consumed
  inside an auto-attack chain. Note: for the smoothest weave on a slow
  dagger, leave the client's `No Control` (right-click-attack) toggle
  off — the client itself throttles skill input while a continuous attack
  is queued, which the server cannot bypass; the resume here keeps your
  auto-attacks going either way.*

- **Detoxify:** On a successful cast that actually removes a `Poison` or
  `Deadly Poison` status from the target, Detoxify now also heals the
  target for `10% of their Max HP`. If the target had no poison to cure,
  no heal is granted. *Developer Comments: Detoxify has always been a
  situational one-point wonder — you take it, it sits on your bar, and
  most parties would rather down a Green Potion than wait for a Thief to
  cast. Attaching a real `10% Max HP` payoff to a successful cleanse gives
  the skill a reason to exist in a rotation and turns the Thief into a
  small but meaningful emergency responder for poison-heavy fights (Rafflesia,
  Argiope, PvP Enchant Poison pressure) without making it a general-purpose
  heal — the poison gate keeps it honest.*

- **Find Stone:** Now grants a persistent `Holding Stone` status buff instead
  of placing a physical `Stone` item in the player's inventory. The status has
  no timer, persists through map changes and relog, and is cleared only on
  death. Casting again while already holding a stone is a silent no-op (it does
  not stack or reset). Re-tuned to a `1 second` (DEX-reducible) cast time and a
  `1 SP` cost, and given a `10 second` cooldown. *Developer Comments: The
  inventory-item model created bag-management friction that made the skill feel
  like a chore. Treating the stone as a consumed status removes that friction
  and sets up a real spend decision between Stone Fling and Sand Attack. The
  cooldown lives on the stone generator rather than on Stone Fling: because a
  stone is required to throw, gating Find Stone at `10 seconds` closes the
  `Find Stone -> Stone Fling` perma-stun loop just as effectively while leaving
  Stone Fling free to fire the instant a stone is in hand.*

- **Stone Fling:** Now requires the `Holding Stone` status to cast; without it
  the skill fails with the standard skill-fail message and costs no SP (the
  requirement is checked before SP is deducted). On a successful hit it consumes
  the stone and stuns the target for `2 seconds` at a guaranteed rate that
  bypasses status resistance. SP cost raised to `8`, and the old `Stone` item
  requirement was removed. The skill itself has no cooldown — the
  `Find Stone -> Stone Fling` perma-stun loop is instead gated by Find Stone's
  `10 second` cooldown, since a stone must be generated before it can be thrown.
  *Developer Comments: A guaranteed stun on a `7`-cell
  ranged skill is strong, so the stone is the cost. Rogues and Stalkers now have
  a reliable ranged CC opener that stays relevant past the early game. The item
  requirement had to be deleted from the base skill DB — requirement removal is
  the one thing the import loader cannot express — since Find Stone no longer
  produces a Stone item.*

- **Sand Attack:** Now requires the `Holding Stone` status to cast; without it
  the skill fails with the standard skill-fail message and costs no SP. On a
  successful hit it consumes the stone and blinds the target for `30 seconds` at
  a guaranteed `100%` rate that bypasses status resistance (previously a `20%`
  chance for players). *Developer Comments: The original partial-chance blind
  was too unreliable to build around. Making it 100% but stone-gated creates
  genuine resource tension — spend the stone for a stun (Stone Fling) or a
  guaranteed blind (Sand Attack) — while keeping unreliable CC off the table.*

### Assassin Skill Changes

- **Right Hand Mastery / Left Hand Mastery:** Both mastery curves are
  shifted up by `+20%` at every rank — Right Hand goes from `60–100%` to
  `80–120%`, Left Hand from `40–80%` to `60–100%`. *Developer Comments:
  Auto-attack Assassins were falling behind skill-centric builds in raw
  DPS. A flat +20% on both mastery curves brings dual-wield parity back
  into striking distance without overhauling the formula.*

- **Venom Dust:** Reagent changed from `1 Red Gemstone` to
  `2 Venom Canines`. *Developer Comments: Red Gemstones are a shared
  reagent across many classes, while Venom Canines are a more thematic and
  more plentiful Assassin-flavored ingredient.*

- **Venom Splasher:** The `75% target HP` requirement and the
  `Red Gemstone` cost are both removed, and the detonation timer is
  retuned to a clean linear curve of `15 seconds at level 1` down to
  `6 seconds at level 10`. *Developer Comments: Poison is central to the
  Assassin's identity, but Splasher was almost never pressed because of
  the HP gate, the reagent and the inconsistent timer. Stripping all three
  blockers and giving it a predictable detonation puts the skill on par
  with the "finisher skills" other classes already have.*

- **Enchant Poison:** Buff duration extended to `5 minutes` at every rank
  and the poisoning proc chance scales up to `10% at rank 10`.
  *Developer Comments: Poison sits at the center of the Assassin's kit, so
  the buff that enables it should not be a chore to maintain. (Verified —
  already in line with this rebalance on the live tree.)*

- **Katar Mastery:** Continues to grant `+4 ATK per skill level`
  (40 ATK at rank 10). Now also grants `+5% nominal CRIT at rank 10`,
  which becomes roughly `+10% effective` once the katar's double crit
  rate is applied. *Developer Comments: With the poison line now viable,
  the CRIT-Assassin needed a matching bump so it stays competitive with
  pure poison builds rather than being a strictly weaker option.*

### Rogue Skill Changes

- **Strip Weapon:** When it lands on a monster, ATK is reduced by `40%`
  (up from 25%). *Developer Comments: Strip skills barely justify a slot
  outside of PvP/GvG. A real PvE debuff value makes them worth the cast
  in upcoming content. (Verified — already in line with this rebalance on
  the live tree.)*

- **Strip Shield:** When it lands on a monster, hard defense is reduced
  by `30%` (up from 15%). *Developer Comments: Same motivation as Strip
  Weapon — a usable PvE debuff value. (Verified — already in line with
  this rebalance on the live tree.)*

- **Strip Armor:** When it lands on a monster, soft defense (VIT) is
  reduced by `70%` (up from 40%). *Developer Comments: Soft defense
  shaves a real chunk off physical mitigation, which gives Rogues a
  meaningful party contribution against tanky targets. (Verified —
  already in line with this rebalance on the live tree.)*

- **Strip Helm:** When it lands on a monster, INT is reduced by `30%`
  (down from 40%) and MDEF is now also reduced by `20%`. *Developer
  Comments: The trade — less INT, but a real MDEF cut — hurts high-MDEF
  monsters substantially more than the old pure-INT reduction did. The
  MDEF path required a small bug fix in `status.cpp` where the reduction
  was reading from the wrong status entry and silently doing nothing.*

- **Sword Mastery:** Each level of Sword Mastery now also grants a
  `7% chance per skill level` of Double Attack with swords for Rogues
  who have learned Double Attack at an equal or higher rank.
  *Developer Comments: Lets Rogues build a sword-based auto-attack
  meta without leaning on the Sidewinder card as the only entry point.
  (Verified — already in line with this rebalance on the live tree.)*

- **Back Stab:** Redesigned as a gap closer — castable from up to
  `3 cells away`, with the Rogue jumping behind the target to deliver
  the strike. Cast delay is now `3.4 sec − 0.1 sec per skill level`
  (3.3 sec at level 1, 2.4 sec at level 10). The old facing/adjacency
  requirements are gone. *Developer Comments: The original Back Stab was
  fiddly to use even when it was strong; turning it into a clean gap
  closer makes the kit feel more like the assassin-fantasy class it has
  always wanted to be. A future toggle may restore the original behavior
  for players who prefer it.*

- **Preserve:** Now reachable on regular Rogues at `Plagiarism 10` for
  `1 skill point`. *Developer Comments: Preserve is technically a
  transcendent Stalker skill, but Plagiarism builds depend on it to stop
  passively losing their copied skill mid-fight. Granting it early avoids
  the awful loop of having to ask another player to recast something
  every time you change maps.*

---

## Notes for server operators

- The skill database changes are layered on top of the stock pre-renewal data
  via `db/import/skill_db.yml` and `db/import/status.yml`. The base
  `db/pre-re/*.yml` files are left untouched, so future upstream merges of
  rAthena will not silently revert this balance pass.
- The Bowling Bash configuration in `conf/battle/skill.conf` now defaults to
  `bowling_bash_area: 2` (area around caster). Restoring the legacy gutter-line
  behavior is still possible by setting it back to `0`.
- The engine-level changes live in:
  - `src/map/skills/swordman/holycross.cpp` / `holycross.hpp`
  - `src/map/skills/swordman/smite.cpp` (Shield Charge / Smite)
  - `src/map/skills/swordman/bowlingbash.cpp`
  - `src/map/skills/thief/venomsplasher.cpp` (drops the 75% HP gate)
  - `src/map/skills/thief/backstab.cpp` (gap-closer redesign)
  - `src/map/status.cpp` (Two-Hand Quicken FLEE/CRIT, Spear Quicken split,
    Katar Mastery CRIT bump at rank 10, Strip Helm MDEF reduction fix)
- Tree changes live in `db/import/skill_tree.yml` (Rogue / Baby Rogue get
  `ST_PRESERVE` from Plagiarism 10).

  All edits are guarded with `#ifndef RENEWAL` where appropriate, so renewal
  behavior is unchanged.
