#ifndef DRAW_TOOLS_H
#define DRAW_TOOLS_H

#include <math.h>
#include <cstdio>
#include <utility>
#include <memory>
#include <functional>
#include "params.h"

void
get_triangles (const std::unique_ptr<double[]> & x,
               std::function <double (double, double)> func,
	       const point_2D & bot, const point_2D & top,
               std::vector<std::array<point_3D, DIM>> & function,
               std::vector<std::array<point_3D, DIM>> & approx,
               std::vector<std::array<point_3D, DIM>> & residuals,
               const int nx, const int ny);

void
get_residual_and_max (const std::unique_ptr<double[]> & x,
                      std::function <double (double, double)> func,
                      double & residual,   double & max,
                      const int threadNum, const int p,
                      const int nx,        const int ny,
	              std::function <point_2D (const int, const int)> simple_point);

#endif // DRAW_TOOLS_H
