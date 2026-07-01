// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "sandattack.hpp"

#include "map/pc.hpp"
#include "map/status.hpp"

SkillSandAttack::SkillSandAttack() : WeaponSkillImpl(TF_SPRINKLESAND) {
}

void SkillSandAttack::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	base_skillratio += 30;
}

void SkillSandAttack::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	map_session_data *sd = BL_CAST(BL_PC, src);
	if (sd != nullptr) {
#ifndef RENEWAL
		// Stone economy redesign: the Requires: Status gate in skill_db already
		// guarantees the caster holds a stone to reach this point (and no SP is
		// spent otherwise). Consume the stone and blind the target for 30s
		// (Duration2) at a guaranteed 100% rate, bypassing status resistance —
		// the stone is the cost, so this is the reliable-CC option opposite
		// Stone Fling's stun.
		status_change *sc = status_get_sc(src);
		if (sc != nullptr && sc->getSCE(SC_PICK_STONE) != nullptr) {
			status_change_end(src, SC_PICK_STONE);
			status_change_start(src, target, SC_BLIND, 10000, skill_lv, 0, 0, 0,
				skill_get_time2(getSkillId(), skill_lv), SCSTART_NOAVOID | SCSTART_NORATEDEF | SCSTART_NOTICKDEF);
		}
#else
		sc_start(src, target, SC_BLIND, 20, skill_lv, skill_get_time2(getSkillId(), skill_lv));
#endif
	} else {
		// Monster cast: original partial blind chance, no stone economy
		sc_start(src, target, SC_BLIND, 15, skill_lv, skill_get_time2(getSkillId(), skill_lv));
	}
}
