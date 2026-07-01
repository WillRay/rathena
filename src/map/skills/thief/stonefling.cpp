// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "stonefling.hpp"

#include "map/clif.hpp"
#include "map/pc.hpp"
#include "map/status.hpp"

SkillStoneFling::SkillStoneFling() : SkillImpl(TF_THROWSTONE) {
}

void SkillStoneFling::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	map_session_data *sd = BL_CAST(BL_PC, src);
	if (sd != nullptr) {
#ifndef RENEWAL
		// Stone economy redesign: the Requires: Status gate in skill_db already
		// guarantees the caster holds a stone to reach this point (and no SP is
		// spent otherwise). Consume the stone and stun the target for 2s
		// (Duration1) at a guaranteed rate — the stone is the cost, so the CC is
		// unconditional and bypasses status resistance.
		status_change *sc = status_get_sc(src);
		if (sc != nullptr && sc->getSCE(SC_PICK_STONE) != nullptr) {
			status_change_end(src, SC_PICK_STONE);
			status_change_start(src, target, SC_STUN, 10000, skill_lv, 0, 0, 0,
				skill_get_time(getSkillId(), skill_lv), SCSTART_NOAVOID | SCSTART_NORATEDEF | SCSTART_NOTICKDEF);
		}
#else
		// Only blind if used by player and stun failed
		if (!sc_start(src, target, SC_STUN, 3, skill_lv, skill_get_time(getSkillId(), skill_lv)))
			sc_start(src, target, SC_BLIND, 3, skill_lv, skill_get_time2(getSkillId(), skill_lv));
#endif
	} else {
		// 5% stun chance and no blind chance when used by monsters
		sc_start(src, target, SC_STUN, 5, skill_lv, skill_get_time(getSkillId(), skill_lv));
	}
}

void SkillStoneFling::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 &flag) const {
	skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag);
}
