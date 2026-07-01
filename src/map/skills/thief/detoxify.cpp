// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "detoxify.hpp"

#include "map/clif.hpp"
#include "map/status.hpp"

SkillDetoxify::SkillDetoxify() : SkillImpl(TF_DETOXIFY) {
}

void SkillDetoxify::castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32 &flag) const {
	clif_skill_nodamage(src, *bl, getSkillId(), skill_lv);

	status_change *tsc = status_get_sc(bl);
	bool had_poison = tsc && (tsc->getSCE(SC_POISON) || tsc->getSCE(SC_DPOISON));

	status_change_end(bl, SC_POISON);
	status_change_end(bl, SC_DPOISON);

#ifndef RENEWAL
	if (had_poison) {
		status_data *tstatus = status_get_status_data(*bl);
		int32 heal_amount = tstatus->max_hp / 10;

		clif_skill_nodamage(nullptr, *bl, AL_HEAL, heal_amount);
		status_heal(bl, heal_amount, 0, 0);
	}
#endif
}
