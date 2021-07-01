#ifndef UTILS_H
#define UTILS_H

#include "params.h"

int
index (const int i, const int j, const int n);

point_2D
point (const int i, const int j, const int nx, const int ny,
       const point_2D & bot, const point_2D & top);

int
matrix_size (const int nx, const int ny);

int
get_len (const int nx, const int ny);

#endif // UTILS_H
