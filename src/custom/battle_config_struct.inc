// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom battle config structure
 * Place the definition of custom battle configuration structure members in this file.
 *
 * Make sure that the name you want is not already taken by looking into battle.hpp
 * Do not forget to add initilization logic to battle_config_init.inc
 *
 * Format:
 *	 <datatype> name;
 **/

// Show Element Info [Hyroshima]
int mob_ele_view;

// VIT redesign
int vit_def2_quadratic_divisor;
int vit_mdef2_quadratic_divisor;
int vit_sc_def_halfpoint;
int vit_flat_hp_cap;

// DEX redesign
int aspd_dex_weight;
int dex_variance_max;
int dex_variance_min;
int dex_variance_halfpoint;
