#ifndef ITER_ALG_H
#define ITER_ALG_H

#include <memory>
#include "params.h"

void
init_matrix (std::unique_ptr<double[]> & a, std::unique_ptr<int[]> & ind,
             const int n_x, const int n_y,
             const int k,
	     std::function <int (const int, const int)> index_simple,
	     std::function <point_2D (const int, const int)> point_simple);

void
init_b   (std::unique_ptr<double[]> & b, std::function <double (double, double)> func,
          const int n_x, const int n_y,
          const int k, const int p,
	  std::function <int (const int, const int)> index_simple,
	  std::function <point_2D (const int, const int)> point_simple);

#endif // ITER_ALG_H
