/**
 * This module hands out powerups to the block-creation functions and triggers powerup effects.
 */

#include "common.h"

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

void p_activatePowerup(const powerup_t &powerup, const Point &position)
{
	// TODO: Activate the powerups here
	switch (powerup)
	{
		case POWERUP_LIGHTNING_STORM:
			// Start a lightning storm
			// TODO: Need the number of lightning bolts to fire
			break;
		
		case POWERUP_LIGHTNING_BOLT:
			// Launch a lightning bolt to clear a column of the grid
			// TODO: Decide if the location should be random or in the given location
			break;
		
		case POWERUP_BOMB:
			// Trigger a bomb on the given spot
			break;
		
		case POWERUP_TOPSY_TURVY:
			// Flip the camera up-side-down for 15 seconds
			break;
		
		case POWERUP_EMP:
			// Start the EMP discharge
			break;
		
		case POWERUP_GHOST:
			// Mark the next three blocks as ghosts
			break;
		
		case POWERUP_BRONZE:
			// Increase the score multiplier by 2x
			break;
		
		case POWERUP_SILVER:
			// Increase the score multiplier by 3x
			break;
		
		case POWERUP_GOLD:
			// Increase the score multiplier by 4x
			break;
	} // end switch(powerup)
} // end p_activatePowerup(powerup, x, y)
