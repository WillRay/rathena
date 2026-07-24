# VIT Stat Redesign — Design Document

Server: Payon Stories (pre-renewal rAthena fork). This document specifies the redesigned VIT
stat: its target identity, the formulas that implement it, the data behind those formulas, the
class-by-class impact, balance risks, and the phased plan to implement it in code.

Status of this document: **approved design**. Implementation follows the phased plan in §6.

---

## 0. Target stat identities (unchanged, for reference)

| Stat | Identity |
|---|---|
| STR | physical power |
| DEX | precision, hit rate, cast speed, ranged effectiveness |
| INT | magical power, SP, SP regen, magical utility |
| AGI | speed, ASPD, avoidance |
| **VIT** | **endurance, resilience, resistance to physical and magical punishment** |
| LUK | critical chance, perfect dodge, existing luck-based mechanics |

VIT no longer shares soft MDEF with INT, and no longer shares defensive status resistance with
LUK. INT keeps MATK/SP/SP-regen/casting; LUK keeps crit/perfect-dodge/offensive utility.

---

## 1. Current-state summary

Verified against this fork's `src/map/status.cpp` and `src/map/battle.cpp` (pre-renewal, `PRERE`
active):

- **Soft DEF**: `def2 = VIT` (status.cpp:2711), reapplied as a delta on stat recalc
  (status.cpp:6066). Consumed in `battle_calc_defense_reduction()` (battle.cpp:4844-4848) via a
  **hidden random roll**: `vit_def = 0.3·VIT + rnd(0, max(0, VIT²/150 − 0.3·VIT − 1)) + 0.5·VIT`.
  This is flat (~0.8×VIT) until ~VIT 55, then a widening random quadratic tail. Average outcome:
  VIT 60 → ~50.5, VIT 100 → ~97.8, VIT 150 → ~172. Nothing about this is visible on the status
  window, and piercing attacks multiply damage by `(def1 + vit_def)`, so the hidden roll also
  swings piercing damage per-hit.
- **Soft MDEF**: `mdef2 = INT + VIT/2` (status.cpp:2715) — magical durability is tied to INT, not
  VIT, contrary to the target identity.
- **Status resistance**: `status_get_sc_def()` (status.cpp:9652-10004) is stock rAthena. Stun/
  Silence/Bleeding/Poison scale off `VIT×100`; Sleep off `INT×100`; Blind off `(VIT+INT)×50`;
  Confusion off `(STR+INT)×50`; Curse off `LUK×100` (with a quirk: `LUK==0` grants **full
  immunity**); Freeze/Stone off hard MDEF. A flat term `LUK×10 + level_diff×10` is subtracted
  after the percentage roll, which is what produces the well-known "97 VIT = stun immune"
  breakpoint — a piece of hidden knowledge, not a stated mechanic.
- **Max HP**: `×(1 + 0.01×VIT)` (status.cpp:3521) — kept as-is.
- **Potion/heal effectiveness**: `+2% per VIT` (pc.cpp:10711) — already smooth, kept as-is.
- **HP regen**: `VIT/5` stepped every 5 points (status.cpp:5284) — optional smoothing only.

Net effect: VIT reads as "HP stat," soft DEF craters against late-game damage, soft MDEF is
INT's problem, and status resistance is a spreadsheet lookup rather than a felt mechanic.

---

## 2. Soft DEF — candidate formulas and chosen value

**Formula:** `def2 = VIT + VIT² / K` (players only; see §5 on mob gating), applied
**deterministically** — the hidden random roll in `battle.cpp` is removed for players.

| VIT | Current avg (hidden roll) | K=300 | K=200 | **K=150 (chosen)** |
|----:|----:|----:|----:|----:|
| 1   | ~1  | 1   | 1   | 1   |
| 20  | 16  | 21  | 22  | 22  |
| 40  | 32  | 45  | 48  | 50  |
| 60  | 50  | 72  | 78  | 84  |
| 80  | 73  | 101 | 112 | 122 |
| 100 | 98  | 133 | 150 | 166 |
| 120 | 126 | 168 | 192 | 216 |
| 150 | 172 | 225 | 262 | 300 |

**Chosen: K = 150.** This lands close to the *ceiling* of the old hidden roll at every VIT value,
made deterministic and visible, and gives roughly 1.7× the old average value at VIT 100 — dedicated
tanks feel a clear step up without any single point being wasted. Growth stays smooth: the
quadratic term never dominates the linear term until VIT exceeds 150 (the practical soft cap),
so there's no runaway scaling.

Config: `vit_def2_quadratic_divisor`, default **150**, `0` disables the quadratic term (falls
back to stock linear `def2 = VIT`).

---

## 3. Soft MDEF — candidate formulas and chosen value

**Formula:** `mdef2 = VIT + VIT² / K2` (players only). The INT term is removed entirely — magical
durability becomes purely a VIT function, matching the target identity. INT retains MATK/SP/regen.

| VIT | K2=300 | **K2=400 (chosen)** | K2=600 |
|----:|----:|----:|----:|
| 1   | 1   | 1   | 1   |
| 20  | 21  | 21  | 20  |
| 40  | 45  | 44  | 42  |
| 60  | 72  | 69  | 66  |
| 80  | 101 | 96  | 90  |
| 100 | 133 | 125 | 116 |
| 120 | 168 | 156 | 144 |
| 150 | 225 | 206 | 187 |

**Chosen: K2 = 400.** Keeps mdef2 ~20-25% below def2 at equal VIT — magic remains the tank's
harder matchup, and gear's percentage-based MDEF (armor refine, cards) remains the primary lever
against magic, consistent with the constraint that VIT should not become a second armor system.

Config: `vit_mdef2_quadratic_divisor`, default **400**, `0` disables (falls back to `mdef2 = 0`
from VIT — INT no longer contributes at all once this change ships; see §5 migration note).

**Migration note:** INT-only casters currently get free mdef2 from `INT`. Removing it is an
intended transfer of magical durability from INT to VIT — casters who want tankiness now buy
VIT for it, rather than getting it as an INT side effect. Call this out explicitly in patch notes.

---

## 4. Status resistance — continuous scaling, chosen curve

**Formula:** `sc_def = 10000 · VIT / (VIT + K3)` (the existing 10000 = 100% scale). This is a
diminishing-returns curve with no breakpoint: it asymptotically approaches but never reaches 100%
from stats alone. `VIT == K3` is exactly the 50%-resist point.

| VIT | K3=100 | **K3=60 (chosen)** | K3=40 |
|----:|----:|----:|----:|
| 1   | 1.0%  | 1.6%  | 2.4%  |
| 20  | 16.7% | 25.0% | 33.3% |
| 40  | 28.6% | 40.0% | 50.0% |
| 60  | 37.5% | 50.0% | 60.0% |
| 80  | 44.4% | 57.1% | 66.7% |
| 100 | 50.0% | 62.5% | 71.4% |
| 120 | 54.5% | 66.7% | 75.0% |
| 150 | 60.0% | 71.4% | 78.9% |

**Chosen: K3 = 60.** VIT 60 (a realistic mid-investment for a hybrid build) reaches 50% chance
resist; VIT 100+ (a dedicated tank) reaches 62-70%+. Both chance and duration reduction use the
same curve (`tick_def = -1` propagates it in the existing code path), so expected total disable
time scales as roughly `(1 − resist)²` — at VIT 100 a stun lands 37.5% as often and lasts 37.5%
as long as the unresisted case, i.e. ~14% of baseline total disable time. That's a strong,
continuously-felt investment payoff without ever producing a hard immunity wall from stats alone.
Immunity remains obtainable only through gear/skills (bResEff cards, SC_SCRESIST, GTB-style
effects) — itemization counterplay is preserved.

Config: `vit_sc_def_halfpoint`, default **60**.

### Per-status consolidation

| Status | Old sc_def | New sc_def | Notes |
|---|---|---|---|
| Stun | VIT×100 | curve(VIT) | drop `LUK×10` flat term |
| Silence | VIT×100 | curve(VIT) | drop `LUK×10` flat term |
| Bleeding | VIT×100 | curve(VIT) | drop `LUK×10` flat term |
| Poison | VIT×100 | curve(VIT) | duration: `tick_def = curve(VIT)×3/4` (preserves the old "poison duration resists slower than chance" asymmetry, continuously instead of via a breakpoint) |
| Sleep | **INT×100** | curve(VIT) | moved from INT to VIT |
| Blind | (VIT+INT)×50 | curve(VIT) | moved fully to VIT |
| Confusion | **(STR+INT)×50** | curve(VIT) | moved from STR/INT to VIT; also drop the old reversed (attacker-favoring) `sc_def2` term |
| Curse | **LUK×100**, `LUK==0`→immunity | curve(VIT) | moved from LUK to VIT; **delete the LUK==0 immunity quirk**; duration `tick_def = -1` (curve) instead of the old `VIT×100` (which already hit 100% at VIT 100 — now continuous) |
| Freeze | hard MDEF×100 | **unchanged** | stays on equipment (counterplay preserved); drop `LUK×10` defensive flat term, keep the caster-side LUK duration bonus (offensive, not defensive) |
| Stone | hard MDEF×100 | **unchanged** | stays on equipment |

All remaining flat `sc_def2` terms become `level_diff × 10` only (the level-difference component
is kept — see §5). `LUK`'s defensive role is fully removed from all of the above; LUK retains its
existing offensive/utility role (crit, perfect dodge) untouched.

The Aegis "round rate up to the next 0.1%" quirk (status.cpp:9974) is removed for pre-renewal
only — with a continuous curve, that rounding systematically favored the attacker at exactly the
low resist values the curve is meant to produce.

---

## 5. Flat HP bonus from VIT (new)

In addition to the existing `×(1 + 0.01×VIT)` multiplier (kept unchanged), VIT grants a flat,
capped HP bonus so early investment feels rewarding even before the multiplier has much base HP
to work with:

**`flat_hp = min(1000, floor(VIT^1.5))`**

| VIT | Flat HP |
|----:|----:|
| 1   | 1 |
| 20  | 89 |
| 40  | 253 |
| 60  | 464 |
| 80  | 715 |
| 100 | 1000 (cap reached) |
| 120 | 1000 |
| 150 | 1000 |

`VIT^1.5` is worth ~4.5 HP/point around VIT 20 (meaningful next to a novice's own two-digit HP
pool) and ~14 HP/point approaching the cap, then flattens completely at 1000 so it can't stack
runaway with job HP multipliers (trans ×1.25, etc.) at very high VIT — by VIT 100+ the existing
percentage multiplier is doing all the late-game scaling work, exactly as intended.

Config: `vit_flat_hp_cap`, default **1000**, `0` disables.

---

## 6. Class impact analysis

- **Dedicated tanks (Crusader/Knight, 80-120 VIT):** the clearest winners. Deterministic def2
  122-192, mdef2 96-156, ~57-67% status resist, plus up to +1000 flat HP — VIT now visibly reads
  as "I am the tank" on the status window instead of being inferred from a HP number.
- **Melee hybrids (60-80 VIT):** smooth mid-curve payoff; every point from 40→80 buys
  increasing (not flat, not stepped) returns, so there's no "stop at 79, next breakpoint isn't
  worth it" calculus.
- **Ranged/DEX-AGI classes (20-40 VIT):** low investment still returns honest, visible value
  (def2 22-50, resist 25-40%). Losing the old 97-VIT stun-immunity meta doesn't hurt them
  relatively, since nobody gets stat-only immunity anymore — the playing field for "who gets
  chain-stunned" flattens.
- **Casters:** lose INT's old free mdef2 and Sleep resistance — this is the deliberate INT→VIT
  transfer. A caster who wants real magical durability now has to buy VIT for it rather than
  getting it as an INT side effect; a glass-cannon build is still viable, it's just honestly a
  glass cannon now.
- **PvP-specific:** piercing/ignore-def skills (e.g. Investigate) multiply damage by
  `(def1 + vit_def)`, so a bigger, deterministic vit_def makes piercing damage against VIT tanks
  both stronger on average and (once the random roll is removed) perfectly predictable per hit —
  flagged as the single biggest PvP shift from this redesign; measure before finalizing K.

---

## 7. Balance concerns and mitigations

- **Mob soft DEF/MDEF must not silently buff every monster.** Gate the quadratic def2/mdef2
  terms to `BL_PC` only (the fork already does this for its custom hit/flee bonuses) — mob PvE
  damage math is untouched.
- **Mob status resistance does change** with the shared curve (a VIT-60 mob goes from 60% to 50%
  stun resist at K3=60). Mitigate with the existing `mob_status_def_rate` battle config
  (e.g. raise to 120-150) rather than adding new code — no new hidden mechanic introduced.
- **Angelus** (`def2 += def2×val2/100`, status.cpp:7898) now multiplies a quadratic base instead
  of a linear one — stronger on high-VIT targets. Flag for a follow-up balance pass; no code
  change bundled with this redesign.
- **Acid Terror** subtracts `tstatus->def2` directly (battle.cpp:4251) and will scale down faster
  against VIT tanks than before — note in patch notes, no code change required.
- **Magic flat reduction shifts from INT builds to VIT builds** (battle.cpp:6176 uses mdef2) —
  intended, must be clearly communicated since it changes a caster's effective magic tankiness.
- **Runaway-scaling check:** at the practical soft stat cap (VIT 150), def2 = 300, mdef2 = 206 —
  both well below `SHRT_MAX` and far short of "second armor system" territory; no percentage
  damage reduction is added anywhere, honoring the constraint that armor refinement remains the
  sole percentage-based mitigation layer.
- **Curse `LUK==0` immunity removal** means mobs/NPCs with 0 LUK (commonly plants, statues) can
  now be cursed. Acceptable — bosses/immune mobs are still protected via `MD_STATUSIMMUNE` and
  per-mob `MinRate` overrides in `status.yml`, unrelated to this change.

---

## 8. Phased implementation plan

Each phase is a separate commit; compile and smoke-test between phases. Phases are ordered
low-risk-and-isolated → high-risk-and-broad.

**Phase 0 — Tunables (no risk).**
Add to `src/custom/battle_config_struct.inc` / `battle_config_init.inc`:
- `vit_def2_quadratic_divisor` (default 150, 0 = off)
- `vit_mdef2_quadratic_divisor` (default 400, 0 = off)
- `vit_sc_def_halfpoint` (default 60)
- `vit_flat_hp_cap` (default 1000, 0 = off)

Document defaults in `conf/battle/status.conf` alongside the existing `pc_max_status_def` /
`mob_status_def_rate` knobs.

**Phase 1 — Soft DEF / soft MDEF base formulas (low-medium risk).**
`src/map/status.cpp`:
- Add helpers near `status_base_atk` (this fork's existing house style for smoothed stat math —
  see `dstr = str/10.0` and the MATK helpers): `status_calc_vit_def2_bonus(bl, vit)` and
  `status_calc_vit_mdef2_bonus(bl, vit, int_)`. PC path: quadratic per §2/§3. Non-PC path: stock
  formulas unchanged (`def2 = vit`, `mdef2 = int_ + vit/2`).
- Use these helpers in `status_calc_misc()` (status.cpp:2709-2716).
- Replace the delta terms in `status_calc_bl_main()` (SCB_DEF2 at status.cpp:6054-6068, SCB_MDEF2
  at 6078-6093) with the **functional delta** `b_status->def2 - f(b_vit) + f(vit)` (and the mdef2
  equivalent) — this is drift-proof by construction regardless of how many times a stat-changing
  SC fires, unlike patching the existing linear-delta arithmetic in place.

**Phase 1b — Flat HP bonus (low risk).**
`status_calc_maxhp_pc()` (status.cpp:3508-3550): add
`min(vit_flat_hp_cap, floor(pow(vit, 1.5)))` as a flat addition after the existing multipliers,
alongside the existing `param_equip[PARAM_VIT]` flat addition. PC-only (function is already
PC-only).

**Phase 2 — Deterministic soft DEF application (low risk).**
`src/map/battle.cpp`, `battle_calc_defense_reduction()`: replace the three-line random roll at
lines 4844-4848 (player branch) with `vit_def = def2;`. Leave the mob branch (4862-4868)
untouched — mob soft-def randomness is stock and out of scope.

**Phase 3 — Status resistance rewrite (high risk, largest surface).**
`src/map/status.cpp`, `status_get_sc_def()` (9652-10004):
- Add a shared helper `status_sc_def_curve(stat)` implementing `10000×stat/(stat+K3)`.
- Rewrite the pre-renewal branches for Stun, Silence, Bleeding, Poison, Sleep, Blind, Confusion,
  Curse per the table in §4. Leave Freeze/Stone's `sc_def` on hard MDEF; only drop their `LUK×10`
  defensive flat term.
- Move the Curse `LUK==0` immunity check inside the `RENEWAL`-only path (delete it from pre-re).
- Remove the Aegis round-up-to-10 quirk for pre-re only.
- Leave `SC_SCRESIST`/`SC_SIEGFRIED`/item `bResEff`/`MinRate`/`MinDuration`/`pc_max_sc_def`
  entirely alone — they compose on top of the curve unchanged.

**Phase 4 — Regen smoothing (optional, trivial, do last).**
`status_calc_regen()` (status.cpp:5284): smooth `VIT/5` using float math in the same style as
Phase 1's helpers. Cosmetic only (±1 HP/tick at boundaries); skip if time-constrained.

---

## 9. Verification checklist

- **Phase 1:** at `@vit` 1/50/100/150, confirm status-window "+def" = `VIT + VIT²/150` and
  "+mdef" = `VIT + VIT²/400`; confirm MaxHP includes the flat `min(1000, VIT^1.5)` term on top of
  the existing percentage. Apply Angelus and a VIT food buff, let both expire, repeat 5× — def2
  must return exactly to its pre-buff base each time (drift check on the new delta logic).
  `@setbattleflag vit_def2_quadratic_divisor 0` must restore stock linear values. Confirm mob
  damage output is unchanged (BL_PC gating check).
- **Phase 2:** hit a VIT-100 test character with a fixed-ATK source (mob with atk_min==atk_max,
  neutral element/size) 20 times — damage must now be constant, not a range. Record the
  Investigate/piercing damage delta against the same target for the balance follow-up.
- **Phase 3:** using a scripted `sc_start` NPC, run 200 trials at VIT 1/100/150 and confirm
  infliction rate tracks the curve; confirm VIT 100 is **not** immune (previously was). Curse a
  0-LUK character and confirm it now lands. Confirm Freeze/Stone resistance still tracks only
  equipped hard MDEF. Confirm a `bResEff` resist card still stacks with the new curve. Stun a
  high-VIT and a low-VIT mob and compare against expectations; adjust `mob_status_def_rate` if
  mobs feel too fragile.
- **Phase 4:** compare HP regen tick at `@vit 99` vs `@vit 100` — should differ by ~1 HP, not
  jump.

---

## 10. Success criteria mapping

| Goal | How this design satisfies it |
|---|---|
| VIT feels valuable at every point | Quadratic def2/mdef2, curve-based resist, and flat HP bonus are all continuous — no stepped or breakpoint terms remain in VIT's own formulas |
| Remains relevant late-game | Quadratic term keeps growing (VIT 150 def2 = 300, mdef2 = 206); resist curve keeps climbing past 70%+ without ever handing out free immunity |
| No hidden breakpoints | The 97-VIT stun-immunity wall and the LUK==0 curse-immunity quirk are both removed; all resistance is now visibly continuous |
| Not mandatory for every build | Ranged/caster builds keep full viability at low VIT with honest (if smaller) returns; no offensive power is granted by VIT |
| No large % damage reduction added | Soft DEF/MDEF stay additive/flat-subtractive in the damage formula, exactly as stock; nothing new multiplies damage by a VIT-derived percentage |
| Clear identity, less overlap | Soft MDEF moves fully off INT; defensive status resistance moves fully off LUK and (for Sleep/Blind/Confusion) off STR/INT; Freeze/Stone deliberately stay on gear MDEF as the one explicit exception, preserving itemization counterplay |
