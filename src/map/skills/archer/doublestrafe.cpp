// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "doublestrafe.hpp"

#include "../../pc.hpp"
#include "../../skill.hpp"

SkillDoubleStrafe::SkillDoubleStrafe() : WeaponSkillImpl(AC_DOUBLE) {
}

void SkillDoubleStrafe::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += 120 + 10 * (skill_lv - 1);
}

void SkillDoubleStrafe::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Normal Double Strafe hit; capture the damage dealt so the Land Mine bleed can scale off it.
	int64 damage = skill_attack(BF_WEAPON, src, src, target, getSkillId(), skill_lv, tick, flag);

#ifndef RENEWAL
	// Land Mine passive: a Hunter who has learned Land Mine lodges an "arrow" in the struck
	// target that tears open 1.5s later for 25% of the damage dealt, all in one burst.
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd != nullptr && pc_checkskill(sd, HT_LANDMINE) > 0)
		skill_apply_landmine_bleed(src, target, damage);
#endif
}
