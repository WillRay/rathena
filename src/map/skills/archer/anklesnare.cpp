// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "anklesnare.hpp"

#include "map/clif.hpp"
#include "map/skill.hpp"
#include "map/status.hpp"

SkillAnkleSnare::SkillAnkleSnare() : WeaponSkillImpl(HT_ANKLESNARE) {
}

// Snaring Arrow is set up in the skill DB as a normal targeted attack skill
// (TargetType Attack), so the cast routes straight to castendDamageId against the
// enemy the player selected. All this class has to do is the damage ratio and the
// root - the inherited WeaponSkillImpl::castendDamageId performs the skill_attack.

void SkillAnkleSnare::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	// A light hit - the value of Snaring Arrow is the root, not the damage.
	base_skillratio += 20 + 20 * skill_lv; // 140% at Lv1 -> 220% at Lv5
}

void SkillAnkleSnare::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	// Briefly root the target it hits. Status-immune targets (bosses) shrug it off.
	if (status_bl_has_mode(target, MD_STATUSIMMUNE))
		return;

	if (sc_start(src, target, SC_ANKLE, 100, 0, skill_get_time2(getSkillId(), skill_lv))) {
		// Show a persistent "bound" cage on the target so it is obvious it is
		// rooted; the matching removal fires when SC_ANKLE ends (status_change_end).
		clif_specialeffect(target, EF_NPC_STOP, AREA);
	}
}
