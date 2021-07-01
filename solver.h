#ifndef SOLVER_H
#define SOLVER_H

#include <math.h>
#include <cstdio>
#include <functional>

#include "params.h"

std::pair<int, double>
minimal_residual_msr (const double * a, const double * b, double * x,
                      double * r, double * u, double * v,
                      const int * ind,
		      const double norm,  double eps, const int max_epochs,
                      const int k, const int p, const int n,
                      pthread_barrier_t * barrier);

#endif // SOLVER_H
