// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "endure.hpp"

#include "map/clif.hpp"
#include "map/status.hpp"

SkillEndure::SkillEndure() : SkillImpl(SM_ENDURE) {
}

void SkillEndure::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Skill was compressed from 10 levels to 5 with the same MDEF bonus at max level,
	// so the MDEF-carrying status level is doubled while duration still comes from skill_db per the real skill_lv.
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start(src, target, skill_get_sc(getSkillId()), 100, skill_lv * 2, skill_get_time(getSkillId(), skill_lv)));
}
