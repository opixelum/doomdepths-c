#include "map.h"

int noise(int x, int y, int seed)
{
    int temp = HASH[(y + seed) % 256];
    return HASH[(temp + x) % 256];
}

float linear_interpolation(float x, float y, float factor)
{
    return x + factor * (y - x);
}

float smooth_interpolation(float x, float y, float factor)
{
    float modified_factor = factor * factor * (3 - 2 * factor);
    return linear_interpolation(x, y, modified_factor);
}

float noise_2d(float x, float y, int seed)
{
    int x_int = (int) x;
    int y_int = (int) y;

    float x_frac = x - (float) x_int;
    float y_frac = y - (float) y_int;

    int s = noise(x_int, y_int, seed);
    int t = noise(x_int + 1, y_int, seed);
    int u = noise(x_int, y_int + 1, seed);
    int v = noise(x_int + 1, y_int + 1, seed);

    float low = smooth_interpolation((float) s, (float) t, x_frac);
    float high = smooth_interpolation((float) u, (float) v, x_frac);

    return smooth_interpolation(low, high, y_frac);
}

float perlin_noise_2d(float x, float y, float freq, int depth, int seed)
{
    float xa = x * freq;
    float ya = y * freq;
    float amp = 1;
    float fin = 0;
    float div = 0;

    for (int i = 0; i < depth; i++)
    {
        div += 256 * amp;
        fin += noise_2d(xa, ya, seed) * amp;
        amp /= 2;
        xa *= 2;
        ya *= 2;
    }

    return fin / div;
}

int initialize_map()
{
    int position_x = -1;
    int position_y = -1;

    FILE *map_file = fopen("map.txt", "w");
    if (!map_file) return -1;

    srand(time(NULL));
    int seed = rand();

    for (int y = 0; y < ROWS; y++)
    {
        for (int x = 0; x < COLUMNS; x++)
        {
            float temp = perlin_noise_2d
            (
                (float) x,
                (float) y,
                (float) 0.05,
                8,
                seed
            );

            if (x == 0 || y == 0 || x == COLUMNS - 1 || y == ROWS - 1)
                fputc(OBSTACLE, map_file);
            else
            {
                if (temp > 0.4 && temp < 0.65) fputc(GRASS, map_file);
                else if (temp <= 0.4)
                {
                    if (position_x == -1 && position_y == -1)
                    {
                        position_x = x;
                        position_y = y;
                    }

                    fputc(PATH, map_file);
                }
                else fputc(OBSTACLE, map_file);
            }
        }
        fputc('\n', map_file);
    }
    fprintf(map_file, "%d %d", position_x, position_y);
    fclose(map_file);

    return 0;
}

int get_map(MapContext* map_context)
{
    map_context->map = malloc(ROWS * sizeof map_context->map);
    if (!map_context->map)
    {
        fprintf(stderr, "ERROR: map.c: get_map: map_context->map: malloc failed\n");
        return EXIT_FAILURE;
    }

    FILE *map_file= fopen("map.txt", "r");
    if(!map_file)
    {
        fprintf(stderr, "ERROR: map.c: get_map: map_file: fopen failed\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < ROWS; i++)
    {
        map_context->map[i] = malloc(COLUMNS * sizeof map_context->map[i]);
        for (int j = 0;j < COLUMNS; j++) map_context->map[i][j] = fgetc(map_file);
        fgetc(map_file);
    }

    int x;
    int y;
    fscanf(map_file, "%d %d", &x, &y);
    map_context->pos_x = x;
    map_context->pos_y = y;

    fclose(map_file);

    return 0;
}

void display_map(MapContext* map_context)
{
    clear_screen();

    for (int i = map_context->pos_y - 12; i < map_context->pos_y + 12; i++)
    {
        for (int j = map_context->pos_x - 24; j < map_context->pos_x + 24; j++)
        {
            if (i < 0 || j < 0 || j >= ROWS || i >= COLUMNS) printf(" ");
            else if (j == map_context->pos_x && i == map_context->pos_y)
                printf("\033[1;31m%c\033[0m",PLAYER);
            else
            {
                if (map_context->map[j][i] == OBSTACLE)
                    printf("\033[1;37m%c", map_context->map[j][i]);
                else if (map_context->map[j][i] == GRASS)
                    printf("\033[1;32m%c", map_context->map[j][i]);
                else
                    printf("\033[1;33m%c", map_context->map[j][i]);

                printf("%c", map_context->map[j][i]);
            }
        }
        printf("\n");
    }
}

unsigned char key_listener(unsigned char key, MapContext *map_context)
{
    switch (key)
    {
    case 'z':
        if
        (
            map_context->pos_y - 1 < 0
            || map_context->map[map_context->pos_x][map_context->pos_y - 1] == OBSTACLE
        )
            break;
        else if (map_context->map[map_context->pos_x][map_context->pos_y - 1] == GRASS)
        {
            map_context->pos_y--;
            // TODO: randomly start a battle
        }
        else map_context->pos_y--;
        break;

    case 'd':
        if
        (
            map_context->pos_x + 1 > ROWS
            || map_context->map[map_context->pos_x + 1][map_context->pos_y] == OBSTACLE
        )
            break;
        else if (map_context->map[map_context->pos_x + 1][map_context->pos_y] == GRASS)
        {
            map_context->pos_x++;
            // TODO: randomly start a battle
        }
        else map_context->pos_x++;
        break;

    case 's':
        if
        (
            map_context->pos_y + 1 > COLUMNS
            || map_context->map[map_context->pos_x][map_context->pos_y + 1] == OBSTACLE
        )
            break;
        else if (map_context->map[map_context->pos_x][map_context->pos_y + 1] == GRASS)
        {
            map_context->pos_y+=1;
            // TODO: randomly start a battle
        }
        else map_context->pos_y+=1;
        break;

    case 'q':
        if
        (
            map_context->pos_x - 1 < 0
            || map_context->map[map_context->pos_x - 1][map_context->pos_y] == OBSTACLE
        )
            break;
        else if (map_context->map[map_context->pos_x - 1][map_context->pos_y] == GRASS)
        {
            map_context->pos_x--;
            // TODO: randomly start a battle
        }
        else map_context->pos_x--;
        break;

    case 'i':
        // TODO: display inventory
        break;

    case 'Q':
        // TODO: return to main menu
        return 0;
    }
}

void explore_map(MapContext *map_context)
{
    while (1)
    {
        display_map(map_context);
        if (key_listener(getchar_no_enter(), map_context) == 0) break;
    }
}
