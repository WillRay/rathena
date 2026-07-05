// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "detect.hpp"

#include "map/battle.hpp"
#include "map/skill.hpp"
#include "map/status.hpp"

SkillDetect::SkillDetect() : SkillImpl(HT_DETECTING) {
}

void SkillDetect::castendPos2(block_list* src, int32 x, int32 y, uint16 skill_lv, t_tick tick, int32& flag) const {
	int32 i = skill_get_splash(getSkillId(), skill_lv);

	// Reveal hidden units and traps in the struck area (the skill's original
	// utility, kept intact).
	map_foreachinallarea( status_change_timer_sub,
		src->m, x-i, y-i, x+i,y+i,BL_CHAR,
		src,nullptr,SC_SIGHT,tick);
	skill_reveal_trap_inarea(src, i, x, y);

	// Hunter rebalance: Detecting is now an offensive falcon strike. Hit every
	// enemy in the 9x9 area with a single Misc hit routed back through
	// skill_castend_damage_id -> castendDamageId below. Damage is computed in
	// battle_calc_misc_attack; Fear and the Hunted mark are applied per target in
	// applyAdditionalEffects.
	map_foreachinallarea( skill_area_sub,
		src->m, x-i, y-i, x+i, y+i, BL_CHAR,
		src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1,
		skill_castend_damage_id );
}

void SkillDetect::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	skill_attack(BF_MISC, src, src, target, getSkillId(), skill_lv, tick, flag);
}

void SkillDetect::applyAdditionalEffects(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	// Inflict Fear for a flat 2 seconds on every enemy struck. NoTickDef keeps the
	// duration fixed at 2s regardless of the target's INT/LUK/level resistance,
	// so it lands as designed on high-stat monsters.
	status_change_start(src, target, SC_FEAR, 10000, skill_lv, 0, 0, 0, 2000, SCSTART_NOTICKDEF);

	// Stamp the Hunted mark (7s) so a following Hunter skill draws the falcon
	// assist / empowered Blitz Beat, matching the Beast Bane mark system.
	sc_start(src, target, SC_HUNTED, 100, skill_lv, 7000);
}
