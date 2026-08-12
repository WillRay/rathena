// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

/**
 * Custom battle config initializations
 * Place the definition of custom battle configuration initializations in this file.
 *
 * Do not forget to add a comma at the end of the line, if you want to add another config
 *
 * Format:
 *	 { "name", &battle_config.<variable name>, <default value>, <minimum value>, <maximum value> },
 **/

{ "mob_ele_view",                       &battle_config.mob_ele_view,                    0,      0,      1,              },

// VIT redesign
{ "vit_def2_quadratic_divisor",         &battle_config.vit_def2_quadratic_divisor,      150,    0,      10000,          },
{ "vit_mdef2_quadratic_divisor",        &battle_config.vit_mdef2_quadratic_divisor,     400,    0,      10000,          },
{ "vit_sc_def_halfpoint",               &battle_config.vit_sc_def_halfpoint,            60,     1,      10000,          },
{ "vit_flat_hp_cap",                    &battle_config.vit_flat_hp_cap,                 1000,   0,      1000000,        },

// DEX redesign
{ "aspd_dex_weight",                    &battle_config.aspd_dex_weight,                 75,     0,      100,            },
{ "dex_variance_max",                   &battle_config.dex_variance_max,                100,    0,      100,            },
{ "dex_variance_min",                   &battle_config.dex_variance_min,                15,     0,      100,            },
{ "dex_variance_halfpoint",             &battle_config.dex_variance_halfpoint,          20,     0,      1000,           },
