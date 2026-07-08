// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "arrowshower.hpp"

#include "../../pc.hpp"
#include "../../skill.hpp"
#include "../../status.hpp"

SkillArrowShower::SkillArrowShower() : SkillImplRecursiveDamageSplash(AC_SHOWER) {
}

void SkillArrowShower::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += 50 + 10 * skill_lv;
#else
	base_skillratio += 25 + 5 * skill_lv;
#endif
}

int64 SkillArrowShower::splashDamage(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
	// Deal the normal splash hit and capture the damage dealt to this particular target.
	int64 damage = SkillImplRecursiveDamageSplash::splashDamage(src, target, skill_lv, tick, flag);

#ifndef RENEWAL
	// Land Mine passive: for a Hunter who has learned Land Mine, every splashed target gets an
	// "arrow" that tears open 1.5s later for 25% of the damage it took, all in one burst.
	map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd != nullptr && pc_checkskill(sd, HT_LANDMINE) > 0)
		skill_apply_landmine_bleed(src, target, damage);
#endif

	return damage;
}

void SkillArrowShower::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change_end(src, SC_CAMOUFLAGE);

	SkillImplRecursiveDamageSplash::castendPos2(src, x, y, skill_lv, tick, flag);
}
