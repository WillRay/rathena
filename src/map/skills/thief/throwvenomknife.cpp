// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "throwvenomknife.hpp"

#include <config/core.hpp>

#include "map/clif.hpp"
#include "map/status.hpp"

SkillThrowVenomKnife::SkillThrowVenomKnife() : SkillImplRecursiveDamageSplash(AS_VENOMKNIFE) {
}

void SkillThrowVenomKnife::calculateSkillRatio(const Damage *wd, const block_list *src, const block_list *target, uint16 skill_lv, int32 &base_skillratio, int32 mflag) const {
	// Assassin Cross rebalance: Whirling Knives, a Dancing-Knife-style dagger stance.
	// 50 / 70 / 90 / 110 / 130% ATK per volley.
	base_skillratio += -100 + 30 + 20 * skill_lv;
}

void SkillThrowVenomKnife::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	sc_type type = skill_get_sc(getSkillId());

	if (flag & 1) {
		skill_area_temp[1] = 0;

		// Note: doesn't force player to stand before attacking
		map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR | BL_SKILL, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_LEVEL | SD_SPLASH, skill_castend_damage_id);
	} else {
		t_tick duration = skill_get_time(getSkillId(), skill_lv);
		status_change *sc = status_get_sc(src);

		// Thief passive rebalance: a banked Opportunist charge (earned from a Sonic
		// Blow hit or leaving Hiding) is spent here to extend the knife stance from
		// 10s to 15s, raising uptime from ~33% to ~50%. Competes with Grimtooth for
		// the same charge. Damage is untouched so the bonus does not multiply with EDP.
		if (sc != nullptr && sc->getSCE(SC_OPPORTUNIST) != nullptr) {
			status_change_end(src, SC_OPPORTUNIST);
			duration += 5000;
		}

		clif_skill_nodamage(src, *target, getSkillId(), skill_lv, sc_start(src, target, type, 100, skill_lv, duration));
	}
}
