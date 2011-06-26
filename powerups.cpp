/**
 * This module hands out powerups to the block-creation functions and triggers powerup effects.
 */

#include <common.h>

#define ENABLE_LIGHTNING_STORM
#define ENABLE_LIGHTNING_BOLT
#define ENABLE_BOMB
#define ENABLE_TOPSY_TURVY
//#define ENABLE_EMP
//#define ENABLE_GHOST
#define ENABLE_BRONZE
#define ENABLE_SILVER
#define ENABLE_GOLD

powerup_t p_getPowerup()
{
	return POWERUP_NONE;
}
