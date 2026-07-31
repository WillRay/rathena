// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "brandishspear.hpp"

#include <config/core.hpp>

#include "map/clif.hpp"
#include "map/pc.hpp"
#include "map/status.hpp"
#include "map/unit.hpp"

SkillBrandishSpear::SkillBrandishSpear() : SkillImpl(KN_BRANDISHSPEAR) {
}

void SkillBrandishSpear::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	map_foreachindir(skill_area_sub, src->m, src->x, src->y, target->x, target->y,
		skill_get_splash(getSkillId(), skill_lv), skill_get_maxcount(getSkillId(), skill_lv), 0, splash_target(src),
		src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 0,
		skill_castend_damage_id);
#else
	// Payon Stories rebalance: Brandish Spear redesigned into the Knight's
	// signature spear follow-up/mob-control tool. The old per-level staggered
	// directional falloff is gone; every level now does the same thing - a
	// flat circular splash centered on the clicked target (skill_area_temp[4]/
	// [5] stash the epicenter so applyAdditionalEffects can pull every hit
	// target back to the same spot after damage lands).
	skill_area_temp[1] = target->id;
	skill_area_temp[4] = target->x;
	skill_area_temp[5] = target->y;

	map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), splash_target(src),
		src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1,
		skill_castend_damage_id);
#endif
}

void SkillBrandishSpear::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
#else
	//Coded apart for it needs the flag passed to the damage calculation.
	if (skill_area_temp[1] != target->id)
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag|SD_ANIMATION);
	else
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
#endif
}

void SkillBrandishSpear::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	const status_data* sstatus = status_get_status_data(*src);

	base_skillratio += -100 + 400 + 100 * skill_lv + sstatus->str * 3;
#else
	// Payon Stories rebalance: 250% weapon ATK at Lv 1, +50% per level, capping
	// at 700% at Lv 10. base_skillratio starts at 100, so the addition below
	// yields a displayed (200 + 50 * skill_lv)% at every level.
	base_skillratio += 100 + 50 * skill_lv;
#endif
}

void SkillBrandishSpear::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
#ifndef RENEWAL
	int16 x = skill_area_temp[4], y = skill_area_temp[5];

	sc_start(src, target, SC_BLIND, 100, skill_lv, 5000);

	// Pull every target caught in the splash back to the impact point,
	// gathering the group for a follow-up AoE.
	if ((target->x != x || target->y != y) && !unit_blown_immune(target, 0x1)) {
		unit_movepos(target, x, y, 0, 0);
		clif_fixpos(*target);
	}
#endif
}
