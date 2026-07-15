// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "bowlingbash.hpp"

#include <config/core.hpp>

#include <common/db.hpp>

#include "map/battle.hpp"
#include "map/status.hpp"
#include "map/unit.hpp"

SkillBowlingBash::SkillBowlingBash() : SkillImpl(KN_BOWLINGBASH) {
}

void SkillBowlingBash::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
#ifdef RENEWAL
	const map_session_data* sd = BL_CAST(BL_PC, &src);

	if (sd != nullptr && sd->status.weapon == W_2HSWORD) {
		if (dmg.miscflag >= 4)
			dmg.div_ = 4;
		else if (dmg.miscflag >= 2)
			dmg.div_ = 3;
	}
#else
	// Payon Stories rebalance: single heavy hit (knockback handled by the
	// splash/chain pinball in castendDamageId, so no default knockback here)
	dmg.div_ = 1;
	dmg.blewcount = 0;
#endif
}

void SkillBowlingBash::calculateSkillRatio(const Damage* wd, const block_list* src, const block_list* target, uint16 skill_lv, int32& base_skillratio, int32 mflag) const {
	base_skillratio += 30 * skill_lv;
#ifndef RENEWAL
	// Knight rebalance: consume up to 5 Momentum stacks (built by Two-Hand
	// Quicken) for +60% ATK each, +300% at 5 stacks - same total ceiling as
	// before, reached in half the stacks now that Bowling Bash's cooldown is
	// no longer linked to Sundering Strike's. Read the live stack count here -
	// the whole splash/chain cascade runs synchronously, so every hit sees the
	// same value; the stacks are consumed once at the end of the initial cast
	// in castendDamageId below.
	const status_change* sc = status_get_sc(src);

	if (sc && sc->getSCE(SC_MOMENTUM))
		base_skillratio += 60 * min(5, sc->getSCE(SC_MOMENTUM)->val1);
#endif
}

void SkillBowlingBash::castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
#ifdef RENEWAL
	if (flag & 1) {
		skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, (skill_area_temp[0]) > 0 ? SD_ANIMATION | skill_area_temp[0] : skill_area_temp[0]);
	} else {
		skill_area_temp[0] = map_foreachinallrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR, src, getSkillId(), skill_lv, tick, BCT_ENEMY, skill_area_sub_count);
		map_foreachinrange(skill_area_sub, target, skill_get_splash(getSkillId(), skill_lv), BL_CHAR|BL_SKILL, src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | SD_SPLASH | 1, skill_castend_damage_id);
	}
#else
	int32 min_x,max_x,min_y,max_y,i,c,dir,tx,ty;
	// Chain effect and check range gets reduction by recursive depth, as this can reach 0, we don't use blowcount
	c = (skill_lv-(flag&0xFFF)+1)/2;
	// Determine the Bowling Bash area depending on configuration
	if (battle_config.bowling_bash_area == 0) {
		// Gutter line system
		min_x = ((src->x)-c) - ((src->x)-c)%40;
		if(min_x < 0) min_x = 0;
		max_x = min_x + 39;
		min_y = ((src->y)-c) - ((src->y)-c)%40;
		if(min_y < 0) min_y = 0;
		max_y = min_y + 39;
	} else if (battle_config.bowling_bash_area == 1) {
		// Gutter line system without demi gutter bug
		min_x = src->x - (src->x)%40;
		max_x = min_x + 39;
		min_y = src->y - (src->y)%40;
		max_y = min_y + 39;
	} else {
		// Area around caster
		min_x = src->x - battle_config.bowling_bash_area;
		max_x = src->x + battle_config.bowling_bash_area;
		min_y = src->y - battle_config.bowling_bash_area;
		max_y = src->y + battle_config.bowling_bash_area;
	}
	// Initialization, break checks, direction
	if((flag&0xFFF) > 0) {
		// Ignore monsters outside area
		if(target->x < min_x || target->x > max_x || target->y < min_y || target->y > max_y)
			return;
		// Ignore monsters already in list
		if(idb_exists(bowling_db, target->id))
			return;
		// Random direction
		dir = rnd()%8;
	} else {
		// Create an empty list of already hit targets
		db_clear(bowling_db);
		// Direction is walkpath
		dir = (unit_getdir(src)+4)%8;
	}
	// Add current target to the list of already hit targets
	idb_put(bowling_db, target->id, target);
	// Keep moving target in direction square by square
	tx = target->x;
	ty = target->y;
	for(i=0;i<c;i++) {
		// Target coordinates (get changed even if knockback fails)
		tx -= dirx[dir];
		ty -= diry[dir];
		// If target cell is a wall then break
		if(map_getcell(target->m,tx,ty,CELL_CHKWALL))
			break;
		skill_blown(src,target,1,dir,BLOWN_NONE);

		int32 count;

		// Splash around target cell, but only cells inside area; we first have to check the area is not negative
		if((max(min_x,tx-1) <= min(max_x,tx+1)) &&
			(max(min_y,ty-1) <= min(max_y,ty+1)) &&
			(count = map_foreachinallarea(skill_area_sub, target->m, max(min_x,tx-1), max(min_y,ty-1), min(max_x,tx+1), min(max_y,ty+1), splash_target(src), src, getSkillId(), skill_lv, tick, flag|BCT_ENEMY, skill_area_sub_count))) {
			// Recursive call
			map_foreachinallarea(skill_area_sub, target->m, max(min_x,tx-1), max(min_y,ty-1), min(max_x,tx+1), min(max_y,ty+1), splash_target(src), src, getSkillId(), skill_lv, tick, (flag|BCT_ENEMY)+1, skill_castend_damage_id);
			// Self-collision
			if(target->x >= min_x && target->x <= max_x && target->y >= min_y && target->y <= max_y)
				skill_attack(BF_WEAPON,src,src,target,getSkillId(),skill_lv,tick,(flag&0xFFF)>0?SD_ANIMATION|count:count);
			break;
		}
	}
	// Original hit or chain hit depending on flag
	skill_attack(BF_WEAPON,src,src,target,getSkillId(),skill_lv,tick,(flag&0xFFF)>0?SD_ANIMATION:0);

	// Knight rebalance: consume Momentum only on the initial cast (flag depth
	// 0), after every splash/chain hit above has already read the live stack
	// count in calculateSkillRatio. Recursive chain calls carry a non-zero
	// depth and must not consume. Only up to 5 stacks are spent - anything
	// beyond that is re-applied instead of being discarded with the rest of
	// the buff (same partial-consumption pattern as Sundering Strike).
	if ((flag&0xFFF) == 0) {
		status_change* sc = status_get_sc(src);

		if (sc && sc->getSCE(SC_MOMENTUM)) {
			int32 total_stacks = sc->getSCE(SC_MOMENTUM)->val1;
			int32 remaining = total_stacks - min(5, total_stacks);

			status_change_end(src, SC_MOMENTUM);
			if (remaining > 0)
				sc_start(src, src, SC_MOMENTUM, 100, remaining, 10000);
		}
	}
#endif
}
