-- Raise existing characters to match INVENTORY_BASE_SIZE (200), set in
-- src/common/mmo.hpp by commit 8d2d39498. Characters left at the old default
-- of 100 make clif_inventory_expansion_info() send a negative expansionSize,
-- which makes the client's barter window compute a capacity of 0 and refuse
-- every purchase locally ("...exceeds the possession limit").
UPDATE `char` SET `inventory_slots` = 200 WHERE `inventory_slots` < 200;
ALTER TABLE `char` ALTER COLUMN `inventory_slots` SET DEFAULT 200;
