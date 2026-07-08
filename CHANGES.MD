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

- **Snaring Arrow (rebalance):** Damage lowered by 20% across the board to
  `112% at Lv 1 scaling to 176% at Lv 5` (down from `140% -> 220%`). The root is
  now `0.5s shorter at every level` (`0.5s at Lv 1 scaling to 1.5s at Lv 5`), and
  the cooldown now scales down with skill level, `30 seconds at Lv 1 falling to
  20 seconds at Lv 5` (was a flat 3 seconds). *Developer Comments: the flat 3s
  cooldown let the reworked poke be chained into a near-permanent root while still
  dealing meaningful chip damage. Pushing the cooldown up an order of magnitude —
  and rewarding investment by tapering it as the skill levels — plus trimming both
  the damage and the root turns it back into a deliberate control opener instead
  of a spammable lock.*

### Hunter Skill Changes

- **Razorwing (formerly Spring Trap):** The old `HT_SPRINGTRAP` active falcon skill is converted into
  a `5`-level passive (`TargetType: Passive`), so it can no longer be cast directly and its SP cost is
  zeroed. While learned, it makes `Blitz Beat` cripple the movement of everything it strikes: a manually
  cast `Blitz Beat` slows every target it hits by `10% per skill level` (up to `50%` at Lv5), while a
  `Blitz Beat` triggered automatically against a `Hunted` target (the falcon-assist strike) slows by
  `5% per skill level` (up to `25%` at Lv5) instead. The slow lasts `2 seconds` and stacks with nothing
  — it uses a dedicated status (`SC_RAZORWING_SLOW`) that reports the highest movement penalty in play.
  *Developer Comments: Spring Trap was a niche WoE trap-popper that most Hunters never slotted. Recycling it into a Blitz Beat rider turns the falcon's signature strike into a real kiting tool, and splitting the manual (10%) from the auto-triggered (5%) value keeps the stronger slow tied to a deliberate cast rather than free falcon procs.*

- **Crippling Talons (formerly Razorwing):** `HT_SPRINGTRAP` is renamed to `Crippling Talons` with
  updated flavor text; the passive Blitz Beat slow effect is unchanged. Its prerequisites are
  changed to `Tranquillizing Dart Lv. 1` and `Blitz Beat Lv. 1` (previously `Jagged Fletching
  Lv. 5` and `Blitz Beat Lv. 1`). *Developer Comments: The name and tooltip now speak to the falcon's raking talons rather than a leftover trap, and swapping the Jagged Fletching gate for a Tranquillizing Dart point keeps the passive tied to Blitz Beat while making it easier to reach.*

- **Master Fletching (formerly Arrow Crafting):** The old `AC_MAKINGARROW` quest skill is rebuilt
  into `Master Fletching`, a `10`-level skill-point passive that ties the Hunter's arrow kit
  together. It is gated behind `Jagged Fletching Lv. 5` and `Tranquillizing Dart Lv. 1`, and each
  level grants three cumulative bonuses: `Tranquillizing Dart`'s cooldown is cut by `2 seconds per
  level` (a full `-20s`, from `60s` to `40s`, at Lv10); each `Serrated Shot` cast has a `5% per
  level` chance to shave `5 seconds` off `Wyvern Bolt`'s remaining cooldown (`50%` at Lv10, and
  only when Wyvern Bolt is actually on cooldown — the reduction cannot be banked); and `Snaring
  Arrow`'s root lasts `0.2 seconds per level` longer (up to `+2s` at Lv10). The old arrow-crafting cast is removed
  entirely (`TargetType: Passive`), and the `IsQuest` flag is cleared so it trains with skill
  points. *Developer Comments: Arrow Crafting was a dead quest skill in a world with vendor arrows, so its slot was recycled into a capstone that rewards Hunters for investing in the reworked Sandman/Serrated Shot/Ankle Snare line rather than sitting idle — the Serrated Shot proc in particular lets a Hunter who keeps up Serrated Shot pressure cut into Wyvern Bolt's long 40s cooldown.*

- **Wyvern Bolt (formerly Beast Strafing):** The old `HT_POWER` Double Attack combo skill is
  rebuilt into `Wyvern Bolt`, a normal `5`-level skill-point bow attack learnable directly in
  the Hunter tree with no prerequisites. The combo-only cast gate (which required the Double
  Attack `SC_COMBO` state) is removed, so it can be cast on any target, and the
  `IsSpirit` flag is cleared so it can actually be trained with skill points (spirit
  skills are unlearnable and only appear when Soul Linked). It is now a piercing
  line attack: a single hit lands on the target and on every enemy on the straight line
  from the caster through the target, continuing `2 cells` past it. Damage is
  `400% ATK + 100% per level` (`500%` at Lv1 up to `900%` at Lv5), and the bolt gains
  power with distance for up to `+50% more damage` at maximum range, scaling linearly
  with each target's distance from the caster. `MaxLevel 1 -> 5`; the old
  `2-hit, [50 + 8*STR]% ATK` formula is dropped for the single scaling hit; SP is
  `25 + 5 per level` (`30` at Lv1 up to `50` at Lv5); a `40 second` cooldown is added.
  It keeps its Bow + Arrow requirement and Vulture's-Eye sight range.
  *Developer Comments: HT_POWER was a dead-end combo tail nobody could reach without the exact
  Double Strafe -> Beast Strafing chain. Turning it into a learnable piercing line shot gives
  Hunters a directional AoE poke that rewards keeping distance; the 40s cooldown keeps its
  heavy damage a windowed opener rather than a spam button.*

- **Wyvern Bolt:** Every enemy the bolt strikes is now knocked back `2 cells` (pushed away
  from the caster, i.e. further along the bolt's line). *Developer Comments: leaning into the
  fantasy of a heavy piercing shot — the knockback punctuates the hit, opens space for a
  kiting Hunter, and scatters targets down the line rather than leaving them stacked on top of
  each other.*

- **Wyvern Bolt:** Now has a fixed `0.5 second` cast time. On this pre-renewal server cast
  time is a single variable value, so the `500ms` is flagged to ignore DEX, cast-reduction
  buffs and gear — the shot always takes exactly half a second to fire regardless of
  DEX or cast-reduction gear. *Developer Comments: an instant-cast piercing line nuke on a
  40s cooldown felt too twitchy and hard to react to; a short, uncappable wind-up gives the
  skill a readable tell and a real animation commitment without meaningfully hurting its
  burst role.*

- **Jagged Fletching (formerly Land Mine):** The old Land Mine trap is renamed to
  `Jagged Fletching` and converted from an active ground trap into a `passive` skill
  (`TargetType: Passive` / `INF_PASSIVE_SKILL`). It can no longer be cast, consumes no
  SP and no longer requires a `Booby Trap`; its old trap machinery (unit, duration, Stun
  status, `IsTrap`) is now inert. Its `MaxLevel` stays `5`, and it is gated behind
  `Serrated Shot (Phantasmic Arrow) level 10` in the Hunter tree. Blast Mine and Remove
  Trap still require it at level 1, so they inherit the new gating transitively.

  While learned, it barbs the Hunter's arrows so that `Double Strafe` and `Arrow Shower`
  leave a jagged arrow lodged in every target they hit. After `1.5 seconds` the barbs tear
  free, dealing `additional damage equal to 50% of that hit's damage` in a single burst. The
  extra damage is applied as fixed damage — it ignores DEF and cannot be resisted (no VIT/LUK
  reduction, no boss immunity), and it is credited to the Hunter. During the delay the target
  shows the bleeding visual, and hitting a target again before it tears free stacks the pending
  damage and refreshes the 1.5s timer. The effect is currently flat and does **not** scale
  with the level of Jagged Fletching. *Developer Comments: Land Mine was a dead trap slot once
  its ground-target playstyle fell off, so it is repurposed into a delayed-damage rider on the
  Hunter's two bread-and-butter bow skills — "it hurts when you pull the arrow out." The 1.5s
  fuse gives a small burst-timing game and the fixed, unresistable damage keeps it relevant
  against high-VIT and boss targets. Level scaling is intentionally left off for now while the
  numbers are being tested.*

- **Serrated Shot (formerly Phantasmic Arrow):** The quest skill Phantasmic Arrow is
  rebuilt into `Serrated Shot`, a non-targeted single-target bow attack. It is no longer a
  quest skill (`IsQuest` cleared) — it is now a `10`-level skill-point skill learnable
  directly in the Hunter tree. It is aimed via `TargetType: Self` + `NoTargetSelf`, so it
  auto-fires on your current target with no target cursor. Damage is now `300% ATK at Lv 1,
  +50% per level to 700% at Lv 9, then 800% at Lv 10` (up from a flat `150%`), with `9`
  cell range, an `8 second` cooldown, and `15 -> 30 SP` (`15, 17, 19, 21, 23, 25, 27, 29,
  30, 30`). It keeps its Bow requirement and weapon element. The old `3-cell knockback` is
  removed. In the skill window it is relocated up one row and three columns to the left in
  the Hunter tree. *Developer Comments: We wanted a spammable single-target damage button
  for Hunters whose pacing lives on an 8s cooldown, and Phantasmic Arrow's clean bow-shot
  animation fits "Serrated Shot" better than the trap skills. Knockback is dropped so the
  shot stays put for follow-ups; a Bleeding passive will supply its on-hit effect later.*

- **Serrated Shot (SP, crit & armor-piercing pass):** SP cost is flattened to a `flat 15 SP at
  all levels` (down from the old `15 -> 30` ramp). The skill can now land `critical hits` like a
  normal attack (`DamageFlags.Critical` / `NK_CRITICAL`), and its barbed heads `bypass 2% of the
  target's DEF per skill level` (up to `20%` at Lv 10), wired up in `battle_calc_defense_reduction`
  (pre-renewal only). *Developer Comments: Serrated Shot wanted to feel like a precise, armor-
  shredding bow strike rather than a flat-rate poke — enabling crits gives crit-DEX Hunters a
  reason to build around it, the DEF-piercing rewards leveling the skill against high-DEF targets,
  and the flat 15 SP removes the perverse "leveling it up costs more to cast" tax so investment is
  a pure upgrade.*

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
- **Blitz Beat (Hunted SP refund reinstated at 20):** Blitz Beat once again refunds SP when it
  consumes a `Hunted` mark, now `20 SP` (down from the original `30`). The refund only triggers on
  the empowered strike that spends the mark, so an ordinary single-target Blitz Beat still costs full
  price. *Developer Comments: removing the refund outright left the Hunted payoff feeling like it only
  ever cost the Hunter resources to set up. A smaller 20 SP return rewards cashing in the mark without
  restoring the near-free spam the old 30 SP refund enabled.*
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
- **Blitz Beat & Serrated Shot (shared cooldown):** The two falcon/bow strikes now share a single
  `5 second cooldown` — casting either one puts both on cooldown. Serrated Shot's own cooldown is
  lowered from `8 seconds` to `5 seconds` to match. *Developer Comments: both skills are the Hunter's
  instant, non-targeted burst button, and letting them be fired back-to-back on independent timers
  doubled the intended single-target spike. Binding them to one shared cooldown makes the choice each
  cycle "which strike do I want now" instead of "press both", without slowing either one in isolation.*
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
  now gains bonus damage equal to `2% of the Hunter's ATK (base ATK + weapon ATK) per Steel Crow
  level`, up to `+20% of ATK at level 10`. The old flat `+level` term folded into the falcon
  Misc-damage formula is removed. *Developer Comments: a flat +60 at max level was a rounding error
  once a Hunter had any real gear, so Steel Crow was a dead-end prerequisite point. Tying the bonus
  to the Hunter's own ATK makes the falcon scale with the bow and STR/gear the player actually
  invests in, giving every point a visible payoff across all three falcon skills instead of a token
  additive that gear quickly drowns out. The per-level bonus was subsequently trimmed from 2.5% to
  2% of ATK (25% -> 20% at max level) to pull the falcon's ATK-scaled ceiling back in line with the
  other Hunter skills.*

- **Tranquillizing Dart (formerly Sandman):** The old Sandman ground trap is rebuilt into
  `Tranquillizing Dart`, a single-rank (`MaxLevel 5 -> 1`) targeted bow skill. Instead of
  laying a hidden trap, it fires a dart at a single enemy at `bow range` (`Range -9`,
  extended by Vulture's Eye) and puts the target to `sleep for up to 15 seconds`. Like
  Snaring Arrow it lands a real (but deliberately tiny, `~1% ATK`) hit so the shot connects
  and the status applies — a true no-damage version routes through the no-damage cast path,
  which never resolves the on-hit effect. It costs `30 SP` (up from 12), gains a `60 second`
  cooldown, requires a bow and 1 Arrow, and no longer consumes a `Booby Trap`. In the Hunter
  tree it now requires `Ankle Snare level 1` instead of Flasher. *Developer Comments: as a
  5-level trap Sandman competed with the server's other, better traps and rarely got picked.
  Refocusing it into a single-point bow-range sleep dart gives Hunters a reliable crowd-control
  opener; the 1% ATK hit is only the delivery vehicle (the engine drops on-hit effects for
  zero-damage shots), and the long 60s cooldown and 30 SP cost are the price for a guaranteed
  15-second sleep.*

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
