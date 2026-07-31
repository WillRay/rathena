// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "pierce.hpp"

#include "map/clif.hpp"
#include "map/status.hpp"

SkillPierce::SkillPierce() : WeaponSkillImpl(KN_PIERCE) {
}

void SkillPierce::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_data* tstatus = status_get_status_data(target);

	dmg.div_= (dmg.div_> 0 ? tstatus->size+2 : -(tstatus->size+2));

	// Pierce lands 2-4 hits depending on target size, so it must advertise itself
	// as a multi-hit packet (same as TF_DOUBLE / GS_CHAINACTION). Without this the
	// type stays DMG_NORMAL, and because the skill has NK_CRITICAL the crit branch
	// in battle_calc_weapon_attack picks DMG_CRITICAL (a single-hit packet type)
	// while div_ is still 2-4. The client cannot render that combination, so a
	// critical Pierce showed no damage and no animation at all.
	dmg.type = DMG_MULTI_HIT;
}

void SkillPierce::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_change* sc = status_get_sc(src);

	base_skillratio += 10 * skill_lv;

	if (sc && sc->getSCE(SC_CHARGINGPIERCE_COUNT) && sc->getSCE(SC_CHARGINGPIERCE_COUNT)->val1 >= 10)
		base_skillratio *= 2;
}

void SkillPierce::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 5 * skill_lv / 100;
}
