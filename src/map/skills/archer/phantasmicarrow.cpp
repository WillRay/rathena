// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "phantasmicarrow.hpp"

#include <config/core.hpp>

#include "map/clif.hpp"
#include "map/pc.hpp"
#include "map/skill.hpp"

SkillPhantasmicArrow::SkillPhantasmicArrow() : WeaponSkillImpl(HT_PHANTASMIC) {
}

void SkillPhantasmicArrow::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += 400;
#else
	// Serrated Shot: 300% ATK at Lv1 scaling linearly to 650% ATK at Lv10.
	// base_skillratio starts at 100, so the addition is the displayed ATK% minus 100.
	base_skillratio += 200 + (350 * (skill_lv - 1)) / 9;
	// Master Fletching (AC_MAKINGARROW) passive: +10% damage per skill level.
	if (const map_session_data* sd = BL_CAST(BL_PC, src); sd != nullptr) {
		if (uint8 mf = pc_checkskill(sd, AC_MAKINGARROW); mf > 0)
			base_skillratio += 10 * mf;
	}
#endif
}

void SkillPhantasmicArrow::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Perform the normal Serrated Shot hit first (this also fires applyAdditionalEffects).
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);

#ifndef RENEWAL
	// Each Serrated Shot hit shaves 2.5s off Wyvern Bolt's (HT_POWER) remaining
	// cooldown - only if Wyvern Bolt is currently cooling down.
	if (map_session_data* sd = BL_CAST(BL_PC, src); sd != nullptr)
		skill_reduce_cooldown(*sd, HT_POWER, 2500);
#endif
}

void SkillPhantasmicArrow::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	// Serrated Shot: show the Red Hit visual effect on any target it hits.
	clif_specialeffect(target, EF_RED_HIT, AREA);
}
