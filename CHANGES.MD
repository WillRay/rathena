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

## General Changes

- **Ranged weapons no longer require ammo to be equipped (optional).** Added a new
  `arrow_require` battle config (`conf/battle/battle.conf`, default `yes` = official
  behavior). When set to `no`, bows and guns can perform normal attacks and use
  ammo-based skills without any arrow/ammo equipped. Arrow damage bonuses and ammo
  consumption still apply only when ammo *is* equipped, and mismatched-ammo checks
  remain enforced when ammo is equipped.
  - `src/map/battle.hpp` — added `arrow_require` config field.
  - `src/map/battle.cpp` — registered the config and gated the normal-attack ammo
    requirement on it.
  - `src/map/skill.cpp` — gated the skill ammo requirement on it.
  - `conf/battle/battle.conf` — documented the new `arrow_require` setting.

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

### Archer Skill Changes

- **Attention Concentrate:** SP cost is now `10 SP + 3 per skill level` (13 SP
  at level 1, 40 SP at level 10) — half of the previous `20 SP + 6 per skill
  level` curve. The `Vulture's Eye 1` prerequisite has been removed, so the
  skill can be learned directly, and it has been moved up two rows and one
  column to the right in the Archer skill tree to sit alongside the other core
  self-buffs. The buff still lasts a flat `5 minutes` at every level, and
  casting it also clears the `Confusion` status (upside-down screen) from the
  caster. *Developer Comments: Attention Concentrate is a baseline Archer buff
  that everyone keeps up permanently, so gating it behind Vulture's Eye and
  charging a steep SP toll only taxed low-level Archers for no real choice;
  halving the cost and freeing the prerequisite lets it read as the core AGI/DEX
  buff it actually is. The Confusion cleanse gives Archers a way to self-correct
  a common status.*

- **Arrow Crafting:** The teaching quest can now be started at `Job Level 15`
  and only consumes `5 Trunks`, down from 13. *Developer Comments: In line with
  the other Platinum skill reworks, a smaller material cost matches the lower
  level at which the skill becomes available.*

- **Double Strafe:** Damage raised by `120 percentage points` at every level,
  from `100% + 10% per level above 1` (100% at Lv 1, 190% at Lv 10) to
  `220% + 10% per level above 1` (220% at Lv 1, 310% at Lv 10). The skill now
  carries a `3 second cooldown` and no longer requires or consumes an arrow —
  it fires from a Bow with no ammunition. SP cost is now a very approachable
  `3 + 1 per skill level` (4 SP at Lv 1, 13 SP at Lv 10), down from the earlier
  `12 + 1 per level` curve. *Developer
  Comments: Double Strafe was the Archer's bread-and-butter nuke but was
  throttled by arrow economy and trivially spammable; dropping the ammo tax
  while adding a short cooldown turns it into a heavy-hitting opener rather than
  a machine-gun. The SP curve was pulled down hard so this core spam skill
  never feels SP-starved — the 3s cooldown, not the SP bar, is what paces it.*

- **Double Strafe (targeting):** Double Strafe is now a non-targeted skill. Instead
  of prompting for a target cursor, it is cast on the caster and automatically
  redirects to your current target (`TargetType: Self` + `NoTargetSelf`, which the
  engine resolves to a combo-style `CAST_DAMAGE` on `ud->target`). If you have no
  target, the skill simply does nothing. *Developer Comments: Requiring a fresh
  target click on every cast made the Archer's main spam skill feel clunky next to
  auto-attack; letting it snap to whatever you are already fighting keeps the
  rhythm on the 3s cooldown instead of the mouse. Vulture's Eye range bonus is
  preserved.*

- **Arrow Shower:** Damage raised by `50 percentage points` at every level, from
  `80% + 5% per level above 1` (80% at Lv 1, 125% at Lv 10) to `130% + 5% per
  level above 1` (130% at Lv 1, 175% at Lv 10). The skill now has a `5 second
  cooldown`, no longer knocks enemies back, and from `rank 5 and up strikes
  twice` in a single cast. All arrow flavor and the pushback/trap-scattering
  description have been dropped from the tooltip (the skill still fires from a
  Bow with arrows as normal). *Developer Comments: Arrow Shower was a niche trap-pushing
  utility that did too little damage to be picked as an attack. Removing the
  knockback lets it actually hit a clumped pack, and the double hit past rank 5
  plus the higher ratio finally give Archers a real AoE nuke; the cooldown keeps
  it from replacing single-target spam outright.*

- **Charge Arrow:** No longer a quest skill — it is now a `5-level skill-point
  skill` learned in the tree directly under Arrow Shower (column 3, row 3),
  gated behind `Arrow Shower 5`. Damage is raised to `300% + 20% per skill
  level` (320% at Lv 1, 400% at Lv 5), up from a flat 150%, knockback is cut
  from `6 cells to 3`, and SP is now a very approachable `4 + 1 per skill level`
  (5 SP at Lv 1, 9 SP at Lv 5), down from the earlier `15 + 1 per level` curve.
  *Developer Comments: Locking a hard-hitting single-target
  knockback behind a quest kept most Archers from ever touching it; folding it
  into the normal tree as an Arrow Shower follow-up gives the class a reliable
  ranged finisher, and the shorter knockback stops it from shoving targets out
  of follow-up range. The SP cost was cut to near-nothing so the finisher is
  always available; Arrow Shower keeps its heavier cost as the AoE nuke. It now
  carries a `15 second cooldown`. Developer note: with SP costs cut to near-nothing,
  nothing stopped the finisher from being spammed on cooldown-free repeat; a 15s
  cooldown keeps it as a hard-hitting opener rather than a sustained-DPS button.*

- **Snaring Arrow (formerly Ankle Snare):** Reworked from a ground trap into a
  single-target bow attack. Instead of being placed on a cell, it is aimed
  directly at a target enemy within bow range. It deals a light `140% + 20% per
  skill level` hit (140% at Lv 1, 220% at Lv 5) and briefly roots the target with
  the old Ankle immobilize for `1 second at Lv 1 scaling to 2 seconds at Lv 5`.
  A rooted target displays a visible "bound" cage effect for the duration so it
  is clear the snare landed. It fires from a `Bow` and consumes `1 Arrow`, costs
  `5 + 1 per skill level SP`
  (5 SP at Lv 1, 9 SP at Lv 5), and carries a `3 second cooldown`. The Booby Trap
  reagent, the trap placement and the old 4-20 second ground snare are all gone.
  *Developer Comments: The trap version asked Hunters to pre-place a
  reagent-hungry snare and hope an enemy wandered onto it, which never fit an
  active bow line. Turning it into a cheap, targeted poke that roots what it hits
  gives Archers a real kiting tool; the short root paired with a 3s cooldown makes
  it a repeatable window rather than a permanent lock, and bosses eat the damage
  but ignore the root so it stays a leveling and PvP utility, not an MVP
  trivializer.*

### Hunter Skill Changes

- **Beast Bane:** Reworked from a flat damage mastery into a passive "Hunted"
  hunter's mark. The old `+4 ATK per skill level` bonus against Brute and Insect
  monsters (plus the Hunter Spirit STR rider) is gone entirely. In its place,
  every offensive skill the Hunter lands now has a chance to mark its target with
  `Hunted` (`SC_HUNTED`, a 7-second mark). The chance scales
  with skill level, running `10% at Lv 1 to 60% at Lv 10`, interpolated linearly
  across the ten levels. Plain auto attacks also carry a smaller `Skill Level%`
  chance to plant the same mark (see the entry below), and Falcon
  skills (`Blitz Beat`, `Detecting`, `Spring Trap`) never apply it — the offensive
  and auto-attack rolls come only from the Hunter, not the falcon. *Developer Comments: A small race-limited ATK mastery gave Hunters no
  reason to max Beast Bane past the Falcon/Blitz prerequisite. Turning it into
  the class's source of the Hunted mark makes it the backbone passive of the
  hunter's-mark identity — every point now widens how reliably your skills keep
  targets tagged.*

- **Beast Bane (auto-attack mark):** On top of the offensive-skill stamp above, every
  plain auto attack now has a flat `Skill Level%` chance to plant the `Hunted` mark on the
  target — `1% at Lv 1 to 10% at Lv 10`. The roll is skipped when the target is already
  `Hunted`, so a white hit never re-rolls (or spends) an existing mark; auto attacks still
  neither consume the mark nor trigger the falcon assist. *Developer Comments: leaving auto
  attacks out entirely meant a Hunter kiting with white hits between skill casts let marks
  lapse for no reason. A small, skill-level-scaled trickle keeps targets tagged during downtime
  without approaching the reliability of an actual skill cast, so maxing Beast Bane still pays
  off mostly through active play.*

- **Blitz Beat (falcon assist):** The `Hunted` mark now reads as "my next offensive skill on
  this target is assisted by my falcon." When a Bow-wielding, Falcon-equipped Hunter lands any
  of their other offensive skills on a `Hunted` target, the falcon automatically strikes it
  with a `single-target Blitz Beat` and the mark is consumed. There is no separate proc roll —
  the mark itself is the gate, so a tagged target always draws the assist. Beast Bane no longer
  stacks a fresh mark on a target that is already `Hunted` (that hit spends the existing mark
  instead), and auto-attacks never consume the mark or draw the falcon assist. *Developer Comments: the old
  proc rolled its own chance on top of the mark's application chance, which double-dipped the
  RNG and made the falcon feel unreliable. Making the mark a guaranteed one-shot "assist token"
  that any skill can cash in gives the Hunted loop a clear cadence: plant with one skill, spend
  with the next.*
- **Blitz Beat (cooldown & Hunted payoff):** Blitz Beat now has a `5 second cooldown`.
  Cast directly on a target carrying the `Hunted` mark, the strike consumes the mark to deal
  `40% more damage` and widen into a `5x5 area attack` (splash radius 2, the nearest centered
  area to a 4x4 footprint) centered on the Hunted foe; nearby enemies caught in the splash take
  the normal strike. Any falcon strike — the manual empowered cast or the assist proc above —
  always spends the mark. The earlier `Hunted`-consume bleed (and its `SC_HUNTED_DOT` status)
  is removed entirely. *Developer Comments: the bleed was a fiddly second formula bolted onto
  the strike. Folding the payoff straight into Blitz Beat — a burst of extra damage plus a small
  area so a tagged pack gets cleared — reads more clearly and keeps the reward on the skill
  itself. Directing the mark yourself with a manual Blitz Beat is the burst option; letting
  another skill spring the falcon assist is the automatic one, and either way the mark is spent.*
- **Blitz Beat (targeting & cast):** Blitz Beat is now `non-targeted` and `instant-cast`.
  It no longer asks for a target cursor - it auto-fires on your `current auto-attack target`
  (via the `TargetType: Self` + `NoTargetSelf` flag pair, which the engine resolves to
  `ud->target`), and its `1.5s cast time` is removed. Vulture's Eye range scaling
  (`AlterRangeVulture`) is preserved. *Developer Comments: as a finisher meant to be woven
  into the Hunter's auto-attack flow, the target-click and cast bar broke the rhythm; letting
  it snap onto whatever you're already shooting keeps its pacing on the 5s cooldown where it
  belongs.*
- **Blitz Beat (SP cost & Hunted damage):** SP cost is now a flat `15 SP` at every level
  (down from the old `10/13/16/19/22` curve), and the empowered strike against a `Hunted`
  target now deals `100% more damage` (double damage) instead of `40% more`. *Developer
  Comments: the scaling SP curve punished maxing the skill for no real reason now that Blitz
  Beat is a woven-in finisher, so a single flat cost keeps it spammable on its cooldown. The
  40% Hunted bonus was too small to feel like a payoff for setting up the mark; doubling the
  strike makes landing a manual Blitz Beat on a tagged foe the real burst it was meant to be.*
- **Blitz Beat (Hunted SP refund removed):** The `30 SP` refund a Blitz Beat used to grant when it
  struck a `Hunted` target has been removed. The strike still deals double damage and widens into a
  5x5 area against a Hunted foe, and still consumes the mark — it just no longer pays SP back.
  *Developer Comments: the refund let a Hunter who kept a target tagged effectively cast Blitz Beat
  for free on its cooldown, which was too much sustain on top of the damage and area the mark already
  grants. Dropping it keeps the Hunted payoff about burst, not resource generation.*
- **Blitz Beat (base damage formula):** The falcon strike's base Misc-damage formula now counts
  the Hunter's `full INT` instead of half. It goes from `(LUK + INT / 2 + 20) x 2` to
  `(LUK + INT + 20) x 2` per hit, before the Steel Crow, Hunted and hit-count multipliers apply.
  Falcon Assault and Predatory Strike, which are both built on the Blitz Beat base formula,
  benefit by the same amount.
  *Developer Comments: Blitz Beat is meant to be the INT-scaling payoff of a Falcon build, but
  only crediting half of INT made the stat feel weak on the one skill that should reward it.
  Counting full INT gives INT-invested Hunters a damage floor that actually tracks their build.*
- **Predatory Strike (formerly Detecting):** Detecting is repurposed from a pure reveal utility
  into an offensive falcon strike and renamed `Predatory Strike`. It is now a `5-level` skill
  (was 4), relocated in the skill tree to the first column directly under Steel Crow and requiring
  `Steel Crow 10` (its old Improve Concentration 1 / Falconry Mastery 1 prerequisites are dropped).
  Cast on the ground, it still reveals every hidden unit and trap in the area, but now also strikes
  each enemy in a `9x9 cell area` (splash radius 4) for `3x a Blitz Beat hit` dealt over `5 hits`.
  Every enemy struck is inflicted with `Fear for 2 seconds` (fixed duration, ignoring stat
  resistance) and stamped with the `Hunted` mark (7s). SP cost is `30 + 5 per level` (35 SP at Lv 1
  to 55 SP at Lv 5) and the skill carries a `30 second cooldown`. *Developer Comments: Detecting
  was a single-purpose reveal button that almost no Hunter kept on the bar once traps and Blitz were
  online. Folding it into the Hunted-mark loop turns it into a genuine opener — a wide AoE that
  reveals a pack, softens it with Fear, tags everything as Hunted for the falcon-assist payoff, and
  lands a meaningful burst of its own. Splitting the 3x Blitz Beat total across five hits lets it
  chew through DEF-stacked packs a little better than one fat hit would. The 30s cooldown keeps it
  an opener rather than a spam nuke. Fear already exists in the pre-renewal status database, so no
  renewal-only status wiring was needed — only the fixed 2s application.*
- **Predatory Strike:** The falcon strike now scales its damage with skill level instead of being
  flat. Damage rises from `300% of a Blitz Beat at level 1` by `50% per level` to `500% at level 5`
  (300 / 350 / 400 / 450 / 500%). *Developer Comments: A fixed 300% made every point past level 1
  buy only SP cost and cooldown with no payoff on the damage itself. Tying the multiplier to skill
  level gives Hunters a real reason to max the skill and rewards the Steel Crow 10 investment it now
  gates behind.*
- **Predator's Eye (formerly Beast Bane):** The Brute/Insect mastery passive is renamed from
  `Beast Bane` to `Predator's Eye`. Cosmetic only — its STR-scaled mastery-damage behaviour is
  unchanged; only the display name in the skill window, tooltips and cross-references (Soul Link,
  Beast Strafing) moves. *Developer Comments: A name refresh to sit alongside the reworked
  Predatory Strike and read more like an active Hunter identity; no gameplay values were touched.*
- **Falconry Mastery:** Converted from a passive into an active self-cast toggle that summons or
  dismisses the Hunter's falcon, mirroring the Falcon Flute item. Costs `0 SP` and has a fixed
  `2 second` cast time that DEX cannot reduce. Learning the skill still gates whether a falcon can be
  summoned (so it remains the prerequisite for the Falcon tree), and it refuses to fire while a warg
  is out. *Developer Comments: Requiring a consumable Falcon Flute just to re-perch a falcon after a
  map change or death was pure friction. Folding the toggle onto the mastery skill itself gives
  Hunters a one-button summon/dismiss without touching their inventory, while the free cost and short
  fixed cast keep it a convenience rather than a combat action.*
- **Steel Crow:** Reworked from a flat `+6 damage per skill level` mastery into an ATK-scaling
  falcon amplifier. Every falcon strike — `Blitz Beat`, `Falcon Assault` and `Predatory Strike` —
  now gains bonus damage equal to `2.5% of the Hunter's ATK (base ATK + weapon ATK) per Steel Crow
  level`, up to `+25% of ATK at level 10`. The old flat `+level` term folded into the falcon
  Misc-damage formula is removed. *Developer Comments: a flat +60 at max level was a rounding error
  once a Hunter had any real gear, so Steel Crow was a dead-end prerequisite point. Tying the bonus
  to the Hunter's own ATK makes the falcon scale with the bow and STR/gear the player actually
  invests in, giving every point a visible payoff across all three falcon skills instead of a token
  additive that gear quickly drowns out.*

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

- **Hiding — Opportunist:** Coming out of `Hiding` now grants `Opportunist`,
  the same opening earned from Improve Dodge — it halves the SP cost of your
  next damaging skill (`8 second` duration). Like the flee-dodge proc, it is
  fully disabled in PvP and WoE. *Developer Comments: This closes the stealth
  loop the Opportunist buff was built for — duck into Hiding, then emerge into
  a discounted opener. Unlike the `5%` flee-dodge roll this is a guaranteed
  grant, since committing to Hiding already costs a turn and the SP upkeep, so
  the payoff should be reliable. PvP and WoE stay gated for the same reason as
  the dodge proc: SP sustain layered on stealth would warp duels.*

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

- **Envenom — Non-Targeted Cast:** Envenom no longer opens a target cursor.
  It is now flagged `TargetType: Self` + `NoTargetSelf`, which makes the engine
  treat it as a combo-style damage skill (`skill_get_casttype` -> `CAST_DAMAGE`)
  that auto-fires on your current target (`unit.cpp`: `target_id = ud->target`).
  Press it once and it lands on whatever you are already attacking; with no
  current target the cast simply does nothing. *Developer Comments: Envenom is a
  spam skill you weave into auto-attacks, and the extra target-click was pure
  friction — pacing it on ASPD and SP rather than the mouse is how it should
  have felt all along. Pairs directly with the Auto-Attack Resume change above.*

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

- **Grimtooth:** Reworked into the Assassin's stealth burst window. Damage
  raised from `100% + 20% per level` to `300% + 50% per level` (350% at
  level 1 up to 550% at level 5), the attack now always hits (ignores the
  target's Flee), and it is gated by a `2 second cooldown` so it is a
  window rather than a spam. Splash grows with level — `3x3` at levels 1–2,
  `5x5` at levels 3–4 and `7x7` at level 5 — and the skill is now usable with
  Daggers as well as Katars. Hiding is the primary way to open it, though an
  `Opportunist` charge can now substitute for it (see the Opportunist entry
  below).
  *Developer Comments: Grimtooth was a weak, niche payoff that rarely
  justified the Hiding setup. Giving it a real damage floor, guaranteed hits
  and a short cooldown turns it into the shared stealth burst for both Katar
  and dual-dagger builds; unlocking Daggers (alongside the matching change to
  Sonic Blow) is what lets a double-dagger stealth Assassin have a real burst.
  The guaranteed hit lets daggers spend their card slots on damage instead of an
  accuracy tax.*

- **Sonic Blow — Dagger Support:** Sonic Blow can now be cast with `Daggers` as
  well as `Katars`, and it no longer requires `Katar Mastery 4` to learn — the
  skill-tree prerequisite is removed, so it can be picked up directly. The
  per-level SP cost (`16` at level 1 up to `34` at level 10) and every other
  property are unchanged. *Developer Comments: The Opportunist loop leans on
  landing Sonic Blow to bank a charge, but a dual-dagger stealth Assassin could
  never press the skill — it was Katar-only to cast and gated behind Katar
  Mastery, a skill dagger builds have no reason to take. Opening Sonic Blow to
  Daggers and dropping the Katar Mastery gate lets dagger builds run the same
  Sonic Blow -> Opportunist -> Grimtooth rotation the rework was designed
  around, and pairs with Grimtooth's own Dagger unlock so the whole dagger
  stealth kit is coherent rather than half-locked to Katars.*

- **Opportunist — Sonic Blow & Grimtooth:** The `Opportunist` opening earned
  from Improve Dodge (a `5%` flee-dodge roll) and from leaving Hiding now has a
  third source and a second use, closing the stealth loop into a full rotation.
  Sonic Blow gains a `25% chance` on a successful hit (PvE only) to grant
  `Opportunist` — the same `8 second` charge that halves the SP cost of your
  next damaging skill. Grimtooth can now spend that charge instead of requiring
  Hiding: opened while not hidden it consumes the charge and lands as a
  single-target strike (the splash is suppressed) that still always hits, and
  opened from Hiding it keeps its full splash exactly as before. Like the other
  Opportunist sources, the Sonic Blow proc is fully disabled in PvP and WoE.
  *Developer Comments: Opportunist started life as a small flee-dodge reward,
  but with Hiding and now Sonic Blow feeding it, it becomes the connective
  tissue of the stealth Assassin's rotation — land a Sonic Blow, bank the
  charge, and either discount your next nuke or fire a Grimtooth out of stealth
  without paying the Hiding tax. Grimtooth's charge-cast is deliberately
  single-target so the out-of-Hiding option is a precision opener, not a free
  AoE: the full splash stays the reward for actually committing to Hiding. The
  `25%` rate is high enough to be part of the plan without being guaranteed, and
  the PvP/WoE gate matches the other Opportunist sources so stacked SP sustain
  never warps duels.*

- **Sonic Blow — Non-Targeted Cast:** Sonic Blow no longer opens a target
  cursor. It is now flagged `TargetType: Self` + `NoTargetSelf`, so the engine
  treats it as a combo-style damage skill (`skill_get_casttype` -> `CAST_DAMAGE`)
  that auto-fires on your current target (`unit.cpp`: `target_id = ud->target`).
  Press it once and it strikes whatever you are already attacking; with no
  current target the cast does nothing. The Katar/Dagger weapon requirement is
  unchanged. *Developer Comments: Sonic Blow is the Assassin's burst opener and
  the core of the stealth rotation — removing the target-click keeps the flurry
  flowing off cooldown and ASPD instead of the mouse, which matters most when
  chaining into the Opportunist -> Grimtooth follow-up.*

- **Venom Dust:** Reworked from a poison-status trap into a damaging poison
  cloud. The `2 Venom Canines` reagent is removed; the cloud now lasts a flat
  `10 seconds` and is gated by a `20 second cooldown`. Its element is switched
  from Poison to Weapon and it now deals a direct `50% + 10% per level`
  weapon-damage tick every second (60% at level 1 up to 150% at level 10), on
  top of the existing `Poison` status chance. *Developer Comments: The old cloud did nothing but a
  poison-status DoT, so it was dead weight against the poison-immune MVPs
  this pass is aimed at. Routing venom power into a direct, Weapon-element
  tick — rather than the poison status — is what makes it contribute on
  bosses; the poison is now gravy. The reagent gave way to a cooldown so
  the only economy is timing.*

- **Venom Splasher:** The `75% target HP` requirement, the `Red Gemstone`
  cost, and the `boss / status-immune` cast gate are all removed. The skill is
  gated by a `7.5s → 12s per-level cooldown`, it always hits (`IgnoreFlee`),
  and it deals direct `400% + 50% per level` Weapon-element splash damage
  (550% at level 1 up to 1000% at level 10) rather than poison-status damage.
  The detonation timer is a clean linear `8 seconds at level 1` down to
  `2 seconds at level 10` so the bomb detonates sooner. *Developer Comments: This is the venom burst
  finisher, so it has to be usable on the MVPs this pass targets. The bomb
  planted the SC_SPLASHER timer status via a hardcoded `MD_STATUSIMMUNE`
  check that failed the cast outright on every boss — the exact monsters the
  skill exists to finish. Because the SC_SPLASHER status carries no BossResist
  flag and detonation deals pure direct weapon damage (not poison-status
  damage), dropping that gate lets the bomb stick to bosses and blow up for
  full damage. The `400% + 50%` ratio (capping at `1000%`) is kept, and the
  fuse was shortened to an `8s → 2s` curve so the finisher pays off quickly.*

- **Enchant Poison:** Buff duration extended to `5 minutes` at every rank and
  the poisoning proc chance scales up to `10% at rank 10`. It now also grants
  a flat ATK bonus of `+5 per skill level` (`+50 ATK at rank 10`) for its
  duration, which applies even against poison-immune targets.
  *Developer Comments: Poison sits at the center of the Assassin's kit, so
  the buff that enables it should not be a chore to maintain — and it should
  do something on the bosses that shrug off the poison status. The flat ATK
  is the venom analog to Cloaking's ambush setup: a deliberate damage window
  that is not conditional on the enemy actually being poisoned. No cooldown
  was added: with a maintainable 5-minute buff a cooldown would be inert
  clutter, so the "window" here is simply keeping the buff up.*

- **Katar Mastery:** Continues to grant `+4 ATK per skill level`
  (40 ATK at rank 10). Now also grants `+5% nominal CRIT at rank 10`,
  which becomes roughly `+10% effective` once the katar's double crit
  rate is applied. *Developer Comments: With the poison line now viable,
  the CRIT-Assassin needed a matching bump so it stays competitive with
  pure poison builds rather than being a strictly weaker option.*

- **Poison React:** Reduced to `5 levels`, and its bespoke damage mechanics are
  removed. It no longer empowers the caster's next attack with a `+30% per level`
  ATK boost, and the reactive Envenom autocast is no longer locked to level 5 —
  when the buffed caster is struck it now simply autocasts `Envenom` on the
  attacker at the caster's own learned Envenom level, consuming one counter per
  reaction. Counters were doubled to `one per skill level` (1 at level 1 up to
  5 at level 5). The poison-element attack block / retaliation mode was dropped
  along with the damage boost. *Developer Comments: Poison React carried its own one-off damage
  formula and a hardcoded level-5 Envenom that ignored how much the player had
  actually invested in Envenom. Collapsing it to "cast your Envenom at your
  level when hit" makes it a clean extension of the reworked Envenom rather than
  a separate damage table to balance. Note: this removes the old poison-attack
  block, so Poison React is now purely offensive reactive pressure.*

- **Venom Knife:** No longer consumes a thrown `Dagger`; instead it is gated
  by a short `2 second cooldown`. In pre-renewal it now deals direct
  `150% + 30% per level` ranged damage (`180% ATK` at its single level)
  instead of a flat `100% ATK`, and keeps its high poison chance.
  *Developer Comments: Venom Knife was a flavour poke that cost a real Dagger
  every throw for almost no damage. Turning the ammo cost into a cooldown and
  giving it a genuine direct-damage ratio makes it a usable ranged venom
  opener for dagger-venom Assassins without the inventory drain.*

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
  via `db/import/skill_db.yml` and `db/import/status.yml` wherever the change is
  expressible there, so future upstream merges of rAthena will not silently
  revert this balance pass. The one exception is requirement/flag *removals*,
  which the import loader cannot express and therefore live in
  `db/pre-re/skill_db.yml` (each marked with a `Payon Stories rebalance`
  comment): Venom Dust's `NoDamage` flag (cleared so the cloud can deal direct
  ticks) and Venom Knife's `Ammo` requirement (the thrown Dagger cost). These
  match the precedent already set by Stone Fling's item-requirement removal.
- The Bowling Bash configuration in `conf/battle/skill.conf` now defaults to
  `bowling_bash_area: 2` (area around caster). Restoring the legacy gutter-line
  behavior is still possible by setting it back to `0`.
- The engine-level changes live in:
  - `src/map/skills/swordman/holycross.cpp` / `holycross.hpp`
  - `src/map/skills/swordman/smite.cpp` (Shield Charge / Smite)
  - `src/map/skills/swordman/bowlingbash.cpp`
  - `src/map/skills/thief/venomsplasher.cpp` (drops the 75% HP gate and the
    boss / status-immune cast gate)
  - `src/map/skills/thief/grimtooth.cpp` (stealth-burst damage floor)
  - `src/map/skills/thief/venomdust.cpp` (per-tick direct-damage ratio)
  - `src/map/skills/thief/throwvenomknife.cpp` (pre-re direct ranged damage)
  - `src/map/skill.cpp` (`UNT_VENOMDUST` now deals a direct weapon-damage tick)
  - `src/map/skills/thief/backstab.cpp` (gap-closer redesign)
  - `src/map/battle.cpp` (Poison React: learned-level Envenom autocast, removal
    of the +30%/level damage boost and the poison-attack block mode)
  - `src/map/status.cpp` (Two-Hand Quicken FLEE/CRIT, Spear Quicken split,
    Katar Mastery CRIT bump at rank 10, Strip Helm MDEF reduction fix,
    Enchant Poison flat ATK bonus)
- Tree changes live in `db/import/skill_tree.yml` (Rogue / Baby Rogue get
  `ST_PRESERVE` from Plagiarism 10).

  All edits are guarded with `#ifndef RENEWAL` where appropriate, so renewal
  behavior is unchanged.
