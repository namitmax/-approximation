#include "utils.h"

int
index (const int i, const int j, const int n)
{
  return i * (n + 1) + j;
}

point_2D
point (const int i, const int j, const int nx, const int ny,
       const point_2D & bot, const point_2D & top)
{
  point_2D diff = top - bot;
  point_2D tmp = {diff.x * i / nx, diff.y * j / ny};
  return bot + tmp;
}

int
matrix_size (const int nx, const int ny)
{
  return (nx + 1) * (ny + 1);
}

int
get_len (const int nx, const int ny)
{
  int n = std::max(nx, ny);
  return 7 * n * n + 6 * n + 10;
}
