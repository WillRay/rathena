// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "spiralpierce.hpp"

#include <config/core.hpp>

#include "map/map.hpp"
#include "map/mob.hpp"
#include "map/pc.hpp"
#include "map/status.hpp"

SkillSpiralPierce::SkillSpiralPierce() : WeaponSkillImpl(LK_SPIRALPIERCE) {
}

void SkillSpiralPierce::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd == nullptr)
		dmg.flag = (dmg.flag&~(BF_RANGEMASK|BF_WEAPONMASK))|BF_LONG|BF_MISC;

#ifndef RENEWAL
	// Payon Stories Spear Knight rework: Spiral Pierce inherits Pierce's
	// size-rule hit count (Small 2 / Medium 3 / Large 4) applied to each
	// target struck along the line.
	const status_data* tstatus = status_get_status_data(target);
	dmg.div_ = (dmg.div_ > 0 ? tstatus->size + 2 : -(tstatus->size + 2));
#endif
}

void SkillSpiralPierce::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &skillratio, int32 mflag) const {
#ifdef RENEWAL
	const status_change *sc = status_get_sc(src);

	skillratio += 50 + 50 * skill_lv;
	RE_LVL_DMOD(100);
	if (sc && sc->getSCE(SC_CHARGINGPIERCE_COUNT) && sc->getSCE(SC_CHARGINGPIERCE_COUNT)->val1 >= 10)
		skillratio *= 2;
#else
	// Payon Stories Spear Knight rework: small bump over Pierce since this is
	// the 2nd-class evolution and now also pays for the line targeting.
	skillratio += 50 + 30 * skill_lv;
#endif
}

#ifndef RENEWAL
void SkillSpiralPierce::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Payon Stories Spear Knight rework: pierce through up to 3 targets in a
	// straight line from caster to primary target. Reuses Brandish Spear's
	// line iterator pattern; each target hit gets the full Pierce treatment.
	if (flag & 1) {
		skill_attack(BF_WEAPON, src, src, target, getSkillId(), skill_lv, tick, flag);
	} else {
		map_foreachindir(skill_area_sub, src->m, src->x, src->y, target->x, target->y,
			0 /* range */, 3 /* length */, 0 /* offset */, splash_target(src),
			src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1,
			skill_castend_damage_id);
	}
}
#endif

void SkillSpiralPierce::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	map_session_data *dstsd = BL_CAST(BL_PC, target);
	mob_data* dstmd = BL_CAST(BL_MOB, target);

	if( dstsd || ( dstmd && !status_bl_has_mode(target,MD_STATUSIMMUNE) ) ) //Does not work on status immune
		sc_start(src,target,SC_ANKLE,100,0,skill_get_time2(getSkillId(),skill_lv));
}

void SkillSpiralPierce::modifyElement(const Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv, int32& element, int32 flag) const {
	if (src.type != BL_PC)
		element = ELE_NEUTRAL; // forced neutral for monsters
}
