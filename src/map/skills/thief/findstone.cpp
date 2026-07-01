// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "findstone.hpp"

#include "map/battle.hpp"
#include "map/clif.hpp"
#include "map/log.hpp"
#include "map/map.hpp"
#include "map/pc.hpp"
#include "map/status.hpp"

SkillFindStone::SkillFindStone() : SkillImpl(TF_PICKSTONE) {
}

void SkillFindStone::castendNoDamageId(block_list *src, block_list *bl, uint16 skill_lv, t_tick tick, int32 &flag) const {
	map_session_data *sd = BL_CAST(BL_PC, src);

	if (sd == nullptr)
		return;

	clif_skill_nodamage(src, *bl, getSkillId(), skill_lv);

#ifndef RENEWAL
	// Stone economy redesign: instead of adding a physical Stone item, Find
	// Stone grants the persistent SC_PICK_STONE ("Holding Stone") status. If
	// the caster already holds a stone the cast still succeeds but is a silent
	// no-op — the stone does not stack or reset. INFINITE_TICK keeps it up
	// until consumed (Stone Fling / Sand Attack) or cleared on death.
	status_change *sc = status_get_sc(src);
	if (sc == nullptr || sc->getSCE(SC_PICK_STONE) == nullptr)
		sc_start(src, src, SC_PICK_STONE, 100, 1, INFINITE_TICK);
#else
	unsigned char eflag;
	item item_tmp;
	block_list tbl;
	memset(&item_tmp, 0, sizeof(item_tmp));
	memset(&tbl, 0, sizeof(tbl)); // [MouseJstr]
	item_tmp.nameid = ITEMID_STONE;
	item_tmp.identify = 1;
	tbl.id = 0;
	// Commented because of duplicate animation [Lemongrass]
	// At the moment this displays the pickup animation a second time
	// If this is required in older clients, we need to add a version check here
	// clif_takeitem(*sd,tbl);
	eflag = pc_additem(sd, &item_tmp, 1, LOG_TYPE_PRODUCE);
	if (eflag) {
		clif_additem(sd, 0, 0, eflag);
		if (battle_config.skill_drop_items_full)
			map_addflooritem(&item_tmp, 1, sd->m, sd->x, sd->y, 0, 0, 0, 4, 0);
	}
#endif
}
