// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "sonicimpact.hpp"

#include <config/core.hpp>

#include "map/clif.hpp"
#include "map/map.hpp"
#include "map/pc.hpp"
#include "map/status.hpp"

SkillSonicImpact::SkillSonicImpact() : WeaponSkillImpl(AS_SONICACCEL) {
}

void SkillSonicImpact::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	// 400% at Lv1, +200% per level to 1200% at Lv5, spread over 7 hits.
	skillratio += 100 + 200 * skill_lv;
}

void SkillSonicImpact::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	map_session_data* sd = BL_CAST( BL_PC, src );

	uint8 dir = DIR_NORTHEAST;

	if (target->x != src->x || target->y != src->y)
		dir = map_calc_dir(target, src->x, src->y);	// dir based on target as we move player based on target location

	if (skill_check_unit_movepos(0, src, target->x + dirx[dir], target->y + diry[dir], 1, 1)) {
		clif_blown(src);

		// Visual only: play Cross Impact's animation, no damage number attached.
		// Must be sent BEFORE the name announcement below - the client keys both
		// the pose/effect it plays AND the floating "<Name> !!" text off the most
		// recently received skill packet's id, so there is no packet that carries
		// one id's effect and another id's name.
		clif_skill_damage(*src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, GC_CROSSIMPACT, skill_lv, DMG_SINGLE);

		// Announce the real name, "Sonic Impact !!", using the actual
		// AS_SONICACCEL id. Sent last so it - not Cross Impact - is what's shown.
		clif_skill_nodamage(src, *target, AS_SONICACCEL, skill_lv);

		WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
	} else {
		if (sd)
			clif_skill_fail( *sd, getSkillId(), USESKILL_FAIL );
	}
}
