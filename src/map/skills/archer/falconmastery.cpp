// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "falconmastery.hpp"

#include "map/clif.hpp"
#include "map/pc.hpp"

SkillFalconMastery::SkillFalconMastery() : SkillImpl(HT_FALCON) {
}

void SkillFalconMastery::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	if (sd) {
		// Can't summon a falcon while a warg is out (matches the Falcon Flute behavior).
		if (!pc_iswug(sd) && !pc_isridingwug(sd))
			pc_setfalcon(sd, !pc_isfalcon(sd));
		clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	}
}
