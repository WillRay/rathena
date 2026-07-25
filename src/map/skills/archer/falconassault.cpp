// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "falconassault.hpp"

#include <common/timer.hpp>

#include "map/clif.hpp"
#include "map/map.hpp"
#include "map/mob.hpp"
#include "map/skill.hpp"
#include "map/status.hpp"
#include "map/unit.hpp"

// Sniper rebalance: Hunting Party cosmetic companions. Mob ids of the client's
// reserved 4th-job falcon sprites, filled in by db/import/mob_db.yml.
// 4JOB_R_FALCON is an owl sprite - that is what the "Ranger falcon" actually
// looks like in the client.
static constexpr int32 MOBID_HUNTINGPARTY_RANGER = 20832;   // owl
static constexpr int32 MOBID_HUNTINGPARTY_WINDHAWK = 20830; // falcon

/**
 * Spawn one purely cosmetic Hunting Party falcon next to the caster.
 *
 * The companion is a real mob so the client has something to draw and walk
 * around (the Sniper's own falcon is just an OPTION_FALCON bit on the player and
 * cannot be duplicated), but it is deliberately inert:
 *  - master_id + AI_LEGION hand it to the existing slave AI
 *    (mob_ai_sub_hard_slavemob, src/map/mob.cpp), which walks it back to the
 *    caster whenever it drifts too far. The slave AI also kills the companion by
 *    itself if its master dies or logs out.
 *
 *    AI_LEGION is picked as the most inert summon AI on offer: any value other
 *    than AI_NONE keeps the "player summoned" semantics that suppress experience
 *    and drops, and AI_LEGION is the only one carrying no special handling of its
 *    own anywhere else in the codebase. It is deliberately NOT AI_ABR/AI_BIONIC,
 *    the two the client draws a summon HP gauge over (clif_summon_init) - a gauge
 *    floating above a decorative bird looks wrong. The tradeoff is that those two
 *    are also the only AIs that teleport a slave to its master, so a companion
 *    left behind by a map change stays on the old map instead of following. That
 *    is bounded and harmless: the strikes fall back to being drawn from the Sniper
 *    while no companion is reachable, and status_change_end still despawns it
 *    correctly across maps when the buff ends.
 *  - ud.immune_attack makes battle_check_target (src/map/battle.cpp) report it
 *    as not a valid target at all, so wild monsters never aggro onto it and it
 *    can never soak a hit meant for the Sniper.
 *  - a deletetimer despawns it when the buff would expire, as a backstop for the
 *    explicit despawn in status_change_end (src/map/status.cpp).
 *
 * @return the spawned mob's block_list id, or 0 if the spawn failed.
 */
static int32 huntingparty_spawn_companion(block_list *src, int32 mob_id, t_tick duration) {
	// x/y of -1 asks mob_once_spawn_sub for a free cell right next to the caster.
	mob_data *md = mob_once_spawn_sub(src, src->m, -1, -1, nullptr, mob_id, "", SZ_SMALL, AI_LEGION);

	if (md == nullptr)
		return 0;

	md->master_id = src->id;
	md->special_state.ai = AI_LEGION;
	md->ud.immune_attack = true;

	if (md->deletetimer != INVALID_TIMER)
		delete_timer(md->deletetimer, mob_timer_delete);
	md->deletetimer = add_timer(gettick() + duration, mob_timer_delete, md->id, 0);

	mob_spawn(md);

	return md->id;
}

SkillFalconAssault::SkillFalconAssault() : SkillImpl(SN_FALCONASSAULT) {
}

void SkillFalconAssault::castendNoDamageId(block_list *src, block_list *target, uint16 skill_lv, t_tick tick, int32& flag) const {
	t_tick duration = skill_get_time(getSkillId(), skill_lv);

	// The two extra falcons of the hunting party. Their ids ride along on the
	// status (val2/val3) so status_change_end can send them away the moment the
	// buff drops, rather than leaving them out until their delete timer fires.
	int32 ranger_id = huntingparty_spawn_companion(src, MOBID_HUNTINGPARTY_RANGER, duration);
	int32 windhawk_id = huntingparty_spawn_companion(src, MOBID_HUNTINGPARTY_WINDHAWK, duration);

	bool started = sc_start4(src, src, skill_get_sc(getSkillId()), 100, skill_lv, ranger_id, windhawk_id, 0, duration);

	if (!started) {
		// Nothing will ever come along to clean these up otherwise.
		mob_despawn_summon(ranger_id);
		mob_despawn_summon(windhawk_id);
	}

	clif_skill_nodamage(src, *src, getSkillId(), skill_lv, started);
}
