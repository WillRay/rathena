// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "sandman.hpp"

#include "map/skill.hpp"
#include "map/status.hpp"

SkillSandman::SkillSandman() : WeaponSkillImpl(HT_SANDMAN) {
}

// Tranquillizing Dart is set up in the skill DB as a normal targeted attack skill
// (TargetType Attack), so the cast routes straight to castendDamageId against the
// enemy the player selected - the inherited WeaponSkillImpl::castendDamageId performs
// the skill_attack, which shows the arrow and then triggers applyAdditionalEffects.
// (The effect only fires on a hit that lands: skill_additional_effect returns early
// on dmg_lv < ATK_DEF, which is why the dart deals damage instead of being NoDamage.)

void SkillSandman::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	// The value of this skill is the sleep, not the damage - a light poke so the arrow
	// connects and the sleep lands.
	base_skillratio -= 99; // ~1% ATK
}

void SkillSandman::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	// Put the target to sleep for the skill's time2 (15s). Status-immune targets
	// (e.g. bosses) shrug it off.
	if (status_bl_has_mode(target, MD_STATUSIMMUNE))
		return;

	sc_start(src, target, SC_SLEEP, 100, skill_lv, skill_get_time2(getSkillId(), skill_lv));
}
