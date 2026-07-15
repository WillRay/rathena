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

- **Sword Mastery (renamed "Blade Mastery"):** Trimmed from `10` levels down
  to `5`. Now grants `+4 ATK per skill level` and `+4 HIT per skill level` (up
  to `+20 ATK` / `+20 HIT` at level 5) as real additions to the character's
  ATK and HIT stats while wielding a One-Handed Sword, Two-Handed Sword, or
  Dagger, both visible in the status window. *Developer Comments: A 10-level
  passive that only did flat, hidden damage was a lot of skill points for a
  bonus players couldn't even see reflected on their sheet. Five levels gets
  it filled out early, putting the ATK and HIT bonuses directly on the
  visible stats means sword builds (heavier weapons, Blade Mastery Rogues)
  can actually see what they're getting for the points, and extending it to
  Two-Handed Swords means the rename to "Blade Mastery" isn't just cosmetic —
  it now genuinely covers every blade a Swordsman can wield instead of ceding
  greatswords to Relentless Assault alone.*
- **Two-Handed Sword Mastery (renamed "Relentless Assault"):** Trimmed from `10` levels down to `5` and
  reworked from a flat 2H-sword ATK bonus into a skill-into-melee proc.
  Landing any offensive skill now grants a buff, lasting `5` seconds, that
  empowers the caster's very next auto attack with `+10% damage per skill
  level`; the buff consumes itself on that attack, or simply expires if no
  auto attack lands within the window. The Parry prerequisite is lowered
  from level 10 to level 5 to match. *Developer Comments: A flat 2H-sword-
  only ATK bonus didn't do anything for the growing number of Swordsmen who
  split time between skills and steel, and it was invisible outside of
  damage meters. Turning it into a payoff for actually alternating skills
  and swings gives auto-attack Swordsmen a reason to pick it up, and it
  reads clearly on screen instead of disappearing into a damage formula.*
- **Bash:** SP cost reduced to a smooth `3 + 1 per skill level` (4 SP at level 1
  up to 13 SP at level 10). *Developer Comments: Previously the skill had only
  two SP costs — one for levels 1–5 and another for 6–10. Because damage scales
  linearly with skill level, SP cost should scale with it too, and players who
  prefer a mid-level Bash should not be punished for stopping short of 10.*

- **Bash:** Now non-targeted — it automatically fires on your current target
  instead of requiring a target-cursor click, via the `Self` target type plus the
  `NoTargetSelf` flag. *Developer Comments: Bash is a spam skill, and forcing a
  target click on every press interrupts its flow. Auto-firing on the current
  target keeps its pacing on cooldown and ASPD, where it belongs, rather than on
  the mouse. No damage, SP, or range values change.*

- **Bash:** Now has a 1 second cooldown (`Cooldown: 1000`). *Developer Comments:
  Pairs with the non-targeted change above — with the target-click removed, Bash
  can be spammed as fast as the client sends packets, so a short 1s cooldown caps
  its cadence and keeps its DPS where it was intended rather than letting it
  outpace ASPD.*

- **Bash:** Cooldown doubled to 2 seconds (`Cooldown: 2000`). *Developer
  Comments: The 1s cooldown from the non-targeted change above still let Bash
  outpace its intended cadence at high ASPD; doubling it reins that back in.*

- **Magnum Break:** Splash radius increased to 4 cells (9×9 area) and SP cost
  lowered from 30 to 18. *Developer Comments: With the lower SP cost and the
  larger area, Swordsmen actually have a viable mobbing tool at low levels.
  Compared to second-class AoEs it is still on the modest side, but it should
  finally feel worth pressing.*

- **Combat Recovery:** No longer a quest-only skill — it is now a `5`-level
  skill learned with skill points, requiring Increase HP Recovery Rank `5`.
  *Developer Comments: Gating a kill-triggered heal behind a one-time quest
  meant most Swordsmen either had it maxed forever or never bothered with it
  at all. Putting it on the tree behind Increase HP Recovery gives players a
  real point-investment choice and ties the two skills together thematically.*
- **Combat Recovery (reworked effect):** No longer grants passive HP/SP
  regen while walking. Instead, defeating an enemy now heals `10% per skill
  level` (up to `50%` at level 5) of what Increase HP Recovery's sit-regen
  tick would restore (`skill_lv_recovery * 10 + skill_lv_recovery * MaxHP /
  250`), and this proc has an internal cooldown of `5` seconds. *Developer
  Comments: The old walking-regen effect was easy to miss and did nothing to
  make combat itself feel rewarding. Tying a slice of Increase HP Recovery's
  healing to landing the killing blow gives Swordsmen a small, noticeable
  trickle of sustain for staying aggressive, scaling with the points they've
  put into the skill, while the 5 second cooldown keeps it from turning into
  a farming-heal loop against weak mobs.*
- **Combat Recovery (removed free grants):** No longer auto-granted on
  Swordman rebirth or by the Platinum Skill NPC / Jobmaster's
  `F_GetPlatinumSkills`. The "Knight De Thomas" quest NPC that used to teach
  it for 50 Empty Bottles has been retired (`npc/quests/skills/
  swordman_skills.txt`, and the Izlude placements in the `pre-re`/`re`
  variants of that path). Fatal Blow and Auto Berserk still grant for free at
  rebirth/from the Platinum Skill NPC — only Combat Recovery changed.
  *Developer Comments: These were all leftover from when the skill was
  quest-only, and each one silently handed it out for free, bypassing the
  new Increase HP Recovery Rank 5 requirement. A skill that's supposed to
  cost tree points shouldn't also be a freebie three other ways.*

- **Increase HP Recovery:** Trimmed from `10` levels down to `5`, with the
  per-level regen bonus doubled to compensate — level 5 now restores
  `50 HP + 2.0% of MaxHP`, matching the old level 10 total, and the
  potion-healing bonus from this skill is doubled to `+20% per skill level`
  (up to `+100%` at level 5, same ceiling as before). The regen tick interval
  is unchanged at every `10` seconds while idle.
  *Developer Comments: Another 10-level passive that took forever to feel
  meaningful point-for-point. Five levels gets it to full strength twice as
  fast without changing the cap players were already building toward.*

- **Endure:** Trimmed from `10` levels down to `5`, with the MDEF bonus
  doubled per level — level 5 now grants `MDEF +10` for `37` seconds, the
  same totals the old level 10 gave. *Developer Comments: Endure's MDEF was
  creeping up by a single point every level, which made the back half of the
  skill barely register. Halving the level count and doubling the per-rank
  MDEF gets Swordsmen to the same ceiling for half the skill points.*

### Knight Skill Changes

- **Tension Relax:** Its Increase HP Recovery prerequisite is lowered from
  level 10 to level 5. *Developer Comments: Increase HP Recovery was trimmed
  to a 5-level skill, so the old level 10 gate was no longer reachable and
  would have made Tension Relax unlearnable. Level 5 restores the original
  "max out Increase HP Recovery first" requirement.*

- **Two-Hand Quicken:** The flat ASPD bonus has been removed and replaced with
  **Momentum**, a stacking buff (new `SC_MOMENTUM` status, shown with its own
  icon so it does not share Two-Hand Quicken's status slot — the Sorcerer's
  Striking icon is borrowed, since `SO_STRIKING` has no entry in the pre-re
  skill tree and is unreachable/unused on this server) that only accrues
  while Two-Hand Quicken is active. Landing any
  auto-attack, or taking a melee auto-attack, adds a stack; stacks cap at 5,
  last 15 seconds, and each new stack refreshes the timer. Each stack reduces
  weapon swing delay by `7%` (up to `35%` at 5 stacks — 5 points better than the
  old flat `+30%` ASPD-rate at full stacks). The skill still grants
  `+1 FLEE per skill level` and `+0.8% CRIT per skill level`, unchanged. The
  in-client skill description and the borrowed status icon's hover tooltip
  have both been updated to describe the new behavior.
  *Developer Comments: A flat, front-loaded ASPD buff meant the payoff was the
  same whether you fought for one hit or one minute. Momentum ties the reward
  to sustained combat instead — a Knight who keeps the pressure on now ends up
  slightly faster than the old buff ever allowed, but only after ramping up.*
  - `src/map/status.hpp` — added `SC_MOMENTUM` to the `sc_type` enum.
  - `src/map/script_constants.hpp` — exported `SC_MOMENTUM` as a script constant.
  - `src/map/status.cpp` — removed Two-Hand Quicken's ASPD sources
    (`status_change_start`'s `val2 = 300` fallthrough, the
    `status_calc_aspd_rate` block, and the renewal-only `status_calc_aspd`
    fixed-bonus term); added Momentum's `7% per stack` swing-delay reduction
    (capped at `35%`) to the `status_calc_aspd_rate` max group.
  - `src/map/skill.cpp` — `skill_additional_effect` grants a Momentum stack on
    any landed auto-attack while Two-Hand Quicken is active;
    `skill_counter_additional_effect` grants a stack when receiving a melee
    (`BF_SHORT`) auto-attack while Two-Hand Quicken is active.
  - `db/pre-re/status.yml` — dropped `CalcFlags` from `Twohandquicken`; added
    the `Momentum` status entry (`EFST_STRIKING` borrowed icon so it occupies
    its own status slot, `SendVal1` shows the stack count).
- **Two-Hand Quicken → Battle Rhythm (follow-up):** Renamed to **Battle
  Rhythm** (display name only — the Aegis name `KN_TWOHANDQUICKEN` is
  unchanged). All in-client references to the old name were updated to
  match, including its own tooltip and status icon, and the skills that
  name it in their own Requirement lines or prose (Bowling Bash, Parry,
  Gypsy's Please Don't Forget Me, Wizard's Watery Evasion).
  *Developer Comments: "Two-Hand Quicken" was a holdover name from the old
  flat-ASPD buff; now that it drives the Momentum stacking mechanic, "Battle
  Rhythm" better fits a buff that builds up through sustained combat rather
  than firing once on cast.*
  - `lua files/skillinfoz/skillinfolist.lub` — `KN_TWOHANDQUICKEN`'s
    `SkillName` changed to "Battle Rhythm".
  - `lua files/skillinfoz/skilldescript.lub` — `KN_TWOHANDQUICKEN`'s title
    changed to "Battle Rhythm"; `KN_BOWLINGBASH`'s Requirement line and
    Momentum cross-reference, and `LK_PARRYING`'s Requirement line, updated
    to say "Battle Rhythm"; the Gypsy and Wizard entries that mention it by
    name in their Comments prose updated to match.
  - `lua files/stateicon/stateiconinfo.lub` — the `EFST_TWOHANDQUICKEN`
    status icon's title changed to "Battle Rhythm".

- **Bowling Bash:** The gutter-line bug has been removed entirely (the chain
  reaction now uses a 5×5 area around the caster, independent of where on the
  map you stand) and the skill now hits twice per cast, with each hit dealing up
  to 400% damage at level 10 (800% total). *Developer Comments: The official
  workaround for gutter-lines was simply to inflate damage and tell players to
  edit their GRFs or memorize map cells. That is not a fix. We've set the
  per-hit damage a little lower than the inflated 800% the official game ended
  up with, but the skill now hits twice and behaves the same way on every map,
  every cell.*

- **Auto Counter:** Reworked from a reactive, facing-gated one-shot into a
  proactive combat stance. The cast is **instant** and applies a **15 second**
  self-buff (new `SC_KNIGHTCOUNTER` status, its own icon) with a **60 second**
  cooldown from cast. The Knight is rooted in place for the duration (cannot
  move) but can still act/attack freely. While the buff is active, any
  physical auto-attack landed on the Knight — **melee or ranged** — has a
  flat **50% chance** to trigger a return
  swing, a plain normal attack (defense applies, no forced crit) dealing
  **200% ATK** at every skill level; the incoming hit still lands, it is not
  blocked. Because a proper phantom-attacker unit isn't feasible in this
  engine, the counter simply plays as the Knight swinging back, which reads
  correctly for both melee and ranged attackers. *Developer Comments: the old
  Auto Counter asked you to stand still, face your target, and hope its next
  hit was melee — in
  practice nobody used it. Turning it into an always-on stance with a coin-flip
  retaliation makes it a real defensive tool a Knight can lean on mid-fight
  instead of a gimmick you set up in advance. One accepted side effect: Counter
  Instinct's (`ST_REJECTSWORD`) own counterstrike still calls into
  `KN_AUTOCOUNTER` for its damage, so it now also hits as a plain 200% blow
  instead of a defense-ignoring guaranteed crit — giving it its own dedicated
  strike skill was judged out of scope for this pass.*
  - `src/map/status.hpp` — added `SC_KNIGHTCOUNTER` to the `sc_type` enum.
  - `src/map/script_constants.hpp` — exported `SC_KNIGHTCOUNTER` as a script
    constant.
  - `db/import/status.yml` — added the `Knightcounter` status entry (reuses the
    classic `EFST_AUTOCOUNTER` icon, duration looked up from `KN_AUTOCOUNTER`,
    `States: NoMove` but no `NoAttack`/`NoInteract`).
  - `db/import/skill_db.yml` — `KN_AUTOCOUNTER` now applies `Knightcounter` on
    cast, with `CastTime: 0` (instant), `Duration1: 15000` and
    `Cooldown: 60000`; `DamageFlags` no longer force
    `IgnoreDefense`/`Critical`.
  - `src/map/skills/swordman/counterattack.cpp` — `castendNoDamageId` now just
    starts the self-buff (no more immediate scheduled hit); added
    `calculateSkillRatio` for the flat `200%` ratio; removed the old
    `modifyHitRate` `+20%` hit bonus.
  - `src/map/battle.cpp` — `is_attack_critical` no longer force-crits
    `KN_AUTOCOUNTER`'s counterstrike (Counter Instinct's `SC_AUTOCOUNTER`
    guaranteed-crit path is unchanged); `battle_weapon_attack` gained a new
    non-blocking `SC_KNIGHTCOUNTER` check (50% chance, deferred 100ms via
    `skill_addtimerskill`) alongside the existing `SC_AUTOCOUNTER` block; plays
    the classic `EF_AUTOCOUNTER` effect (the same audio/visual cue the old
    reactive counter used) on the Knight whenever the retaliation triggers.
  - `src/map/skill.cpp` — removed the now-dead `KN_AUTOCOUNTER` case in
    `skill_timerskill` (a no-damage stub left over from the old cast path) so
    the deferred counterstrike falls through to the normal `skill_attack` path
    and actually deals damage.
- **Auto Counter (follow-up):** The counterstrike no longer carries a flat
  `200% ATK` bonus — it now hits **twice** at normal ATK per hit, and each of
  the two hits can land a critical strike at **double** the Knight's normal
  `CRIT` rate. *Developer Comments: a flat damage multiplier felt like a
  bigger single number but played the same as a normal hit every time. Two
  rolls of the dice, each with a real shot at a crit, makes the retaliation
  feel like a distinct payoff instead of a bigger version of a plain attack,
  and it scales naturally with any crit-rate investment the Knight already
  made.*
  - `db/import/skill_db.yml` — `KN_AUTOCOUNTER` now sets `Hit: Multi_Hit` and
    `HitCount: 2` (drives `div_` via `skill_get_num`), and `DamageFlags.Critical`
    is `true` (allows the skill to roll crits at all; defense still applies —
    `IgnoreDefense` stays `false`).
  - `src/map/skills/swordman/counterattack.cpp` — removed the `calculateSkillRatio`
    override that added the flat `+100` ratio; the counterstrike now deals
    plain `100%` ATK per hit like a normal attack. `modifyDamageData` also
    tags the damage as `DMG_MULTI_HIT` — since the counterstrike carries an
    explicit `skill_id`, the engine's usual auto-tagging (which only runs for
    plain `skill_id == 0` attacks) never applies, and without this the client
    rendered the doubled total as one bigger hit instead of two separate hits.
  - `src/map/battle.cpp` — `is_attack_critical` gained a `case KN_AUTOCOUNTER:`
    that doubles the effective `cri` stat used for the roll, on top of the
    normal crit-rate formula (STR/LUK/equipment bonuses all still apply before
    the doubling).
- **Auto Counter (follow-up):** Fixed a lingering client-side cast bar shown
  for the full duration of casting the skill, even though the cast is
  instant. *Developer Comments: the client always shows its default cast-bar
  UI for any skill-use packet unless explicitly told to dismiss it. The old
  reactive version cleared it the moment a counter fired
  (`clif_skillcastcancel`, "Remove the casting bar." - Skotlex); since the
  old buff only lasted 400-2000ms, that reactive clear (or the SC's own
  early expiry) usually hid the bar before it was noticeable. This stance
  keeps the buff up for 15 seconds and has no equivalent reactive moment, so
  the bar was left to play out its full default length every time, unmasked.*
  - `src/map/skills/swordman/counterattack.cpp` — `castendNoDamageId` now
    calls `clif_skillcastcancel` right after applying `SC_KNIGHTCOUNTER`, so
    the cast bar is dismissed immediately once the buff lands instead of
    lingering.
- **Auto Counter (follow-up):** The buff's status icon now actually appears
  in the status bar. *Developer Comments: the classic `EFST_AUTOCOUNTER` icon
  has no entry in this client's `stateiconinfo.lub` table, so it silently
  never rendered no matter what the server sent. Swapped to Rune Knight's
  Crush Strike icon (`EFST_CRUSHSTRIKE`) instead - `RK_CRUSHSTRIKE` is
  unreachable on this pre-re server (same trick already used for Two-Hand
  Quicken/Momentum borrowing Enchant Blade/Striking), and its icon is
  confirmed to render, with its tooltip repurposed to describe Auto Counter.*
  - `db/import/status.yml` — `Knightcounter`'s `Icon` changed from
    `EFST_AUTOCOUNTER` to `EFST_CRUSHSTRIKE`.
  - `lua files/stateicon/stateiconinfo.lub` — repurposed the
    `EFST_CRUSHSTRIKE` `StateIconList` entry's title/description to describe
    Auto Counter instead of Crush Strike.
- **Auto Counter → Retaliation (follow-up):** Renamed to **Retaliation**
  (display name only - Aegis name `KN_AUTOCOUNTER` is unchanged). The
  retaliation trigger chance is now `10% * skill level` (10% at Lv1 up to
  50% at Lv5) instead of a flat 50%, and SP cost is now a flat `10 SP` at
  every level (was `3 SP`). Casting now also plays a one-shot activation
  flash (`EF_GUMGANG8`) on the Knight, and shows the same persistent "bound"
  cage effect used by rooted targets (Snaring Arrow's `SC_ANKLE`, Sohee's
  `NPC_STOP`) for the full 15s duration, making the immobility visually
  obvious. *Developer Comments: "Auto Counter" was a holdover name from the
  old reactive mechanic and no longer described a stance the player
  proactively activates - "Retaliation" fits the new active-buff identity.
  Scaling the trigger chance with skill level gives investing in higher
  ranks a clear payoff beyond just the flat ATK/crit numbers, and the SP
  bump to 10 brings the cost in line with a 15-second combat stance rather
  than the old sub-2-second reactive window. The activation flash and root
  visual close the loop on "how do I know this is active and why can't I
  move" raised during testing.*
  - `db/import/skill_db.yml` — `Requires.SpCost: 10`; `Description:
    Retaliation`.
  - `src/map/battle.cpp` — the `SC_KNIGHTCOUNTER` retaliation check in
    `battle_weapon_attack` now rolls `rnd()%100 < 10 * skill_lv` instead of
    a flat `< 50`.
  - `src/map/skills/swordman/counterattack.cpp` — `castendNoDamageId` plays
    `EF_GUMGANG8` once on cast, and (when the buff successfully starts)
    `EF_NPC_STOP` to begin the persistent root-cage visual.
  - `src/map/status.cpp` — `status_change_end` gained a `case
    SC_KNIGHTCOUNTER:` that calls `clif_specialeffect_remove` on
    `EF_NPC_STOP`, tearing the cage down when the stance ends (mirrors the
    existing `SC_ANKLE` case).
  - `lua files/skillinfoz/skillinfolist.lub` — `SkillName` changed to
    "Retaliation"; `SpAmount` changed to `{ 10, 10, 10, 10, 10 }`.
  - `lua files/skillinfoz/skilldescript.lub` — title changed to
    "Retaliation"; body rewritten for the `10% per skill level` trigger
    chance and `10 SP` cost.
  - `lua files/stateicon/stateiconinfo.lub` — the repurposed
    `EFST_CRUSHSTRIKE` entry's title changed to "Retaliation".
- **Retaliation (follow-up):** Reverted the double-hit change - the
  counterstrike is back to a single hit at normal ATK, still able to
  critically hit at double the Knight's normal `CRIT` rate. The stance no
  longer plays a special-effect flash each time the counter actually fires.
  *Developer Comments: two hits and a crit shot each added visual noise on
  top of what is already a frequent proc at higher levels (up to 50% per hit
  taken); dropping back to one hit and cutting the per-proc flash keeps the
  crit-rate payoff from the previous pass without turning every incoming hit
  into a fireworks show.*
  - `db/import/skill_db.yml` — removed the `Hit: Multi_Hit` / `HitCount: 2`
    override, so `KN_AUTOCOUNTER` falls back to the base `Hit: Single` /
    `HitCount: 1`.
  - `src/map/skills/swordman/counterattack.cpp` — `modifyDamageData` no
    longer tags the damage `DMG_MULTI_HIT` (only needed for the two-hit
    display).
  - `src/map/battle.cpp` — removed the `clif_specialeffect(target,
    EF_AUTOCOUNTER, AREA)` call in the `SC_KNIGHTCOUNTER` retaliation check,
    so a successful counter no longer flashes/dings on every proc.
  - `lua files/skillinfoz/skilldescript.lub` — body no longer says
    "counterattacking twice"; describes a single normal attack that can crit.
- **Retaliation (follow-up):** Now additionally requires **Two-Hand Quicken
  Rank 5** to learn, on top of the existing Two-Handed Sword Mastery
  (`SM_TWOHAND`) Rank 1 requirement. *Developer Comments: Retaliation is a
  standalone combat stance now, not a reactive one-shot, so it makes sense to
  gate it behind a Knight actually committing to the two-handed weapon
  identity Two-Hand Quicken represents, rather than the token single rank of
  Blade/Two-Handed Sword Mastery it inherited from the old Swordman tree.*
  - `db/import/skill_tree.yml` — `KN_AUTOCOUNTER` gained a `Requires:
    KN_TWOHANDQUICKEN Level: 5` entry under the Knight job tree (merges
    alongside the inherited `SM_TWOHAND` Level 1 requirement; propagates to
    Lord_Knight through inheritance).
- **Retaliation (follow-up):** Trigger chance is now `5%` per skill level
  (down from `10%`), the stance now lasts `10 seconds` (down from `15`), the
  cooldown is `30 seconds` (down from `60`), and the SP cost is now a flat
  `15 SP` (up from `10`). The proc roll also now happens before the
  triggering attack's own hit/miss is resolved, so a Knight can counter an
  attack that whiffs against them, not just one that lands.
  *Developer Comments: at up to 50% per swing the old proc rate was drowning
  out the Knight's own offense, and the long duration/cooldown pair made the
  stance feel more like a toggle than an active decision. A shorter window,
  a shorter cooldown, and a lower per-swing rate turn Retaliation into
  something you time around a fight rather than leave running constantly;
  rewarding a stand-and-defend Knight even when the enemy misses closes off
  a case where the stance previously gave nothing back.*
  - `db/import/skill_db.yml` — `KN_AUTOCOUNTER` `Duration1` changed to
    `10000`, `Cooldown` changed to `30000`, `Requires.SpCost` changed to `15`.
  - `src/map/battle.cpp` — the `SC_KNIGHTCOUNTER` proc chance in
    `battle_weapon_attack` changed from `10 * skill_lv` to `5 * skill_lv`;
    the roll already runs ahead of `battle_calc_attack`'s hit/miss
    resolution, so no code change was needed for it to fire on a miss -
    only the comment was updated to call this out explicitly.
  - `src/map/status.hpp` — updated the `SC_KNIGHTCOUNTER` comment to match
    the new duration, rate, and hit-or-miss trigger.
  - `lua files/skillinfoz/skilldescript.lub` — body updated to `10 seconds`,
    `5% chance per skill level`, `hit or miss`, `15 SP`, and `30 second
    cooldown`.
  - `lua files/skillinfoz/skillinfolist.lub` — `SpAmount` changed to
    `{ 15, 15, 15, 15, 15 }`.
- **Retaliation (follow-up):** Trigger chance raised to `8%` per skill level
  (up from `5%`, `8%` at Lv1 up to `40%` at Lv5), and the skill's position in
  the Knight skill tree has been moved down one row.
  *Developer Comments: 5% per level made the stance feel like it rarely paid
  off outside of a full 5-point investment; 8% keeps a meaningful floor at
  low levels while still topping out well short of a guaranteed proc.*
  - `src/map/battle.cpp` — the `SC_KNIGHTCOUNTER` proc chance in
    `battle_weapon_attack` changed from `5 * skill_lv` to `8 * skill_lv`.
  - `src/map/status.hpp` — updated the `SC_KNIGHTCOUNTER` comment to match
    the new `8%` rate.
  - `db/import/skill_db.yml` — updated the rebalance comment to match the
    new `8%` rate.
  - `lua files/skillinfoz/skilldescript.lub` — body updated to `8% chance
    per skill level`.
  - `lua files/skillinfoz/skilltreeview.lub` — `KN_AUTOCOUNTER` moved from
    grid index `8` to `15` in the `JT_KNIGHT` tree (one row down, same
    column).
- **Retaliation (follow-up):** Now requires **only Bowling Bash Rank 5** to
  learn - the Two-Hand Quicken Rank 5 and Two-Handed Sword Mastery
  (`SM_TWOHAND`) Rank 1 requirements have been removed. Bowling Bash, in
  turn, no longer requires Retaliation.
  *Developer Comments: an earlier pass gated Retaliation behind Bowling
  Bash Rank 5 without accounting for Bowling Bash's own base-game
  requirement of Retaliation Rank 5 - the two skills pointed at each other
  and became permanently unlearnable, which also blocked Sundering Strike
  (`KN_ONEHAND`) since it sits behind Bowling Bash Rank 10. Breaking the
  cycle by erasing Bowling Bash's Retaliation requirement, and simplifying
  Retaliation down to a single clean Bowling Bash Rank 5 gate, restores a
  learnable one-directional progression: Bowling Bash first, Retaliation
  (and Sundering Strike) after.*
  - `db/import/skill_tree.yml` — `KN_AUTOCOUNTER`'s `Requires` list is now
    `SM_TWOHAND Level: 0` (erases the inherited Rank 1 gate) plus
    `KN_BOWLINGBASH Level: 5`; the earlier `KN_TWOHANDQUICKEN Level: 5`
    entry was dropped. A new `KN_BOWLINGBASH` entry was added with
    `Requires: KN_AUTOCOUNTER Level: 0`, erasing Bowling Bash's inherited
    base-tree requirement of Retaliation Rank 5
    (`db/pre-re/skill_tree.yml`) so the two skills no longer gate each
    other.
  - `lua files/skillinfoz/skilldescript.lub` — Retaliation's Requirement
    line is now just "Bowling Bash 5"; Bowling Bash's Requirement line no
    longer lists "Auto Counter 5".
- **Peco Peco Ride:** Is now a properly castable skill instead of a passive
  prerequisite gate. Casting it takes a `2 second` fixed cast (unaffected by
  DEX, INT, cast-reducing items, or status effects), has a `5 second`
  cooldown, and costs no SP. Completing the cast toggles the caster between
  mounted and dismounted on a Peco Peco. *Developer Comments: Riding used to
  only be togglable through `@ride` or item scripts, with the skill itself
  doing nothing when used — a Knight clicking their own Peco Peco Ride icon
  got no feedback at all. Giving it a real cast makes the skill icon
  actually do what its tooltip always implied.*
  - `db/import/skill_db.yml` — `KN_RIDING` gained `TargetType: Self`,
    `DamageFlags.NoDamage: true`, `CastTime: 2000` with `CastTimeFlags`
    (`IgnoreDex`, `IgnoreStatus`, `IgnoreItemBonus`) so the cast time can't be
    reduced, `Cooldown: 5000`, and `Requires.SpCost: 0`.
  - `src/map/skill.cpp` — `skill_castend_nodamage_id` gained a `KN_RIDING`
    case that calls `pc_setriding` to toggle the mount state on cast
    completion.

- **One-Hand Quicken → Sundering Strike:** Repurposed from a no-damage,
  Soul-Link-gated ASPD buff into **Sundering Strike**, a 5-level offensive
  Two-Handed Sword strike gated behind Bowling Bash level 10 instead of
  Two-Hand Quicken level 10 (Aegis name `KN_ONEHAND` is unchanged). The blow
  deals `100% ATK` per skill level plus `100% ATK` per stack of Momentum (the
  stacking buff Two-Hand Quicken builds, see `SC_MOMENTUM`) consumed on hit,
  capped at `1000%` total. At 5 stacks the blow also strikes every enemy in a
  3x3 area around the target and has a `50%` chance to Stun each target hit
  for `2` seconds. Momentum stacks are consumed once, after all hits resolve,
  so a full-stack cast sees every splash target hit at the same scaled
  damage. The strike plays Crush Strike's (`RK_CRUSHSTRIKE`) swing animation
  — a damage-less visual-only packet, the same trick already used for Wyvern
  Bolt borrowing Crescive Bolt's projectile — but keeps its own icon and SP
  cost (`30` SP flat, `15` second cooldown). *Developer Comments: One-Hand
  Quicken was a dead pick — it required Soul Link, gave nothing a maxed
  Two-Hand Quicken didn't already provide, and dealt no damage. Turning it
  into a payoff strike for the Momentum stacks a Knight already builds by
  fighting gives sustained melee pressure an actual damage payoff, and the
  5-stack AoE/stun spike rewards staying on a target instead of hunting for a
  fresh audience each fight.*
  - `src/map/skills/swordman/sunderingstrike.cpp` /
    `src/map/skills/swordman/sunderingstrike.hpp` — new `SkillSunderingStrike`
    (`WeaponSkillImpl`). `calculateSkillRatio` reads `SC_MOMENTUM`'s `val1`
    (stack count) off the caster and adds `100%` per skill level plus `100%`
    per stack, capped at `1000%`. `castendDamageId` plays the
    `RK_CRUSHSTRIKE` visual via a `DMGVAL_IGNORE` `clif_skill_damage` call,
    then either a plain `skill_attack` (below 5 stacks) or a
    `map_foreachinrange`/`skill_area_sub` 3x3 splash (at 5 stacks), and
    consumes `SC_MOMENTUM` once at the end regardless of path.
    `applyAdditionalEffects` rolls the `50%`/`2s` Stun (`Duration2`, read via
    `skill_get_time2`) at 5 stacks.
  - `src/map/skills/swordman/skill_factory_swordman.cpp` — `KN_ONEHAND` now
    maps to `SkillSunderingStrike` instead of `StatusSkillImpl`.
  - `src/map/map-server.vcxproj` / `.vcxproj.filters` — added the new
    `sunderingstrike.cpp`/`.hpp` source pair alongside the other Swordman skill
    files.
  - `db/import/skill_db.yml` — `TargetType: Attack`, `MaxLevel: 5`,
    `Range: 2`, `Element: Weapon`, `Duration2: 2000` (stun length),
    `Cooldown: 15000`, `Requires.SpCost: 30`; `DamageFlags.NoDamage` and
    `Flags.IsSpirit` cleared so the skill deals damage and can be learned
    with skill points; `Requires.Weapon` switched from `1hSword` to
    `2hSword`. The old `Duration1`/`Status: OneHand` fields are left in the
    base entry inert — the code path that read them no longer runs now that
    the factory returns `SkillSunderingStrike`.
  - `db/import/skill_tree.yml` — Knight tree: `KN_ONEHAND` `MaxLevel` raised
    to `5`, its `KN_TWOHANDQUICKEN` prerequisite erased (`Level: 0`) and
    replaced with `KN_BOWLINGBASH` level `10`. Propagates to Lord Knight
    through inheritance.
  - `lua files/skillinfoz/skillinfolist.lub` — `SkillName` changed to
    "Sundering Strike", `MaxLv: 5`, `SpAmount` flattened to `30` at every level,
    `AttackRange: 2` at every level, `Type` changed from `"Soul"` to
    `"Offensive"`, and `_NeedSkillList` changed to Bowling Bash level 10.
  - `lua files/skillinfoz/skilldescript.lub` — tooltip rewritten: title
    "Sundering Strike", Bowling Bash 10 requirement, Active/Enemy-targeted, and
    a description covering the per-level/per-stack damage formula, the
    `1000%` cap, and the 5-stack AoE/Stun payoff.
  - `lua files/skillinfoz/skilltreeview.lub` — Knight tree grid slot moved
    from `[13]` to `[15]`, directly under Bowling Bash (`[8]`).
- **Sundering Strike (follow-up):** Now plays Sonic Wave's (`RK_SONICWAVE`) swing
  animation instead of Crush Strike's, the 5-stack payoff now strikes a
  3-cell-wide, 5-cell-long line straight in front of the caster instead of a
  3x3 box around the target, its Stun no longer silently fails to trigger,
  and it is now a non-targeted skill that auto-fires on the caster's current
  target instead of requiring a target click. *Developer Comments: the Stun
  bug was a Momentum timing race — `applyAdditionalEffects` only runs after
  `battle_delay_damage`'s `amotion`-delayed timer fires, but Momentum was
  already consumed by the time that timer went off, so the stack count it
  read back was always `0` and the `>= 5` check never passed. Rolling the
  Stun immediately when each hit lands, before Momentum is consumed, fixes
  that. A line read better than a box centered on the target - it's a
  frontal strike, so its area should follow where the Knight is facing, not
  where the target happens to be standing. Removing the target-click fits
  the "chain it off whatever you're already hitting" identity a Momentum
  payoff strike is going for.*
  - `src/map/skills/swordman/sunderingstrike.cpp` — `castendDamageId` now plays
    `RK_SONICWAVE` instead of `RK_CRUSHSTRIKE` in its visual-only
    `clif_skill_damage` call, and always hits the cast-at target directly
    first (it is no longer guaranteed to be lined up with the caster's
    facing, since the cast is no longer aimed by hand). The 5-stack payoff
    replaced its `map_foreachinrange`/`skill_area_sub` 3x3 splash with a
    stepped sweep along the caster's `unit_getdir`/`dirx`/`diry` facing
    direction (`CRUSHINGBLOW_LINE_LENGTH = 5` cells deep) and its 90-degree
    rotation (`(dir + 2) % 8`) for the 3-cell width, calling a new
    file-scope `skill_crushingblow_line_sub` callback per cell via
    `map_foreachincell`. The callback calls `battle_check_target` directly,
    then `skill_attack` and rolls the `50%`/`2s` Stun itself right after a
    successful hit - synchronously, while `SC_MOMENTUM` is still present
    (the cast-at target's own Stun roll is done inline in
    `castendDamageId`). The `applyAdditionalEffects` override (where the
    Stun roll used to live) was removed, along with the header declaration
    in `sunderingstrike.hpp`.
  - `db/import/skill_db.yml` — `TargetType` changed from `Attack` to `Self`
    and `Flags.NoTargetSelf: true` added, so `skill_get_casttype` resolves
    to `CAST_DAMAGE` and `unit_skilluse_id2` (`src/map/unit.cpp`)
    auto-redirects the cast onto the caster's current target
    (`ud->target`) - native engine behavior, no further C++ change needed.
  - `lua files/skillinfoz/skilldescript.lub` — `Target:` line changed to
    "Current Target"; description rewritten for the line (was "3x3 area");
    added the standard blue "Fires on your current target" note.
- **Sundering Strike (follow-up):** Damage now lands in 3 successive waves,
  roughly `125ms` apart, instead of a single hit - matching Sonic Wave's
  animation, which already plays as 3 quick pulses. *Developer Comments: the
  single lump-sum hit didn't read as 3 pulses landing, it read as 1 big hit
  with some extra flair playing after the fact. Splitting the damage across 3
  waves timed to the pulses makes the number actually track what's on
  screen.*
  - `src/map/skills/swordman/sunderingstrike.cpp` — new
    `skill_sunderingstrike_schedule_waves` helper fires an immediate
    `skill_attack` plus two more via `skill_addtimerskill` at
    `CRUSHINGBLOW_WAVE_INTERVAL` (`125ms`) and double that, reusing the
    engine's existing generic fallback in `skill_timerskill` (`skill.cpp`) to
    land the delayed hits - no core-engine change needed. `calculateSkillRatio`
    now divides the total ratio by `3` so the waves sum back to the original
    total. Because Momentum is still consumed immediately on cast (right
    after `castendDamageId` fires) but the 2nd/3rd waves land after that,
    `calculateSkillRatio` can no longer just re-read live `SC_MOMENTUM` - it
    would see `0` for the delayed waves, the same class of bug the Stun fix
    above ran into. Fixed with a new file-scope
    `sunderingstrike_stack_snapshot` map (keyed by caster id) that
    `castendDamageId` populates once at cast time and every wave's
    `calculateSkillRatio` call reads instead; safe to leave unentered since
    the skill's cooldown guarantees no re-cast can land before a 3-wave
    sequence (max `250ms`) finishes. The 5-stack line sweep's Stun is
    now rolled once per target (before scheduling its 3 waves) rather than
    per wave, so it stays a flat `50%` chance instead of drifting to
    `1 - 0.5^3 ≈ 87.5%`.
- **Sundering Strike (follow-up):** Its 5-stack payoff no longer sweeps a
  uniform 3-cell-wide, 5-cell-long line; it now sweeps a narrower cone that
  tapers in close to the caster and reaches out to 11 cells: 1 cell wide for
  the first 3 cells, widening to 3 cells for the next 4, then 5 cells for the
  final 4. *Developer Comments: a uniform-width line that's 3 cells wide
  right next to the caster read as unreasonably generous up close for a
  skill this cheap; tapering the width in tightens the "free" hits near the
  caster while still rewarding a Knight who lines up a long, clean swing at
  range.*
  - `src/map/skills/swordman/sunderingstrike.cpp` /
    `src/map/skills/swordman/sunderingstrike.hpp` — renamed from
    `crushingblow.cpp`/`.hpp`; `SkillCrushingBlow` renamed to
    `SkillSunderingStrike`, `skill_crushingblow_line_sub` renamed to
    `skill_sunderingstrike_cone_sub`, `skill_crushingblow_schedule_waves` to
    `skill_sunderingstrike_schedule_waves`, `crushingblow_stack_snapshot` to
    `sunderingstrike_stack_snapshot`, and `CRUSHINGBLOW_WAVE_INTERVAL` to
    `SUNDERINGSTRIKE_WAVE_INTERVAL`. `CRUSHINGBLOW_LINE_LENGTH` (`5`,
    constant width `-1..1`) replaced with `SUNDERINGSTRIKE_CONE_LENGTH`
    (`11`); the per-cell sweep loop now computes `half_width =
    min(step / 4, 2)` so the swept width grows from `1` to `3` to `5` cells
    as `step` increases, instead of staying fixed at `3`.
  - `src/map/skills/swordman/skill_factory_swordman.cpp` — `KN_ONEHAND` now
    maps to `SkillSunderingStrike`; `#include` moved from the `crushingblow`
    slot to the alphabetically correct `sunderingstrike` slot.
  - `src/map/map-server.vcxproj` / `.vcxproj.filters` — `crushingblow.cpp`/
    `.hpp` entries removed, `sunderingstrike.cpp`/`.hpp` added in their
    alphabetically correct position.
  - `db/import/skill_db.yml` — `Description` changed to "Sundering Strike".
  - `lua files/skillinfoz/skillinfolist.lub` — `SkillName` changed to
    "Sundering Strike".
  - `lua files/skillinfoz/skilldescript.lub` — title changed to "Sundering
    Strike"; description updated from "every enemy in a line" to "every
    enemy in a narrow cone up to 11 cells".
- **Sundering Strike (follow-up):** The Sonic Wave visual now travels as far
  on screen as the hit actually reaches, instead of always stopping at the
  cast-at target's position. *Developer Comments: `clif_skill_damage`'s
  destination parameter is a unit id, not a coordinate - the client animates
  the wave traveling to wherever that specific unit is standing, not out to
  some fixed distance. Sundering Strike is cast at melee range (it's a
  non-targeted skill firing on the current target, which is always within
  `Range: 2`), so anchoring the visual there made an 11-cell cone hit look
  like it stopped dead after two cells the moment any of the farther targets
  actually got hit.*
  - `src/map/skills/swordman/sunderingstrike.cpp` — factored the cone's
    step/width loop out into a new template helper,
    `skill_sunderingstrike_walk_cone`, shared by two passes. Before playing
    the visual, a new `skill_sunderingstrike_find_farthest_sub` callback
    walks the cone read-only (just `battle_check_target`, no `skill_attack`)
    and records the last (i.e. deepest) enemy it finds into a local
    `visual_target`, which the existing damage sweep pass then reuses
    unchanged. `clif_skill_damage` now targets `visual_target` instead of
    always targeting the cast-at `target`; it still falls back to `target`
    when the cone is empty (below 5 stacks, or nothing else caught in it).
- **Sundering Strike (follow-up):** Cooldown raised from `15` seconds to
  `45` seconds.
  - `db/import/skill_db.yml` — `Cooldown: 45000`.
  - `lua files/skillinfoz/skilldescript.lub` — description updated from "15
    second cooldown" to "45 second cooldown".
  - `src/map/skills/swordman/sunderingstrike.cpp` — updated the
    `sunderingstrike_stack_snapshot` comment's cooldown reference for
    accuracy (the snapshot-safety argument itself is unaffected — it holds
    at any cooldown longer than the ~250ms 3-wave sequence).

- **Bowling Bash (rework):** Turned from a spammable double-hit into a
  committed burst strike. It now fires on your current target with no cursor
  selection (`TargetType: Self` + `NoTargetSelf`, the same non-targeted "combo
  skill" behavior Sundering Strike uses), lands as a single hit instead of two
  (`div_` 2 → 1), and carries a `5` second cooldown. Every splash and chain hit
  can now roll a critical (`NK_CRITICAL`). Base damage is unchanged at
  `100% + 40% per level` (500% at level 10), but it now also consumes up to 5
  `Momentum` stacks — the ones Two-Hand Quicken builds — for `+60% ATK per
  stack`, up to `+300%` (800% total at level 10 with a full stack). Splash,
  knockback pinball, range and SP cost are untouched.
  - `db/import/skill_db.yml` — `TargetType: Self`, `Flags.NoTargetSelf: true`,
    `DamageFlags.Critical: true`, `Cooldown: 5000`.
  - `src/map/skills/swordman/bowlingbash.cpp` — `modifyDamageData` now sets
    `div_ = 1`; `calculateSkillRatio` adds `60 * min(5, Momentum stacks)`
    (read live, `#ifndef RENEWAL`); `castendDamageId` consumes `SC_MOMENTUM`
    once at the end of the initial cast (flag depth 0), after the whole
    synchronous splash/chain cascade has already scored off the live stacks.
  - `lua files/skillinfoz/skilldescript.lub` — rewritten to the current-target,
    single-hit, crit, Momentum and 5s-cooldown behavior; the stale Renewal
    "up to four hits" multi-hit text and per-level ATK list were removed in
    favor of the scaling formula. *Developer Comments: Bowling Bash and
    Sundering Strike had drifted into the same "2H frontal AoE nuke" niche.
    Slowing Bowling Bash down (5s cooldown, one hit) and paying it back through
    crits and a Momentum dump gives it a distinct high-commitment feel while
    keeping its signature pinball. Momentum is now a shared resource both
    Knight burst skills can spend — pick your dump.*

- **Bowling Bash (damage tuning):** Base damage lowered from `100% + 40% per
  level` to `100% + 30% per level` (400% at level 10, down from 500%). The
  Momentum consumption mechanic (up to `+60% ATK` per stack, `+300%` at 5
  stacks) is unchanged.
  - `src/map/skills/swordman/bowlingbash.cpp` — `calculateSkillRatio` now adds
    `30 * skill_lv` instead of `40 * skill_lv`; the Momentum bonus block below
    it is untouched.
  - `lua files/skillinfoz/skilldescript.lub` — updated "40% per skill level"
    to "30% per skill level". *Developer Comments: 800% total damage at level
    10 with a full Momentum stack was landing too hard relative to Sundering
    Strike's damage ceiling. Trimming the base ratio pulls the non-Momentum
    floor down to a more reasonable 400% while leaving the Momentum payoff
    (the skill's signature "spend your stacks" moment) exactly as strong as
    before.*

- **Sundering Strike (rework):** Given its own identity as an armor-breaking
  setup strike instead of a second AoE nuke. It now plays Cart Termination's
  animation (`WS_CARTTERMINATION`) instead of Sonic Wave's, and lands as a
  single hit instead of 3 delayed waves (see the follow-up entry below for
  the animation fix that made this land correctly as a weapon swing).
  Damage is
  `200% + 100% per skill level` (700% at level 5), consuming up to 5
  `Momentum` stacks for `+50% ATK per stack` (up to +250%, 950% total at
  level 5 with a full stack — down from the old +100%/stack). Cooldown is
  lowered from `45` to `30` seconds. On every cast it also crushes the
  target's armor: a new `Sunder` debuff reduces `DEF` by `5% per skill level`
  (25% at level 5) for `12` seconds, no PC exemption (applies in PvP/WoE too).
  At 5 stacks, the existing 11-cell frontal cone also sunders (not just
  stuns) every enemy it catches.
  - `src/map/status.hpp` — new `SC_SUNDER` enum entry (`val1` = DEF reduction
    %).
  - `src/map/script_constants.hpp` — `export_constant(SC_SUNDER)`; required
    for the status DB's YAML loader to resolve `Status: Sunder` to the new
    enum value (it looks up `SC_<NAME>` via `script_get_constant`, which only
    finds constants registered here, not just declared in `status.hpp`).
  - `src/map/status.cpp` — `status_calc_def` subtracts
    `def * SC_SUNDER->val1 / 100`.
  - `db/pre-re/status.yml` — new `Sunder` status, `CalcFlags.Def: true`,
    borrowed `EFST_BROKENARMOR` icon.
  - `db/import/skill_db.yml` — `Cooldown: 30000`; `Duration1` repurposed from
    the dead old buff duration into the `Sunder` debuff length (`12000`,
    read via `skill_get_time`).
  - `src/map/skills/swordman/sunderingstrike.cpp` — rewritten for a single
    synchronous hit: removed the `sunderingstrike_stack_snapshot` map, the
    125ms wave interval, and the `skill_addtimerskill` scheduling now that
    there's nothing delayed left to desync from live `SC_MOMENTUM`;
    `calculateSkillRatio` reads Momentum live; `castendDamageId` applies
    `SC_SUNDER` to the cast-at target every cast and to every cone target at
    5 stacks, then plays the Cart Termination visual and consumes Momentum
    once at the end.
  - `lua files/skillinfoz/skilldescript.lub` — rewritten for the new damage
    formula, armor-crush debuff, and 30s cooldown; dropped the redundant
    "fires on your current target" note now that the `Target: Current
    Target` line already states it (per the tooltip format spec). *Developer
    Comments: with Bowling Bash now doing the "spend Momentum for a bigger
    number" job, Sundering Strike needed a reason to exist beyond a worse
    version of the same nuke. Making it a DEF-shred plants "Sundering" in
    the name for real and turns it into a setup tool the whole party
    benefits from, not just a personal damage line.*

- **Sundering Strike (follow-up):** Fixed the raise-hand/cast pose players
  kept seeing instead of a weapon swing, after three borrowed visuals
  (`RK_SONICWAVE`, `WS_CARTTERMINATION`, `LK_HEADCRUSH`) were all tried and
  none fixed it — because the borrowed visual was never the cause. The real
  damage hit (`skill_attack(..., KN_ONEHAND, ...)`) sends its *own*, separate
  `clif_skill_damage` packet using `KN_ONEHAND`'s real id (495), and the
  client's per-skill-id animation table still had 495 mapped to its old
  "One-Hand Quicken" self-buff cast pose (One-Hand Quicken was never a
  damage skill in retail, so it never needed a combat animation) — entirely
  independent of whatever plays in the borrowed visual packet beforehand.
  This is exactly why Bowling Bash never had this problem: its own id has
  always been a genuine attack skill with a real swing baked in. The fix
  adds `KN_ONEHAND` to `skill.cpp`'s "Display damage" switch, in the same
  `clif_damage` (plain weapon-swing, no per-id animation lookup) branch
  `KN_AUTOCOUNTER` already uses for the identical reason. The visual is back
  on Cart Termination now that the real hit displays correctly regardless of
  which flourish plays first.
  - `src/map/skill.cpp` — added `case KN_ONEHAND:` to the `clif_damage`
    whitelist alongside `KN_AUTOCOUNTER`, `TF_DOUBLE`, `GS_CHAINACTION`, etc.
  - `src/map/skills/swordman/sunderingstrike.cpp` — visual reverted to
    `WS_CARTTERMINATION`.
  *Developer Comments: three borrowed-visual swaps in a row failing for
  different in-client symptoms should have been the signal that the visual
  packet wasn't the variable that mattered. The actual damage packet was
  the one carrying the stale animation the whole time. Lesson for any future
  skill built by repurposing an old buff/no-damage id: check this switch in
  skill.cpp before touching the visual at all.*

- **Sundering Strike (follow-up):** The floating skill-name text above the
  caster read "Cart Termination !!" instead of "Sundering Strike !!" after
  the animation fix above, since the borrowed visual packet's id was the
  only `clif_skill_damage`-family packet left carrying a name once the real
  hit moved to the nameless `clif_damage` path. Fixed by announcing the real
  skill name explicitly: `clif_skill_nodamage(src, *target, KN_ONEHAND,
  skill_lv)` is now called (using the genuine id) immediately before the
  Cart Termination visual packet, following the same pairing
  `abyssflame.cpp` already uses elsewhere in this codebase.
  - `src/map/skills/swordman/sunderingstrike.cpp` — added the
    `clif_skill_nodamage` call in `castendDamageId`.

- **Sundering Strike (rebalance):** Simplified back down to a pure
  single-target strike. The 5-stack frontal cone AoE and its 50%/2s Stun are
  both removed entirely (no more `stacks >= 5` payoff of any kind — the
  Momentum-fed damage bonus still scales smoothly from 0 to 5 stacks, just
  with no special threshold behavior). The armor-crush debuff is stronger to
  compensate for losing its AoE spread: `DEF` reduction raised from `5%` to
  `10% per skill level` (`50%` at level 5, up from `25%`), still main-target
  only. SP cost is now a flat `22` at every level (was a flat `30`).
  Cooldown is cut from `30` to `5` seconds, and it now shares that cooldown
  with Bowling Bash — casting either one puts both on cooldown, so they can
  no longer be alternated to bypass each other's downtime.
  - `db/import/skill_db.yml` — `Cooldown: 5000`, `SpCost: 22`; `Duration2`
    (the now-unused stun length) removed from the override.
  - `src/map/skill.cpp` — added a Bowling Bash / Sundering Strike mutual
    cooldown lockout in the same `BL_PC` cooldown-application block that
    already pairs `HT_BLITZBEAT`/`HT_PHANTASMIC`.
  - `src/map/status.hpp` — `SC_SUNDER` comment updated to `10 * skill level`
    and main-target-only (the 5-stack cone application is gone).
  - `src/map/skills/swordman/sunderingstrike.cpp` — removed the cone-walk
    helpers (`skill_sunderingstrike_walk_cone`,
    `skill_sunderingstrike_find_farthest_sub`,
    `skill_sunderingstrike_cone_sub`), the `SUNDERINGSTRIKE_CONE_LENGTH`
    constant, and the entire `stacks >= 5` branch (stun rolls, cone sweep,
    visual re-anchoring); `SC_SUNDER`'s `val1` changed from `5 * skill_lv` to
    `10 * skill_lv`; dropped the now-unused `map/battle.hpp`, `map/map.hpp`,
    and `map/unit.hpp` includes.
  - `lua files/skillinfoz/skilldescript.lub` — removed the "At 5 stacks..."
    cone/stun paragraph, updated the `DEF` formula and SP/cooldown numbers,
    and added a `Comments:` line noting the shared cooldown with Bowling
    Bash (mirrored onto Bowling Bash's own entry too, for the same reason).
  - `lua files/skillinfoz/skillinfolist.lub` — `SpAmount` changed from
    `{ 30, 30, 30, 30, 30 }` to `{ 22, 22, 22, 22, 22 }`.

- **Sundering Strike (follow-up):** Fixed the armor-crush debuff's
  `EFST_BROKENARMOR` icon not appearing on monster targets (the DEF
  reduction itself was always applying correctly — only the icon was
  missing). `SC_SUNDER` borrows its icon from the vanilla `SC_BROKENARMOR`
  status, and the engine code that decides which unit types (`BL_PC`,
  `BL_MOB`, etc.) are allowed to show a given icon was aggregating that
  shared icon slot in hash-table iteration order rather than a safe union:
  whichever of the two statuses happened to be processed last could
  overwrite the other's bits, so depending on load order the `BL_MOB` bit
  `SC_SUNDER` needs would sometimes get silently dropped.
  - `src/map/status.cpp` — in `StatusDatabase::loadingFinished()`, the
    non-`BlEffect` branch now OR's `BL_PC` into `StatusRelevantBLTypes`
    instead of overwriting it, so it can no longer clobber `BL_SCEFFECT`
    bits a colliding icon's `BlEffect` status already set. This is a
    general engine fix, not specific to Sundering Strike - it protects
    every icon-borrowing status in the database from the same race.
  *Developer Comments: the AoE/stun payoff never got much play time before
  being cut, so this isn't walking back something proven - it's trading
  breadth for a much tighter, faster, harder-hitting single-target tool that
  can't just be chained back-to-back with Bowling Bash for permanent uptime
  on one or the other.*

- **Two-Hand Quicken (Momentum tuning):** `Momentum`'s per-stack duration is
  cut from `15` to `7` seconds; stacks still cap at 5, refresh on any new
  stack, and reduce weapon swing delay by `7%` per stack (`35%` at 5 stacks).
  *Developer Comments: 15 seconds let a Knight coast on a maxed stack through
  long lulls between hits; 7 seconds keeps the ASPD payoff tied to actually
  staying in a fight rather than a hit landed a while back.*
  - `src/map/skill.cpp` — both `sc_start(..., SC_MOMENTUM, ...)` calls (landing
    an auto-attack in `skill_additional_effect`, taking a melee hit in
    `skill_counter_additional_effect`) changed from `15000` to `7000`.
  - `src/map/status.hpp` — `SC_MOMENTUM` comment updated to `7s duration`.
  - `lua files/skillinfoz/skilldescript.lub` — `KN_TWOHANDQUICKEN`'s Momentum
    line updated from `15 sec` to `7 sec`. No SP/range/max-level change, so
    `skillinfolist.lub` is untouched.

- **Two-Hand Quicken (Momentum retuning):** `Momentum`'s duration is raised
  from `7` to `10` seconds and its stack cap is doubled from `5` to `10`. The
  per-stack ASPD payoff is halved to compensate, dropping from `7%` to `3%`
  per stack, so the cap still tops out close to where it was (`30%` at 10
  stacks, versus `35%` at 5). Bowling Bash and Sundering Strike's
  Momentum-fed damage bonuses are halved in the same pass — `60%` to `30%`
  per stack on Bowling Bash, `50%` to `25%` per stack on Sundering Strike —
  so their maximum damage at full stacks (`+300%` and `+250%` respectively)
  is unchanged, only smoothed across twice as many stacks. *Developer
  Comments: the 5-stack cap made Momentum swingy - a Knight either had it
  maxed or didn't, with no meaningful in-between. Spreading the same total
  payoff across 10 stacks over a longer window makes partial stacks matter
  and rewards sustained engagement instead of one lucky hit topping the bar.*
  - `src/map/skill.cpp` — both `sc_start(..., SC_MOMENTUM, ...)` calls changed
    from `min(5, stacks + 1), 7000` to `min(10, stacks + 1), 10000`.
  - `src/map/status.cpp` — `status_calc_aspd_rate`'s Momentum block changed
    from `min(350, 70 * stacks)` to `min(300, 30 * stacks)`.
  - `src/map/status.hpp` — `SC_MOMENTUM` comment updated to `max 10` stacks,
    `3%` per stack (`30%` cap), `10s duration`.
  - `src/map/skills/swordman/bowlingbash.cpp` — `calculateSkillRatio` changed
    from `60 * min(5, stacks)` to `30 * min(10, stacks)`.
  - `src/map/skills/swordman/sunderingstrike.cpp` — both `calculateSkillRatio`
    and `castendDamageId` changed their stack clamp from `min(5, ...)` to
    `min(10, ...)`; the damage term changed from `50 * stacks` to
    `25 * stacks`.
  - `lua files/skillinfoz/skilldescript.lub` — `KN_TWOHANDQUICKEN`'s Momentum
    lines updated to `max 10, 10 sec` and `3%, up to 30%`; `KN_BOWLINGBASH`
    updated to `up to 10 Momentum stacks` / `30% ATK each`; `KN_ONEHAND`
    (Sundering Strike) updated to `10 stacks of Momentum` / `extra 25% ATK
    each`. No SP/range/max-level change on any of the three, so
    `skillinfolist.lub` is untouched.

- **Sundering Strike (Momentum cap trim):** The stack clamp is lowered from
  `10` to `5`, and the per-stack bonus is doubled from `25% ATK` to `50% ATK`,
  so the maximum payout is unchanged at `+250% ATK` from a full Momentum bar.
  *Developer Comments: Sundering Strike's own cap didn't need to track Two-Hand
  Quicken's 10-stack ceiling — this just compresses its consumption curve so
  the skill pays out its full bonus off a half-full Momentum bar instead of
  requiring the Knight to sit on stacks all the way to 10 before dumping them.*
  - `src/map/skills/swordman/sunderingstrike.cpp` — both `calculateSkillRatio`
    and `castendDamageId` changed their stack clamp from `min(10, ...)` to
    `min(5, ...)`; the damage term changed from `25 * stacks` to `50 *
    stacks`. Since Momentum can hold up to 10 stacks (built by Two-Hand
    Quicken) but Sundering Strike now only spends 5, `castendDamageId` no
    longer unconditionally ends the whole buff — it ends it and then
    re-applies `SC_MOMENTUM` with whatever stacks were left over
    (`total_stacks - 5`) so a fuller bar isn't discarded on cast.
  - `lua files/skillinfoz/skilldescript.lub` — `KN_ONEHAND` updated to `up to
    5 stacks of Momentum` / `extra 50% ATK each`. No SP/range/max-level
    change, so `skillinfolist.lub` is untouched.

- **Two-Hand Quicken (Momentum icon tooltip fix):** The borrowed Striking status
  icon's hover text still read `Reduces weapon swing delay by 7% per stack`, the
  pre-retuning value — the Momentum retuning pass that halved the per-stack ASPD
  from `7%` to `3%` updated `skilldescript.lub` but missed this one. Corrected to
  `3% per stack` so the icon tooltip matches the actual `30%`-at-10-stacks cap.
  Purely a text fix; no gameplay numbers change.
  - `lua files/stateicon/stateiconinfo.lub` — `EFST_STRIKING`'s descript line
    changed from `7%` to `3%` per stack.

- **Two-Hand Quicken (Momentum max-stacks aura):** When Momentum reaches its
  `10`-stack cap, a spinning brown aura (Chaos Panic's `EF_BOTTOM_ANI`, effect
  id `827`) now plays under the Knight's feet so it is immediately clear the bar
  is maxed. The aura is torn down the instant the stack count drops below `10` —
  whether the buff expires, is consumed by Bowling Bash or Sundering Strike, or
  is otherwise removed — so it is only ever visible at a full `10`-stack bar.
  Purely visual; no gameplay numbers change. *Developer Comments: the Striking
  status icon shows the running stack count, but nothing on the character model
  told a Knight (or an onlooker) they'd actually hit the cap and stopped gaining
  ASPD. A max-only aura makes the "spend it now" moment read at a glance.*
  - `src/map/skill.cpp` — both Momentum-building sites (`skill_additional_effect`
    on a landed auto-attack, `skill_counter_additional_effect` on taking a melee
    hit) fire `clif_specialeffect(..., EF_BOTTOM_ANI, AREA)` on the `9`→`10`
    transition (`stacks == 9`), so the looping aura starts exactly once when the
    bar tops out and is not re-sent while sitting at cap.
  - `src/map/status.cpp` — `status_change_end` removes the aura via
    `clif_specialeffect_remove(..., EF_BOTTOM_ANI, AREA, bl)` for `SC_MOMENTUM`,
    the single choke point every stack-loss path (expiry, Bowling Bash/Sundering
    Strike consumption) already runs through. Harmless no-op if the player never
    reached `10` stacks. No client tooltip change, so the `lua files` are
    untouched.

- **Charge Attack:** No longer a quest skill — it is now a 5-level
  skill-point skill gated behind Cavalier Mastery Rank 5. SP cost is cut from
  a flat `40` to a flat `5`, and it gains a `30/25/20/15/10` second cooldown
  across ranks 1-5. Damage is now `100% ATK, +50% ATK per skill level for
  every 3 cells of distance` (capped at 4 steps, 12+ cells), up from the old
  flat `+100% per 3 cells`. The target is no longer knocked back on hit (the
  Knight still charges to melee range). It remains a normal player-targeted
  skill — the player still picks the enemy with the cursor — but the cast is
  now instant (was 500ms-1500ms, scaling with distance), so the Knight
  resumes auto-attacking the same target the instant the charge lands instead
  of standing idle. *Developer Comments: Charge Attack was a one-off quest
  curiosity nobody built around. Turning it into a ranked, cooldown-gated gap
  closer that scales with Cavalier Mastery gives mounted Knights a real spam
  tool; the instant cast keeps the charge from interrupting the Knight's
  attack flow, and dropping the knockback keeps it from fighting with the
  rest of the kit's positioning.*
  - `src/map/skills/swordman/chargeattack.cpp` — `calculateSkillRatio`'s
    pre-renewal branch changed from `base_skillratio += 100 * k;` to
    `base_skillratio += 50 * skill_lv * k;`; the pre-renewal `skill_blown`
    knockback call in `castendDamageId` moved inside the `RENEWAL`-only
    branch so pre-re no longer knocks the target back.
  - `db/import/skill_db.yml` — `MaxLevel: 5`, `CastTime: 0`,
    `Flags: { IsQuest: false }`, a per-level `Cooldown` (`30000` down to
    `10000`), and `Requires.SpCost: 5`. `TargetType` is left at its base
    `Attack` value (still player-targeted).
  - `src/map/unit.cpp` — the `case KN_CHARGEATK` distance-based cast-time
    multiplier is left in place with a comment noting it is now inert against
    the 0 base cast time.
  - `db/import/skill_tree.yml` — added to the `Knight` tree requiring
    `KN_CAVALIERMASTERY` level 5.
  - `lua files/skillinfoz/skilldescript.lub` — rewrote the `KN_CHARGEATK`
    entry to the current Cavalier Mastery requirement, `Enemy` targeting, the
    instant cast, and the new damage/SP/cooldown formula.
  - `lua files/skillinfoz/skillinfolist.lub` — `MaxLv: 5`,
    `SpAmount: { 5, 5, 5, 5, 5 }`, `AttackRange` restated per level, `Type`
    field (formerly `"Quest"`) removed, and `_NeedSkillList` added for
    Cavalier Mastery 5.

- **Bowling Bash & Sundering Strike (unlinked cooldowns):** The two skills no
  longer share a cooldown — casting one no longer puts the other on cooldown
  as well, so a Knight can freely alternate between them. Bowling Bash keeps
  its own `5 second` cooldown; Sundering Strike's is raised from `5` to
  `10 seconds` to stand on its own now that it isn't riding Bowling Bash's
  shorter timer. Bowling Bash now consumes up to `5` Momentum stacks (down
  from `10`) for `+60% ATK` each instead of `+30%`, so the total damage
  payoff at full stacks is unchanged (`+300%`) — it just arrives twice as
  fast per stack, matching Sundering Strike's own 5-stack cap. Sundering
  Strike's armor-crush debuff is nerfed to compensate for its longer uptime
  as an independent skill: `DEF` reduction drops from `10%` to `5% per skill
  level` (`25%` at level 5, down from `50%`).
  *Developer Comments: the shared-cooldown lockout was meant to stop players
  from alternating the two skills to bypass their downtime, but it made them
  feel like one skill wearing two faces instead of two distinct tools. Cutting
  the link lets each skill stand on its own cooldown and identity; Sundering
  Strike's longer cooldown and softer DEF shred keep it from just being
  Bowling Bash's on-demand backup now that it's free to fire independently.*
  - `src/map/skill.cpp` — removed the `KN_BOWLINGBASH`/`KN_ONEHAND` mutual
    `skill_blockpc_start` lockout from the `BL_PC` cooldown-application block.
  - `db/import/skill_db.yml` — `KN_ONEHAND` (Sundering Strike) `Cooldown`
    raised from `5000` to `10000`.
  - `src/map/skills/swordman/bowlingbash.cpp` — `calculateSkillRatio`'s
    Momentum bonus changed from `30 * min(10, stacks)` to
    `60 * min(5, stacks)`; `castendDamageId` now consumes only up to `5`
    stacks and re-applies any remainder, mirroring Sundering Strike's
    partial-consumption pattern instead of ending the whole buff outright.
  - `src/map/skills/swordman/sunderingstrike.cpp` — `SC_SUNDER`'s `val1`
    changed from `10 * skill_lv` to `5 * skill_lv`.
  - `src/map/status.hpp` — `SC_SUNDER` comment updated to `5 * skill level`
    (max `25%` at Lv 5).
  - `lua files/skillinfoz/skilldescript.lub` — `KN_BOWLINGBASH` and
    `KN_ONEHAND` entries updated to the new stack counts/percentages and
    cooldown, and both lost their now-inaccurate `Comments:` line about
    sharing a cooldown.

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

- **Owl's Eye:** In addition to its usual `+1 DEX per skill level` (+10 DEX at level 10), the skill now also grants `+3 ATK per skill level` (+30 ATK at level 10) as a passive. *Developer Comments: With arrows no longer required to fire a bow, Archers lost the flat ATK that their ammunition used to contribute; folding a small ATK bonus into Owl's Eye — a skill every Archer already maxes for the DEX — restores that lost damage without adding another button to press.*

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

- **Serrated Shot / Wyvern Bolt / Master Fletching (interaction rework):** The link between
  `Serrated Shot`, `Wyvern Bolt` and `Master Fletching` is retuned. `Serrated Shot` now shaves
  a guaranteed `2.5 seconds` off `Wyvern Bolt`'s remaining cooldown on **every hit** (previously
  a `5% per Master Fletching level` chance for `-5s`), so keeping up Serrated Shot pressure
  reliably pulls Wyvern Bolt's `40s` cooldown forward. In exchange, `Master Fletching` no longer
  touches Wyvern Bolt's cooldown at all — its third bonus is now `+10% Serrated Shot damage per
  level` (up to `+100%` at Lv10). `Serrated Shot`'s base damage is also lowered at the top end:
  the curve now runs `300% ATK` at Lv1 scaling linearly to `650% ATK` at Lv10 (previously `800%`
  at Lv10, a `-150%` cut at max), with the intermediate ranks smoothed across the range.
  *Developer Comments: the old random Wyvern proc was swingy and hard to feel; making the reduction
  a flat per-hit amount turns Serrated Shot into a dependable cooldown engine for Wyvern Bolt.
  Master Fletching's damage bonus now stacks cleanly on top of a lower base, so the max-rank
  ceiling (650% base + 100% from Master Fletching) lands close to where it was before without
  Serrated Shot hitting like a truck on its own.*

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

- **Wyvern Bolt:** The bolt can no longer miss (`IgnoreFlee`). *Developer Comments: as a weapon attack
  Wyvern Bolt was paying the full flee tax that its rival Predatory Strike (a Misc strike) skips
  entirely, so against high-flee targets it landed softer despite the bigger ATK% on the tin. Making it
  always connect lets its large ratio actually deliver against the targets where it was falling behind,
  keeping it a hair ahead of Predatory Strike single-target as intended.*

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
- **Predatory Strike:** Damage lowered by `100%` at every level — the curve now runs from
  `200% of a Blitz Beat at level 1` by `50% per level` to `400% at level 5` (200 / 250 / 300 / 350 /
  400%), down from 300–500%. *Developer Comments: Predatory Strike is a Misc falcon strike, so it
  ignores the target's DEF and (with IgnoreFlee) never misses, on top of hitting a 9x9 and applying
  a 2s Fear. Compared on paper against Wyvern Bolt's raw ATK% it looked even, but because it bypasses
  the whole mitigation layer it was quietly the stronger nuke against anything armored. Shaving 100%
  off each rank keeps it as the AoE/crowd-control option while ceding the single-target damage crown
  to Wyvern Bolt, as intended.*
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
