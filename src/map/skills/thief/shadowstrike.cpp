// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "shadowstrike.hpp"

#include <config/core.hpp>

#include "map/clif.hpp"
#include "map/map.hpp"
#include "map/pc.hpp"
#include "map/status.hpp"

SkillShadowStrike::SkillShadowStrike() : WeaponSkillImpl(ASC_BREAKER) {
}

// Mobs/NPCs (Eremes Guile, Naght Sieger, Gang Member, ...) keep the old
// "Soul Destroyer" weapon-part ratio and single hit - only player casters get
// the reworked Shadow Strike numbers. The old skill's INT-based misc-damage
// half and its DEF-respecting weapon part are restored for non-player casters
// only via the ASC_BREAKER special cases in battle.cpp (the misc-damage
// splice in battle_calc_weapon_attack, the case in battle_calc_misc_attack,
// and the attack_ignores_def override).
//
// Pure physical, per-hit ratio for players (HitCount is positive in the db,
// so the ratio below applies per hit rather than being split across the
// total). 46% ATK per skill level per hit -> 460% per hit at Lv10, summing to
// 1380% across the 3 hits (a flat 15% bump over the original 1200% tune).
// Katar/Dagger no longer change the ratio or hit count - see
// applyAdditionalEffects for their on-hit status effects instead.
void SkillShadowStrike::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd == nullptr) {
		// Old Soul Destroyer weapon-part ratio (pre-renewal): 100% ATK per level.
		skillratio += -100 + 100 * skill_lv;
		return;
	}

	skillratio += -100 + 46 * skill_lv;
}

// Non-player casters keep the old single-hit HitCount instead of the new
// 3-hit db value.
void SkillShadowStrike::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd == nullptr)
		dmg.div_ = 1;
}

void SkillShadowStrike::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	// Non-player casters get the old Soul Destroyer cast: a plain ranged hit,
	// no dash and no stealth interaction.
	if (sd == nullptr) {
		WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
		return;
	}

	uint8 dir = DIR_NORTHEAST;	// up-right when src shares the target's cell

	if (target->x != src->x || target->y != src->y)
		dir = map_calc_dir(target, src->x, src->y);	// dir based on target as we move player based on target location

	// Close the gap where the map allows it. Cast range is 5, so this always
	// lands the caster adjacent to the target. On BG/WoE grounds the dash is
	// suppressed but the strike still connects at range.
	if (skill_check_unit_movepos(5, src, target->x + dirx[dir], target->y + diry[dir], 0, 1))
		clif_blown(src);

	status_change_end(src, SC_HIDING);
	status_change_end(src, SC_CLOAKING);

	// Visual only: play Shadow Stab's animation, no damage number attached.
	// Must be sent BEFORE the name announcement below - the client keys both
	// the pose/effect it plays AND the floating "<Name> !!" text off the most
	// recently received skill packet's id, so there is no packet that carries
	// one id's effect and another id's name (see SkillSonicImpact for the
	// same pattern).
	clif_skill_damage(*src, *target, tick, status_get_amotion(src), 0, DMGVAL_IGNORE, 1, SHC_SHADOW_STAB, skill_lv, DMG_SINGLE);

	// Announce the real name, "Shadow Strike !!", using the actual
	// ASC_BREAKER id. Sent last so it - not Shadow Stab - is what's shown.
	clif_skill_nodamage(src, *target, getSkillId(), skill_lv);
	WeaponSkillImpl::castendDamageId(src, target, skill_lv, tick, flag);
}

// Katars have a high chance to cause Bleeding; Daggers have a high chance to
// cause Blind instead. Player casters only - mobs keep the old Soul Destroyer
// hybrid with no weapon-specific proc (see calculateSkillRatio/modifyDamageData).
void SkillShadowStrike::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	const map_session_data* sd = BL_CAST(BL_PC, src);

	if (sd == nullptr)
		return;

	if (sd->status.weapon == W_KATAR)
		sc_start2(src, target, SC_BLEEDING, 60, skill_lv, src->id, 5000 + 500 * skill_lv);
	else if (sd->status.weapon == W_DAGGER)
		sc_start(src, target, SC_BLIND, 60, skill_lv, 5000 + 500 * skill_lv);
}
