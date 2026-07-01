// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "grimtooth.hpp"

#include "map/mob.hpp"
#include "map/status.hpp"

SkillGrimtooth::SkillGrimtooth() : SkillImplRecursiveDamageSplash(AS_GRIMTOOTH) {
}

void SkillGrimtooth::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
#ifndef RENEWAL
	// Payon Stories rebalance: Grimtooth is the cloak-gated stealth burst
	// window. Real damage floor of 300% + 50% per level (550% at level 5).
	base_skillratio += 200 + 50 * skill_lv;
#else
	base_skillratio += 20 * skill_lv;
#endif
}

void SkillGrimtooth::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	status_change* sc = status_get_sc(src);

	// Thief passive rebalance: Grimtooth can be opened from Hiding OR with a
	// banked Opportunist charge (the castbegin gate in skill.cpp allows both).
	// A cast made from Hiding keeps its full behavior — recursive splash plus
	// the guaranteed hit (IgnoreFlee) from the skill DB. A cast made purely on
	// an Opportunist charge (not in Hiding) consumes that charge and lands as a
	// single-target strike with no splash; the DB IgnoreFlee flag still makes
	// it a guaranteed hit.
	if (sc != nullptr && sc->getSCE(SC_HIDING) == nullptr && sc->getSCE(SC_OPPORTUNIST) != nullptr) {
		status_change_end(src, SC_OPPORTUNIST);
		skill_attack(BF_WEAPON, src, src, target, getSkillId(), skill_lv, tick, flag);
		return;
	}

	flag |= SD_PREAMBLE; // a fake packet will be sent for the first target to be hit

	SkillImplRecursiveDamageSplash::castendDamageId(src, target, skill_lv, tick, flag);
}

void SkillGrimtooth::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	status_data* tstatus = status_get_status_data(*target);
	mob_data* dstmd = BL_CAST(BL_MOB, target);

	if (dstmd && !status_has_mode(tstatus,MD_STATUSIMMUNE))
		sc_start(src,target,SC_QUAGMIRE,100,0,skill_get_time2(getSkillId(),skill_lv));
}
