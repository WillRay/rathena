// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "phantasmicarrow.hpp"

#include <config/core.hpp>
#include <common/random.hpp>

#include "map/clif.hpp"
#include "map/pc.hpp"
#include "map/skill.hpp"

SkillPhantasmicArrow::SkillPhantasmicArrow() : WeaponSkillImpl(HT_PHANTASMIC) {
}

void SkillPhantasmicArrow::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
#ifdef RENEWAL
	base_skillratio += 400;
#else
	// Serrated Shot: 300% ATK at Lv1, +50% per level up to 700% at Lv9, then 800% at Lv10.
	// base_skillratio starts at 100, so the addition is the displayed ATK% minus 100.
	base_skillratio += (skill_lv < 10 ? 200 + 50 * (skill_lv - 1) : 700);
#endif
}

void SkillPhantasmicArrow::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Perform the normal Serrated Shot hit first (this also fires applyAdditionalEffects).
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);

#ifndef RENEWAL
	// Master Fletching passive: each Serrated Shot cast has a 5% per skill level chance
	// to shave 5s off Wyvern Bolt's (HT_POWER) remaining cooldown - only if Wyvern Bolt
	// is currently cooling down.
	if (map_session_data* sd = BL_CAST(BL_PC, src); sd != nullptr) {
		if (uint8 mf = pc_checkskill(sd, AC_MAKINGARROW); mf > 0 && rnd_chance(5 * mf, 100))
			skill_reduce_cooldown(*sd, HT_POWER, 5000);
	}
#endif
}

void SkillPhantasmicArrow::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	// Serrated Shot: show the Red Hit visual effect on any target it hits.
	clif_specialeffect(target, EF_RED_HIT, AREA);
}
