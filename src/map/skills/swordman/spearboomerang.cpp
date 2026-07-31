// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "spearboomerang.hpp"

#include "map/map.hpp"
#include "map/skill.hpp"
#include "map/status.hpp"

SkillSpearBoomerang::SkillSpearBoomerang() : WeaponSkillImpl(KN_SPEARBOOMERANG) {
}

void SkillSpearBoomerang::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += 50 * skill_lv;

	// Distance bonus: the further the boomerang has to travel to reach its target,
	// the harder it hits, scaling linearly up to double damage at max range.
	int32 range = skill_get_range2(src, getSkillId(), skill_lv, true);

	if (range > 0) {
		int32 dist = distance_bl(src, target);

		if (dist > range)
			dist = range;

		base_skillratio += base_skillratio * dist / range;
	}
}

void SkillSpearBoomerang::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	sc_start(src, target, SC_SILENCE, 100, skill_lv, 5000);
}
