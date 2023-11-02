#include "map.h"

int noise(int x, int y, int seed) {
    int tmp = HASH[(y + seed) % 256];
    return HASH[(tmp + x) % 256];
}

float linear_interpolation(float x, float y, float s) {
    return x + s * (y - x);
}

float smooth_interpolation(float x, float y, float s) {
    return linear_interpolation(x, y, s * s * (3 - 2 * s));
}

float noise_2d(float x, float y, int seed) {
    int x_int = (int)x;
    int y_int = (int)y;
    float x_frac = x - x_int;
    float y_frac = y - y_int;
    int s = noise(x_int, y_int, seed);
    int t = noise(x_int + 1, y_int, seed);
    int u = noise(x_int, y_int + 1, seed);
    int v = noise(x_int + 1, y_int + 1, seed);
    float low = smooth_interpolation(s, t, x_frac);
    float high = smooth_interpolation(u, v, x_frac);
    return smooth_interpolation(low, high, y_frac);
}

float perlin_noise_2d(float x, float y, float freq, int depth, int seed) {
    float xa = x * freq;
    float ya = y * freq;
    float amp = 1.0;
    float fin = 0;
    float div = 0.0;

    for(int i = 0; i < depth; i++) {
        div += 256 * amp;
        fin += noise_2d(xa, ya, seed) * amp;
        amp /= 2;
        xa *= 2;
        ya *= 2;
    }

    return fin / div;
}

int initialize_map() {
    int posx = -1;
    int posy = -1;
    FILE *f = fopen("../map.map", "w");
    if (f == NULL) {
        return -1;
    }
    srand(time(NULL));
    int seed = rand();

    for(int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLUMNS; x++) {
            float tmp = perlin_noise_2d(x, y, 0.05, 8, seed);
            if (x == 0 || y == 0 || x == COLUMNS - 1 || y == ROWS - 1) {
                fputc(OBSTACLE, f);
            } else {
                if (tmp > 0.4 && tmp < 0.65) {
                    fputc(GRASS, f);
                } else if (tmp <= 0.4) {
                    if (posx == -1 && posy == -1) {
                        posx = x;
                        posy = y;
                    }
                    fputc(PATH, f);
                } else {
                    fputc(OBSTACLE, f);
                }
            }
        }
        fputc('\n', f);
    }
    fprintf(f, "%d %d", posx, posy);
    fclose(f);
    return 0;
}

int get_map(Context* context)
{
    context->map = malloc(sizeof(TILE*)*ROWS);
    if(context->map == NULL)
    {
        printf("ALED\n");
        return 1;
    }
    FILE * f= fopen("../map.map", "r");
    if(f == NULL)
    {
        perror("ALED FICHIER\n");
        return 1;
    }
    for (int i = 0;i < ROWS; i++)
    {
        context->map[i] = malloc(sizeof(TILE)*COLUMNS);
        for (int j = 0;j < COLUMNS; j++)
        {
            context->map[i][j] = fgetc(f);
        }
        fgetc(f);
    }
    int x;
    int y;
    fscanf(f,"%d %d",&x,&y);
    context->pos_x = x;
    context->pos_y = y;
    fclose(f);
    return 0;
}

void display_map(Context* context)
{
    for(int i = context->pos_y-12;i<context->pos_y+12;i++)
    {
        for(int j = context->pos_x-24;j<context->pos_x+24;j++)
        {
            if(i<0 || j<0 || j>=ROWS || i>=COLUMNS)
            {
                printf(" ");
            }
            else if(j==context->pos_x && i==context->pos_y)
            {
                printf("\033[1;31m%c\033[0m",PLAYER);
            }
            else
            {
                if(context->map[j][i] == OBSTACLE) {
                    printf("\033[1;37m%c",context->map[j][i]);
                } else if(context->map[j][i] == GRASS) {
                    printf("\033[1;32m%c",context->map[j][i]);
                } else {
                    printf("\033[1;33m%c",context->map[j][i]);
                }
                printf("%c",context->map[j][i]);
            }
        }
        printf("\n");
    }
}
