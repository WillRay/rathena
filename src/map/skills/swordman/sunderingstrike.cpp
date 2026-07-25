// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "sunderingstrike.hpp"

#include "map/clif.hpp"
#include "map/skill.hpp"
#include "map/status.hpp"

SkillSunderingStrike::SkillSunderingStrike() : WeaponSkillImpl(KN_ONEHAND) {
}

void SkillSunderingStrike::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	// Momentum rework: Sundering Strike no longer consumes Momentum stacks for
	// bonus damage - the old 5-stack ceiling (+250% ATK) is now baked in
	// unconditionally, delivered as a single heavy slam, single target.
	base_skillratio += 350 + 100 * skill_lv;
}

void SkillSunderingStrike::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Visual only: play Cart Termination's animation, no damage number attached.
	// This has to be sent BEFORE the name announcement below. The client keys
	// both the pose/effect it plays AND the floating "<Name> !!" text off the
	// most recently received skill packet's id - there is no way to send one
	// id's effect and another id's name in a single packet. Sending this one
	// first means it gets its moment on screen before being superseded.
	clif_skill_damage(*src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, WS_CARTTERMINATION, skill_lv, DMG_SINGLE);

	// Announce the real name, "Sundering Strike !!", using the actual
	// KN_ONEHAND id. Sent last so it - not Cart Termination - is what's shown
	// as the skill name. This does mean KN_ONEHAND's own registered cast
	// effect (a leftover from its old One-Hand Quicken buff visuals) briefly
	// flashes right after the Cart Termination animation above; there is no
	// packet that carries a name without also carrying an effect for that id.
	clif_skill_nodamage(src, *target, KN_ONEHAND, skill_lv);

	// The cast-at target always gets hit, regardless of where it stands -
	// this is a non-targeted skill that auto-fires on the current target, so
	// it is not guaranteed to be lined up with the caster's facing direction.
	skill_attack(BF_WEAPON, src, src, target, KN_ONEHAND, skill_lv, tick, flag);

	// The armor-crush debuff is the skill's signature effect: it lands on the
	// cast-at target on every cast. Single target only - no AoE spread.
	sc_start(src, target, SC_SUNDER, 100, 5 * skill_lv, skill_get_time(KN_ONEHAND, skill_lv));
}
