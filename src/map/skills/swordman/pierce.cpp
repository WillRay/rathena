// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "pierce.hpp"

#include "map/status.hpp"

SkillPierce::SkillPierce() : WeaponSkillImpl(KN_PIERCE) {
}

void SkillPierce::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	const status_data* tstatus = status_get_status_data(target);

#ifdef RENEWAL
	dmg.div_= (dmg.div_> 0 ? tstatus->size+1 : -(tstatus->size+1));
#else
	// Payon Stories Spear Knight rework: +1 hit at every size
	// Small: 2 hits, Medium: 3 hits, Large: 4 hits
	dmg.div_= (dmg.div_> 0 ? tstatus->size+2 : -(tstatus->size+2));
#endif
}

void SkillPierce::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	const status_change* sc = status_get_sc(src);

#ifdef RENEWAL
	base_skillratio += 10 * skill_lv;
#else
	// Payon Stories Spear Knight rework: stronger thrust to compensate for the
	// extra hit on each size and to give Pierce a real damage floor against
	// the high-DEF targets Bowling Bash can't reach.
	base_skillratio += 25 * skill_lv;
#endif

	if (sc && sc->getSCE(SC_CHARGINGPIERCE_COUNT) && sc->getSCE(SC_CHARGINGPIERCE_COUNT)->val1 >= 10)
		base_skillratio *= 2;
}

void SkillPierce::modifyHitRate(int16& hit_rate, const block_list* src, const block_list* target, uint16 skill_lv) const {
	hit_rate += hit_rate * 5 * skill_lv / 100;
}
