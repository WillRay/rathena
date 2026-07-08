// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#pragma once

#include "../skill_impl.hpp"

// Gameplay test: Claymore Trap is no longer a ground trap. It is now a
// non-targeted direct attack that auto-fires on the caster's current target
// (Self + NoTargetSelf in skill_db). Instead of hitting instantly it arms on
// the target and detonates after 2 seconds, then deals its splash damage
// (SplashArea 2, BF_MISC formula in battle.cpp).
class SkillClaymoreTrap : public SkillImplRecursiveDamageSplash {
public:
	SkillClaymoreTrap();

	void castendDamageId(block_list* src, block_list* target, uint16 skill_lv, t_tick tick, int32& flag) const override;
};
