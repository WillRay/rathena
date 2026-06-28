// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "spearstab.hpp"

#include <config/core.hpp>

#include "map/status.hpp"
#include "map/unit.hpp"

SkillSpearStab::SkillSpearStab() : SkillImpl(KN_SPEARSTAB) {
}

void SkillSpearStab::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	dmg.blewcount = 0;
}

void SkillSpearStab::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if(flag&1) {
		if (target->id==skill_area_temp[1])
			return;
#ifdef RENEWAL
		if (skill_attack(BF_WEAPON,src,src,target,getSkillId(), skill_lv, tick, SD_ANIMATION))
			skill_blown(src,target,skill_area_temp[2],-1,BLOWN_NONE);
#else
		// Payon Stories Spear Knight rework: knockback removed; Charge Attack owns that role.
		skill_attack(BF_WEAPON,src,src,target,getSkillId(), skill_lv, tick, SD_ANIMATION);
#endif
	} else {
		int32 x=target->x,y=target->y,i,dir;
		dir = map_calc_dir(target,src->x,src->y);
		skill_area_temp[1] = target->id;
		skill_area_temp[2] = skill_get_blewcount(getSkillId(),skill_lv);
		// all the enemies between the caster and the target are hit, as well as the target
#ifdef RENEWAL
		if (skill_attack(BF_WEAPON,src,src,target, getSkillId(),skill_lv,tick,0))
			skill_blown(src,target,skill_area_temp[2],-1,BLOWN_NONE);
#else
		skill_attack(BF_WEAPON,src,src,target, getSkillId(),skill_lv,tick,0);
#endif
		for (i=0;i<4;i++) {
			map_foreachincell(skill_area_sub,target->m,x,y,BL_CHAR,
				src, getSkillId(),skill_lv,tick,flag|BCT_ENEMY|1,skill_castend_damage_id);
			x += dirx[dir];
			y += diry[dir];
		}
	}
}

void SkillSpearStab::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += 20 * skill_lv;

#ifndef RENEWAL
	// Payon Stories Spear Knight rework: punish targets that are currently
	// being moved by an outside force (Charge Attack knockback, Brandish pull,
	// Magnum Break blowback, etc.). Doubles the damage of a follow-up Stab.
	const status_change* tsc = status_get_sc(target);
	if (tsc != nullptr && tsc->getSCE(SC_OFFBALANCE))
		base_skillratio += 100 + 10 * skill_lv;
#endif
}
