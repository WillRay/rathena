// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "spearboomerang.hpp"

#include <config/core.hpp>

#include "map/pc.hpp"
#include "map/status.hpp"

SkillSpearBoomerang::SkillSpearBoomerang() : WeaponSkillImpl(KN_SPEARBOOMERANG) {
}

void SkillSpearBoomerang::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += 50 * skill_lv;

#ifndef RENEWAL
	// Payon Stories Spear Knight rework: heavy spears throw harder.
	// Small coefficient so this rewards heavy spears without trivializing
	// the skill at low weight. e.g. a 1500-weight 2H spear adds +37 ratio.
	const map_session_data* sd = BL_CAST(BL_PC, src);
	if (sd != nullptr) {
		int16 idx = sd->equip_index[EQI_HAND_R];
		if (idx >= 0 && sd->inventory_data[idx] != nullptr)
			base_skillratio += sd->inventory_data[idx]->weight / 40;
	}
#endif
}

#ifndef RENEWAL
void SkillSpearBoomerang::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	// Payon Stories Spear Knight rework: mark the target as Exposed for 4s.
	// While Exposed, spear-weapon attacks deal +20% damage to the target.
	// Non-stacking, no refresh — throw once, follow up once.
	sc_start(src, target, SC_EXPOSED, 100, 0, 4000);
}
#endif
