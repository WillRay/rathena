// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "venomdust.hpp"


SkillVenomDust::SkillVenomDust() : SkillImpl(AS_VENOMDUST) {
}

void SkillVenomDust::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
#ifndef RENEWAL
	// Payon Stories rebalance: the poison cloud now deals a direct weapon-damage
	// tick each interval (50% + 10% per level) so it contributes on
	// poison-immune MVPs. The poison status is kept as flavor.
	base_skillratio += -50 + 10 * skill_lv;
#endif
}

void SkillVenomDust::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	flag|=1;//Set flag to 1 to prevent deleting ammo (it will be deleted on group-delete).
	skill_unitsetting(src,getSkillId(),skill_lv,x,y,0);
}
