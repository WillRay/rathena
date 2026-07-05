// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "blitzbeat.hpp"

#include <config/core.hpp>

#include "map/battle.hpp"
#include "map/skill.hpp"
#include "map/status.hpp"

SkillBlitzBeat::SkillBlitzBeat() : SkillImplRecursiveDamageSplash(HT_BLITZBEAT) {
}

void SkillBlitzBeat::splashSearch(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 flag) const {
#ifndef RENEWAL
	// Hunter rebalance: Blitz Beat is a single-target strike by default, but when
	// it lands on a target carrying the "Hunted" mark (SC_HUNTED) it becomes a
	// small area strike. Everything in a 5x5 area (splash radius 2, the closest
	// centered area to the intended 4x4 footprint) around the Hunted target is
	// hit; the matching +100% damage bonus is applied in battle_calc_misc_attack
	// (src/map/battle.cpp). The mark is consumed once the empowered strike has
	// resolved (see below) — a falcon strike always spends the Hunted mark.
	status_change* tsc = status_get_sc(target);
	bool hunted = (tsc != nullptr && tsc->getSCE(SC_HUNTED) != nullptr);
	int16 size = hunted ? 2 : 0;

	// Recursive invocation of skill_castend_damage_id() with flag|1 for every
	// enemy in the area. A size of 0 restricts it to the target's own cell, i.e.
	// the vanilla single-target behaviour when the target is not Hunted. The mark
	// must survive this call so both the splash sizing above and the +40% damage
	// in battle.cpp can read it, so it is only consumed afterwards.
	map_foreachinrange(skill_area_sub, target, size, this->getSplashTarget(src), src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);

	if (hunted) {
		// The mark is consumed once the empowered strike has resolved.
		status_change_end(target, SC_HUNTED);
	}
#else
	SkillImplRecursiveDamageSplash::splashSearch(src, target, skill_lv, tick, flag);
#endif
}
