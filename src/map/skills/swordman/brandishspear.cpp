// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "brandishspear.hpp"

#include <config/core.hpp>

#include "map/battle.hpp"
#include "map/map.hpp"
#include "map/pc.hpp"
#include "map/status.hpp"
#include "map/unit.hpp"

SkillBrandishSpear::SkillBrandishSpear() : SkillImpl(KN_BRANDISHSPEAR) {
}

#ifndef RENEWAL
// Payon Stories Spear Knight rework: suppress the default knockback so the
// post-damage pull below can gather enemies inward instead.
void SkillBrandishSpear::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	dmg.blewcount = 0;
}
#endif

#ifndef RENEWAL
// Payon Stories Spear Knight rework: after Brandish Spear resolves, pull every
// enemy in its fan 2 cells toward the caster. The pull also tags those enemies
// with SC_OFFBALANCE via skill_blown, which lets a follow-up Spear Stab cash
// in on the cluster.
static int32 brandishspear_pull_sub(block_list* bl, va_list ap) {
	block_list* src = va_arg(ap, block_list*);

	if (bl == nullptr || src == nullptr || bl == src)
		return 0;
	if (status_bl_has_mode(bl, MD_STATUSIMMUNE))
		return 0;
	if (battle_check_target(src, bl, BCT_ENEMY) <= 0)
		return 0;

	// skill_blown always *negates* the supplied dir vector (it's hardcoded as
	// a knockback function). To pull the enemy toward the caster we pass the
	// direction from caster -> enemy, so the negation lands the enemy back
	// toward us.
	int8 dir = map_calc_dir(src, bl->x, bl->y);
	skill_blown(src, bl, 2, dir, BLOWN_NONE);

	// Tag the pulled enemy as Off-Balance for 4 s so Spear Stab can punish.
	// Applied here (instead of in skill_blown) so generic knockbacks from
	// other skills don't paint half the map gray.
	sc_start(src, bl, SC_OFFBALANCE, 100, 1, 4000);
	return 1;
}
#endif

void SkillBrandishSpear::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	map_foreachindir(skill_area_sub, src->m, src->x, src->y, target->x, target->y,
		skill_get_splash(getSkillId(), skill_lv), skill_get_maxcount(getSkillId(), skill_lv), 0, splash_target(src),
		src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 0,
		skill_castend_damage_id);
#else
	map_session_data* sd = BL_CAST(BL_PC, src);

	skill_area_temp[1] = target->id;

	if(skill_lv >= 10)
		map_foreachindir(skill_area_sub, src->m, src->x, src->y, target->x, target->y,
			skill_get_splash(getSkillId(), skill_lv), 1, skill_get_maxcount(getSkillId(), skill_lv)-1, splash_target(src),
			src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | (sd?3:0),
			skill_castend_damage_id);
	if(skill_lv >= 7)
		map_foreachindir(skill_area_sub, src->m, src->x, src->y, target->x, target->y,
			skill_get_splash(getSkillId(), skill_lv), 1, skill_get_maxcount(getSkillId(), skill_lv)-2, splash_target(src),
			src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | (sd?2:0),
			skill_castend_damage_id);
	if(skill_lv >= 4)
		map_foreachindir(skill_area_sub, src->m, src->x, src->y, target->x, target->y,
			skill_get_splash(getSkillId(), skill_lv), 1, skill_get_maxcount(getSkillId(), skill_lv)-3, splash_target(src),
			src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | (sd?1:0),
			skill_castend_damage_id);
	map_foreachindir(skill_area_sub, src->m, src->x, src->y, target->x, target->y,
		skill_get_splash(getSkillId(), skill_lv), skill_get_maxcount(getSkillId(), skill_lv)-3, 0, splash_target(src),
		src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 0,
		skill_castend_damage_id);

	// Payon Stories Spear Knight rework: pull every enemy in the fan 2 cells
	// toward the caster after the damage resolves. Using the same line iterator
	// as the damage pass keeps the affected set identical.
	map_foreachindir(brandishspear_pull_sub, src->m, src->x, src->y, target->x, target->y,
		skill_get_splash(getSkillId(), skill_lv), skill_get_maxcount(getSkillId(), skill_lv), 0, splash_target(src),
		src);
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
	int32 ratio = 100 + 20 * skill_lv;

	base_skillratio += -100 + ratio;
	if (skill_lv > 3 && wd->miscflag == 0)
		base_skillratio += ratio / 2;
	if (skill_lv > 6 && wd->miscflag == 0)
		base_skillratio += ratio / 4;
	if (skill_lv > 9 && wd->miscflag == 0)
		base_skillratio += ratio / 8;
	if (skill_lv > 6 && wd->miscflag == 1)
		base_skillratio += ratio / 2;
	if (skill_lv > 9 && wd->miscflag == 1)
		base_skillratio += ratio / 4;
	if (skill_lv > 9 && wd->miscflag == 2)
		base_skillratio += ratio / 2;
#endif
}
