#ifndef MAP_H
#define MAP_H

#include <time.h>
#include "battle.h"
#include "character.h"
#include "database.h"

#define ROWS 300
#define COLUMNS 300

typedef enum TILE
{
    PLAYER = 'X',
    PATH = '.',
    GRASS = 'W',
    OBSTACLE = '#'
}
TILE;

typedef struct MapContext
{
    int pos_x;
    int pos_y;
    TILE **map;
    Character *player;
}
MapContext;

/**
 * Hash values used for noise generation.
 */
static int HASH[] =
{
    208,34,231,213,32,248,233,56,161,78,24,140,71,48,140,254,245,255,247,247,40,
    185,248,251,245,28,124,204,204,76,36,1,107,28,234,163,202,224,245,128,167,204,
    9,92,217,54,239,174,173,102,193,189,190,121,100,108,167,44,43,77,180,204,8,81,
    70,223,11,38,24,254,210,210,177,32,81,195,243,125,8,169,112,32,97,53,195,13,
    203,9,47,104,125,117,114,124,165,203,181,235,193,206,70,180,174,0,167,181,41,
    164,30,116,127,198,245,146,87,224,149,206,57,4,192,210,65,210,129,240,178,105,
    228,108,245,148,140,40,35,195,38,58,65,207,215,253,65,85,208,76,62,3,237,55,89,
    232,50,217,64,244,157,199,121,252,90,17,212,203,149,152,140,187,234,177,73,174,
    193,100,192,143,97,53,145,135,19,103,13,90,135,151,199,91,239,247,33,39,145,
    101,120,99,3,186,86,99,41,237,203,111,79,220,135,158,42,30,154,120,67,87,167,
    135,176,183,191,253,115,184,21,233,58,129,233,142,39,128,211,118,137,139,255,
    114,20,218,113,154,27,127,246,250,1,8,198,250,209,92,222,173,21,88,102,219
};

/**
 * @brief Generates consistent pseudo-random noise based on coordinates and a
 * seed.
 * @param x The x-coordinate for noise generation.
 * @param y The y-coordinate for noise generation.
 * @param seed A seed value to vary the noise pattern.
 * @return A pseudo-random, consistent integer based on the inputs.
 */
int noise(int x, int y, int seed);

/**
 * @brief Performs linear interpolation between two values. This is useful for
 * creating gradual transitions between values.
 * @param x The start value.
 * @param y The end value.
 * @param factor The interpolation factor (0.0 to 1.0).
 * @return The interpolated value.
 */
float linear_interpolation(float x, float y, float factor);

/**
 * @brief Performs smooth interpolation between two values. Useful for creating
 * smoother, more natural transitions than linear interpolation.
 * @param x The start value.
 * @param y The end value.
 * @param factor The interpolation factor (0.0 to 1.0), which is modified for
 * smoothness.
 * @return The interpolated value.
 */
float smooth_interpolation(float x, float y, float factor);

/**
 * Generates 2D noise based on x, y coordinates and a seed.
 * @param x X coordinate
 * @param y Y coordinate
 * @param seed Random seed
 * @return 2D noise value
 */
float noise_2d(float x, float y, int seed);

/**
 * Generates Perlin noise for 2D coordinates.
 * @param x X coordinate
 * @param y Y coordinate
 * @param freq Frequency of the noise
 * @param depth Depth of noise calculation
 * @param seed Random seed
 * @return Perlin noise value
 */
float perlin_noise_2d(float x, float y, float freq, int depth, int seed);

/**
 * Initializes the map by generating terrain using Perlin noise and writing to a
 * file.
 * @return 0 on success, -1 on failure
 */
int initialize_map();

/**
 * Loads the map from a file into the given map_context.
 * @param map_context Pointer to the map_context structure
 * @return 0 on success, 1 on failure
 */
int get_map(MapContext* map_context);

/**
 * Displays the map from the given map_context on the console.
 * @param map_context Pointer to the map_context structure
 */
void display_map(MapContext* map_context);

/**
 * @brief Get user key & do something depending on the pressed key.
 * @param key The key pressed by the user.
 * @param map_context A pointer to map context.
 * @return 0 if the user pressed 'q' for quit.
 */
unsigned char key_listener(unsigned char key, MapContext *map_context);

/**
 * @brief Event loop for the map, which displays the map and waits for user
 * input.
 * @param map_context A pointer to map context.
 */
void explore_map(MapContext *map_context);

#endif // MAP_H
