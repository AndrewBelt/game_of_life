#include <stdlib.h>
#include <string.h>
#include "gol.h"

inline int euc_mod(int p, int q)
{
  int r = p % q;
  return (r < 0) ? r + q : r;
}

void grid_init(grid_t *grid, int width, int height)
{
  grid->cells = (char *) malloc(sizeof(char) * width * height);
  grid->width = width;
  grid->height = height;
}

void grid_destroy(grid_t *grid)
{
  free(grid->cells);
}

void grid_seed(grid_t *grid, long seed)
{
  int i, j;

  srand48(seed);

  for (j = 0; j < grid->height; j++) {
    for (i = 0; i < grid->width; i++) {
      grid->cells[i + grid->width * j] = (lrand48() & 0x1);
    }
  }
}

char grid_at(grid_t *grid, int x, int y)
{
  x = euc_mod(x, grid->width);
  y = euc_mod(y, grid->height);
  return grid->cells[x + grid->width * y];
}

int grid_neighbors(grid_t *grid, int x, int y)
{
  int i, j;
  int neighbors;

  neighbors = 0;

  neighbors += (y * grid->width + x+1 >= 0 && y * grid->width + x+1 < grid->width * grid->height) ? grid->cells[y * grid->width + x+1] : 0;
  neighbors += (y * grid->width + x-1 >= 0 && y * grid->width + x-1 < grid->width * grid->height) ? grid->cells[y * grid->width + x-1] : 0;
  neighbors += ((y+1) * grid->width + x >= 0 && (y+1) * grid->width + x < grid->width * grid->height) ? grid->cells[(y+1) * grid->width + x] : 0;
  neighbors += ((y-1) * grid->width + x >= 0 && (y-1) * grid->width + x < grid->width * grid->height) ? grid->cells[(y-1) * grid->width + x] : 0;
  neighbors += ((y+1) * grid->width + x+1 >= 0 && (y+1) * grid->width + x+1 < grid->width * grid->height) ? grid->cells[(y+1) * grid->width + x+1] : 0;
  neighbors += ((y+1) * grid->width + x-1 >= 0 && (y+1) * grid->width + x-1 < grid->width * grid->height) ? grid->cells[(y+1) * grid->width + x-1] : 0;
  neighbors += ((y-1) * grid->width + x+1 >= 0 && (y-1) * grid->width + x+1 < grid->width * grid->height) ? grid->cells[(y-1) * grid->width + x+1] : 0;
  neighbors += ((y-1) * grid->width + x-1 >= 0 && (y-1) * grid->width + x-1 < grid->width * grid->height) ? grid->cells[(y-1) * grid->width + x-1] : 0;

  return neighbors;
}

void grid_step(grid_t *grid)
{
  int i, j;
  int neighbors;
  grid_t tmp;

  grid_init(&tmp, grid->width, grid->height);

  for (i = 0; i < grid->width * grid->height; i++)
    tmp.cells[i] = grid->cells[i];

  for (j = 0; j < grid->height; j++) {
    for (i = 0; i < grid->width; i++) {
      neighbors = grid_neighbors(&tmp, i, j);
      if (neighbors < 2 || neighbors > 3)
        grid->cells[i + j * grid->width] = 0;
      if (neighbors == 3)
        grid->cells[i + j * grid->width] = 1;
    }
  }

  grid_destroy(&tmp);
}