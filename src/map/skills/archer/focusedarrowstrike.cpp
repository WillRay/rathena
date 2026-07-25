// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "focusedarrowstrike.hpp"

#include <config/core.hpp>

#include "map/clif.hpp"
#include "map/skill.hpp"
#include "map/status.hpp"

SkillFocusedArrowStrike::SkillFocusedArrowStrike() : SkillImplRecursiveDamageSplash(SN_SHARPSHOOTING) {
}

void SkillFocusedArrowStrike::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	if (src->type == BL_MOB) { // TODO: Did these formulas change in the renewal balancing?
		skillratio += 100 + 50 * skill_lv;
		return;
	}
#ifdef RENEWAL
	skillratio += -100 + 300 + 300 * skill_lv;
	RE_LVL_DMOD(100);
#else
	// Lethal Arrow: single target, 4 hits (HitCount in skill_db). Each hit deals
	// 100% ATK + 40% per level (300% at Lv 5); total across all 4 hits is
	// 400% + 160% per level (1200% at Lv 5) - double the old single-hit total
	// of 200% + 80% per level. skillratio starts at 100.
	skillratio += 40 * skill_lv;

	// Execution: +10% ATK for every 10% of the target's max HP it is missing
	// (i.e. +1% per 1% missing), up to +100% against a near-dead target.
	const status_data *tstatus = status_get_status_data(*target);
	if (tstatus->max_hp > 0)
		skillratio += 100 - static_cast<int32>(100 * tstatus->hp / tstatus->max_hp);
#endif
}

void SkillFocusedArrowStrike::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	SkillImplRecursiveDamageSplash::castendDamageId(src, target, skill_lv, tick, flag);

	if( flag&1 ) {
		status_change_end(src, SC_CAMOUFLAGE);
	}
#else
	// Lethal Arrow borrows Chain Reaction Shot's two-stage visual: this packet
	// plays the arrow/mark on the target, and the real damage packet is sent
	// under ABC_CHAIN_REACTION_SHOT_ATK's id (see skill.cpp) so the detonation
	// replaces this skill's own arrow-corridor effect entirely.
	clif_skill_nodamage(src, *target, ABC_CHAIN_REACTION_SHOT, skill_lv);

	// Single target, 4 hits: skill_attack applies the per-hit ratio from
	// calculateSkillRatio once, then the engine multiplies by the skill_db
	// HitCount for the total/display, same pattern as Double Strafe.
	skill_attack(BF_WEAPON, src, src, target, getSkillId(), skill_lv, tick, flag);

	// Neither ABC_CHAIN_REACTION_SHOT nor its _ATK variant has a name entry on
	// this client (they're borrowed 4th-job ids with no localized skill data
	// here), so the floating cast banner shows a blank "!!" unless the real id
	// is re-announced afterward. This briefly flashes SN_SHARPSHOOTING's own
	// registered cast effect right after the hit - the same tradeoff already
	// accepted for Sundering Strike's borrowed Cart Termination visual, since
	// there is no packet that carries a name without also carrying an effect
	// for that id.
	clif_skill_nodamage(src, *target, SN_SHARPSHOOTING, skill_lv);
#endif
}
