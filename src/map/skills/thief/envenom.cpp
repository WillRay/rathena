// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "envenom.hpp"

#include "map/clif.hpp"
#include "map/pc.hpp"
#include "map/status.hpp"
#include "map/unit.hpp"

SkillEnvenom::SkillEnvenom() : WeaponSkillImpl(TF_POISON) {
}

void SkillEnvenom::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Capture whether the caster was auto-attacking before the skill cast cleared the target.
	// unit_stop_attack() (called when the skill begins) nulls the target but leaves attack_continue set.
	unit_data* ud = unit_bl2ud(src);
	bool resume_attack = src->type == BL_PC && ud != nullptr && ud->state.attack_continue != 0;

	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);

	if (resume_attack && target != nullptr && !status_isdead(*target))
		unit_attack(src, target->id, 1);
}

void SkillEnvenom::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	map_session_data *sd = BL_CAST(BL_PC, src);
	if (!sc_start2(src, target, SC_POISON, (4 * skill_lv + 10), skill_lv, src->id, skill_get_time2(getSkillId(), skill_lv)) && sd)
		clif_skill_fail(*sd, getSkillId());
}
