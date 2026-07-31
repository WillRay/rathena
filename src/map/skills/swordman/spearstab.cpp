// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "spearstab.hpp"

#include "map/unit.hpp"

// Payon Stories rebalance: the stab sweeps a swath rather than a single file of
// cells. SPEARSTAB_LENGTH cells run from the target back toward the caster, and
// SPEARSTAB_WIDTH cells are covered to each side of that line (0 = the vanilla
// 1-cell-wide line). The side cells are offset along the perpendicular direction
// so every cell is visited exactly once - overlapping lookups would hit the same
// enemy twice.
#define SPEARSTAB_LENGTH 4
#define SPEARSTAB_WIDTH 1

SkillSpearStab::SkillSpearStab() : SkillImpl(KN_SPEARSTAB) {
}

void SkillSpearStab::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	dmg.blewcount = 0;
	// Payon Stories rebalance: one hit per enemy caught in the stab line, capped at 3.
	// The count is passed in through the low 12 bits of the skill_attack flag, so the
	// SD_ANIMATION bit set on the chained targets has to be masked back off here.
	int32 targets = dmg.miscflag & 0xFFF;
	if (targets > 1)
		dmg.div_ = min(3, targets);
}

void SkillSpearStab::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	if(flag&1) {
		if (target->id==skill_area_temp[1])
			return;
		if (skill_attack(BF_WEAPON,src,src,target,getSkillId(), skill_lv, tick, SD_ANIMATION|(skill_area_temp[0]&0xFFF)))
			skill_blown(src,target,skill_area_temp[2],-1,BLOWN_NONE);
	} else {
		int32 x=target->x,y=target->y,i,j,dir,perp;
		dir = map_calc_dir(target,src->x,src->y);
		perp = (dir + 2) & 7; // 90 degrees off the stab line
		skill_area_temp[1] = target->id;
		skill_area_temp[2] = skill_get_blewcount(getSkillId(),skill_lv);
		// Payon Stories rebalance: count every enemy standing in the swath first,
		// so each of them can be hit once per target caught (see modifyDamageData).
		skill_area_temp[0] = 0;
		for (i=0;i<SPEARSTAB_LENGTH;i++) {
			for (j=-SPEARSTAB_WIDTH;j<=SPEARSTAB_WIDTH;j++)
				skill_area_temp[0] += map_foreachincell(skill_area_sub,target->m,
					x+dirx[dir]*i+dirx[perp]*j,y+diry[dir]*i+diry[perp]*j,BL_CHAR,
					src, getSkillId(),skill_lv,tick,BCT_ENEMY,skill_area_sub_count);
		}
		skill_area_temp[0] = max(1, skill_area_temp[0]);
		// all the enemies between the caster and the target are hit, as well as the target
		if (skill_attack(BF_WEAPON,src,src,target, getSkillId(),skill_lv,tick,skill_area_temp[0]&0xFFF))
			skill_blown(src,target,skill_area_temp[2],-1,BLOWN_NONE);
		for (i=0;i<SPEARSTAB_LENGTH;i++) {
			for (j=-SPEARSTAB_WIDTH;j<=SPEARSTAB_WIDTH;j++)
				map_foreachincell(skill_area_sub,target->m,x+dirx[perp]*j,y+diry[perp]*j,BL_CHAR,
					src, getSkillId(),skill_lv,tick,flag|BCT_ENEMY|1,skill_castend_damage_id);
			x += dirx[dir];
			y += diry[dir];
		}
	}
}

void SkillSpearStab::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += 20 * skill_lv;
}
