#ifndef INTEGRATE_H
#define INTEGRATE_H

#include "params.h"

double 
integral (const point_2D & p1, const point_2D & p2, const point_2D & p3,
          double f1, double f2, double f3,
          double g1, double g2, double g3);

double
integral (const point_2D & p1, const point_2D & p2, const point_2D & p3,
          std::function <double (double, double)> func);

#endif // INTEGRATE_H
