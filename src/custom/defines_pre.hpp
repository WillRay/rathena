// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef CONFIG_CUSTOM_DEFINES_PRE_HPP
#define CONFIG_CUSTOM_DEFINES_PRE_HPP

/**
 * rAthena configuration file (http://rathena.org)
 * For detailed guidance on these check http://rathena.org/wiki/SRC/config/
 **/
// Must match the client in D:/Client (2025-06-04_Ragexe.exe). The expanded
// barter packet gained a refine_level field at client 2025-04-02, and it is
// the only packet gate in the tree above 20250122 -- running 20250319 against
// a newer client corrupted every multi-requirement barter shelf.
#define PACKETVER 20250604 //20250319 //20220406 // 20220328
/* #define PACKETVER_RE */

#endif /* CONFIG_CUSTOM_DEFINES_PRE_HPP */
