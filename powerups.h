/**
 * powerups.h
 * This module defines mechanisms useful for tracking and activating powerups.
 */

typedef unsigned int powerup_t;

// No powerup
#define POWERUP_NONE 0x100

// Lighting bolts randomly remove indivudual blocks from the grid
#define POWERUP_LIGHTING_STORM 0x101

// A single bolt of lighting takes out an entire column of blocks
#define POWERUP_LIGHTING_BOLT 0x102

// A bomb that explodes, removing a radius of blocks
#define POWERUP_BOMB 0x103

// Flips the camera upside-down for 20 seconds
#define POWERUP_TOPSY_TURVY 0x104

// Emits static discharges through the display, blocking out the Next Block window for
//   the next three blocks
#define POWERUP_EMP 0x105

// Fades the next block to a barely-distinguishable, transparent white
#define POWERUP_GHOST 0x106

// Doubles your score for the next 30 seconds. Stacks on other multipliers
#define POWERUP_BRONZE 0x107

// Triples your score for the next 30 seconds. Stacks on other multipliers
#define POWERUP_SILVER 0x108

// Quadruples your score for the next 30 seconds. Stacks on other multipliers
#define POWERUP_GOLD 0x109

// Generates a random powerup to give a block
Powerup p_getPowerup();
