// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include "counterattack.hpp"

#include "map/clif.hpp"
#include "map/status.hpp"

SkillCounterAttack::SkillCounterAttack() : SkillImpl(KN_AUTOCOUNTER) {
}

void SkillCounterAttack::modifyDamageData(Damage& dmg, const block_list& src, const block_list& target, uint16 skill_lv) const {
	dmg.flag = (dmg.flag&~BF_SKILLMASK)|BF_NORMAL;
}

void SkillCounterAttack::castendNoDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const {
	bool started = sc_start(src, src, skill_get_sc(getSkillId()), 100, skill_lv, skill_get_time(getSkillId(), skill_lv));

	clif_skill_nodamage(src, *src, getSkillId(), skill_lv, started);
	// The cast is instant (CastTime: 0) but the client still shows its default cast-bar
	// UI for the skill-use packet unless told otherwise. The old reactive version cleared
	// it reactively via clif_skillcastcancel the moment a counter fired ("Remove the
	// casting bar." - Skotlex); this stance no longer has that reactive moment, so clear
	// it immediately once the buff is applied instead.
	clif_skillcastcancel(*src);
	// One-shot "stance activated" flash on the Knight.
	clif_specialeffect(src, EF_GUMGANG8, AREA);
	if (started) {
		// Show the same persistent "bound" cage used for rooted targets (Snaring
		// Arrow's SC_ANKLE, and Sohee's NPC_STOP) so it's visually obvious the
		// Knight cannot move while the stance is up. Matching teardown fires when
		// SC_KNIGHTCOUNTER ends (status_change_end, src/map/status.cpp).
		clif_specialeffect(src, EF_NPC_STOP, AREA);
	}
}
