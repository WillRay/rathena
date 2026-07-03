// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "chargearrow.hpp"

SkillChargeArrow::SkillChargeArrow() : WeaponSkillImpl(AC_CHARGEARROW)
{
}

void SkillChargeArrow::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const
{
#ifndef RENEWAL
	// Payon Stories rebalance: 300% + 20% per level (320% at Lv1, 400% at Lv5).
	base_skillratio += 200 + 20 * skill_lv;
#else
	base_skillratio += 50;
#endif
}
