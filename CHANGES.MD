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

Everything in this changelog is scoped to the **Swordsman, Knight and Crusader**
trees. Other classes are untouched in this pass.

---

## Class Changes

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
  - `src/map/status.cpp` (Two-Hand Quicken FLEE/CRIT, Spear Quicken split)

  All edits are guarded with `#ifndef RENEWAL` where appropriate, so renewal
  behavior is unchanged.
