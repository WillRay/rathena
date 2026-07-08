// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "beaststrafing.hpp"

#include "map/clif.hpp"
#include "map/map.hpp"
#include "map/skill.hpp"
#include "map/status.hpp"

SkillBeastStrafing::SkillBeastStrafing() : SkillImpl(HT_POWER) {
}

void SkillBeastStrafing::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Wyvern Bolt: a piercing line attack. The bolt strikes the target and every
	// enemy standing on the straight line from the caster through the target,
	// continuing up to 2 cells past the target.

	// Visual only: make the client play Crescive Bolt's projectile animation toward the
	// primary target (DMGVAL_IGNORE hides the damage number) before dealing the real damage.
	clif_skill_damage(*src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, WH_CRESCIVE_BOLT, skill_lv, DMG_SINGLE);

	// skill_area_temp[1] marks the primary target so it takes a full (non-splash) hit.
	skill_area_temp[1] = target->id;

	int32 length = distance_bl(src, target) + 2;
	map_foreachinpath(skill_attack_area, src->m, src->x, src->y, target->x, target->y,
		skill_get_splash(getSkillId(), skill_lv), length, splash_target(src),
		skill_get_type(getSkillId()), src, src, getSkillId(), skill_lv, tick, flag, BCT_ENEMY);
}

void SkillBeastStrafing::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	// 400% ATK + 100% ATK per skill level (base_skillratio starts at 100).
	base_skillratio += 300 + 100 * skill_lv;

	// Distance bonus: the bolt gains power as it flies, dealing up to +50% more
	// damage at the edge of its range. Scales linearly with each target's distance
	// from the caster, so enemies further down the line are hit harder.
	int32 range = skill_get_range2(src, getSkillId(), skill_lv, true);

	if (range > 0) {
		int32 dist = distance_bl(src, target);

		if (dist > range)
			dist = range;

		base_skillratio += base_skillratio * 50 * dist / range / 100;
	}
}
