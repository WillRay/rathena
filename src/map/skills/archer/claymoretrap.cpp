// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "claymoretrap.hpp"

#include "map/skill.hpp"

SkillClaymoreTrap::SkillClaymoreTrap() : SkillImplRecursiveDamageSplash(HT_CLAYMORETRAP) {
}

void SkillClaymoreTrap::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if (flag & 1) {
		// Recursive per-target invocation from the detonation splash search
		// (see skill_timerskill, case HT_CLAYMORETRAP): apply damage to this target.
		SkillImplRecursiveDamageSplash::castendDamageId(src, target, skill_lv, tick, flag);
		return;
	}

	// Initial cast on the caster's current target. The trap arms on the target and
	// detonates after 2 seconds; the delayed hit is routed back through
	// skill_timerskill, which runs the splash search so each target re-enters here
	// with flag&1. No caster animation is played here - the only visual is the
	// EF_CLAYMORE explosion shown on the target when the damage lands.
	skill_addtimerskill(src, tick + 2000, target->id, 0, 0, getSkillId(), skill_lv, skill_get_type(getSkillId()), flag);
}
