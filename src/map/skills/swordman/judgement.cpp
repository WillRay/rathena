// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "judgement.hpp"

#include <algorithm>

#include <config/core.hpp>

#include "map/clif.hpp"
#include "map/map.hpp"
#include "map/status.hpp"

// Crusader rebalance: Judgement (formerly Pressure / Gloria Domini).
//
// The instant burst is untouched engine behaviour - battle_calc_misc_attack sets
// md.damage = 500 + 300 * skill_lv and battle_calc_damage returns it past DEF, FLEE
// and every damage reduction. What this class adds is the follow-through: a Holy
// damage-over-time that carries the bulk of the skill, plus a Fear applied to the
// Undead and Demon bystanders around the target.
//
// The DoT total is a percentage of the caster's (ATK + MATK) per skill level. Because
// SC_JUDGEMENT_BURN cannot carry an element or route through the damage pipeline, the
// Holy attribute table, the caster's missing-HP bonus and the siege reduction are all
// applied ONCE here at cast time; the per-tick number stored in the status is final.
static constexpr int32 JUDGEMENT_DOT_PCT_PER_LV = 100;

// Ticks the burn fires over its duration. Must stay in step with Duration1 in
// db/import/skill_db.yml (5000ms) and the 1000ms interval in status_get_sc_interval.
static constexpr int32 JUDGEMENT_DOT_TICKS = 5;

SkillJudgement::SkillJudgement() : SkillImpl(PA_PRESSURE) {
}

void SkillJudgement::castendDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// Marks the primary target so castendNoDamageId can skip it: the target takes the
	// damage, the enemies around it take the Fear. Same idiom as TK_TURNKICK.
	skill_area_temp[1] = target->id;

	if (skill_attack(skill_get_type(getSkillId()), src, src, target, getSkillId(), skill_lv, tick, flag) > 0) {
		status_data *sstatus = status_get_status_data(*src);
		status_data *tstatus = status_get_status_data(*target);

		// batk + rhw.atk is the engine's scalar "caster ATK" (see battle_calc_misc_attack);
		// status_data::watk is only populated on the dual-wield split path. MATK is taken as
		// the midpoint of the min/max band so the skill lands a deterministic number.
		int32 atk = sstatus->batk + sstatus->rhw.atk;
		int32 matk = (sstatus->matk_min + sstatus->matk_max) / 2;

		int64 total = (int64)(atk + matk) * JUDGEMENT_DOT_PCT_PER_LV * skill_lv / 100;

		// Judgement rewards a Crusader who has already spent health - which is the state
		// Grand Cross leaves them in. +1% damage per 2% of MaxHP missing, so roughly +50%
		// at 1 HP and nothing at all at full. Deliberately a kicker, not the main lever.
		int32 missing_pct = 100 - (int32)(sstatus->hp * 100 / std::max<uint32>(sstatus->max_hp, 1));
		total = total * (100 + missing_pct / 2) / 100;

		// The burn is Holy. A status change carries no element, so the attribute table is
		// applied here and baked into the stored per-tick damage.
		total = battle_attr_fix(src, target, total, ELE_HOLY, tstatus->def_ele, tstatus->ele_lv);

		// Likewise for siege reduction: the ticks are dealt with status_fix_damage and so
		// never pass through battle_calc_damage, where this would normally be applied.
		map_data *mapdata = map_getmapdata(target->m);

		if (mapdata != nullptr) {
			// BF_SKILL matters: without it both helpers take their normal-attack branch and
			// scale by BF_SHORT/BF_LONG, which are not set here, so nothing would be reduced.
			if (mapdata_flag_gvg2(mapdata))
				total = battle_calc_gvg_damage(src, target, total, getSkillId(), BF_MISC | BF_SKILL);
			else if (mapdata->getMapFlag(MF_BATTLEGROUND))
				total = battle_calc_bg_damage(src, target, total, getSkillId(), BF_MISC | BF_SKILL);
		}

		// A target that shrugs off Holy entirely gets no burn at all, rather than a token
		// 1 damage per tick that would read as the skill half-working.
		if (total > 0) {
			int64 per_tick = std::max<int64>(total / JUDGEMENT_DOT_TICKS, 1);

			// val1 = final per-tick damage, val2 = caster GID so kills credit the Crusader.
			sc_start2(src, target, SC_JUDGEMENT_BURN, 100, (int32)per_tick, src->id,
			          skill_get_time(getSkillId(), skill_lv));
		}
	}

	map_foreachinallrange(skill_area_sub, target,
	                      skill_get_splash(getSkillId(), skill_lv), BL_CHAR,
	                      src, getSkillId(), skill_lv, tick, flag | BCT_ENEMY | 1,
	                      skill_castend_nodamage_id);
}

void SkillJudgement::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	// The primary target is taking the hit and the burn; the Fear is for everything else
	// caught in the splash.
	if (skill_area_temp[1] == target->id)
		return;

	map_session_data *dstsd = BL_CAST(BL_PC, target);
	status_data *tstatus = status_get_status_data(*target);

	// Same gate Grand Cross uses for its own Blind: non-players that are Undead by race or
	// element, or Demon race. Keeps the crowd control thematic instead of universal.
	if (!dstsd && (battle_check_undead(tstatus->race, tstatus->def_ele) || tstatus->race == RC_DEMON)) {
		// SCSTART_NOTICKDEF pins the duration at the full 3 seconds regardless of the
		// target's INT/LUK/level resistance. Bosses are still immune via BossResist.
		status_change_start(src, target, SC_FEAR, 10000, skill_lv, 0, 0, 0,
		                    skill_get_time2(getSkillId(), skill_lv), SCSTART_NOTICKDEF);
	}
}

void SkillJudgement::applyAdditionalEffects(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32 attack_type, enum damage_lv dmg_lv) const {
	// Upstream intent: let Judgement trigger physical autospells. Kept as-is, but be aware
	// it is currently inert - applyAdditionalEffects takes attack_type BY VALUE (unlike
	// applyCounterAdditionalEffects, which takes a reference), so these writes are discarded
	// when the function returns. Left in place so the intent survives if the base class
	// signature ever changes; do not build anything on top of it in the meantime.
	attack_type |= BF_NORMAL;
	attack_type |= BF_WEAPON;
}
