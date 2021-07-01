#include "solver.h"

double
scalar_product (const double * a, const double * b, const int n)
{
  double res = 0.0;
  for (int i = 0; i < n; i++)
    res += a[i] * b[i];
  return res;
}

void
matrix_mult_sub_vector (const double * a, const double * b, const double * c,
                        double * x,
                        const int *index, 
		        const int n, const int k, const int p,
                        const double coeff = 0.)
{
  int i1 = n * k / p;
  int i2 = n * (k + 1) / p;
  double res = 0.;
  for (int i = i1; i < i2; i++)
    {
      res = a[i] * b[i];
      for (int j = index[i]; j < index[i + 1]; j++)
        res += a[j] * b[index[j]];
      x[i] = res - c[i] * coeff;
    }
}

void
solve_precond_system (const double * a, const double * b, double * x,
                      const int n,      const int k,      const int p)
{
  int i1 = n * k / p;
  int i2 = n * (k + 1) / p;

  for (int i = i1; i < i2; i++)
    x[i] = b[i] / a[i];
}

void
vector_linear_comb (const double * b, double * x,
                    const int n, const int k, const int p,
		    const double coeff = 0.)
{
  int i1 = n * k / p;
  int i2 = n * (k + 1) / p;

  for (int i = i1; i < i2; i++)
    x[i] += coeff * b[i];
}

std::pair<int, double>
minimal_residual_msr (const double * a, const double * b, double * x,
                      double * r, double * u, double * v,
                      const int * ind,
                      const double norm, const double eps, const int max_epochs,
		      const int k, const int p, const int n,
                      pthread_barrier_t * barrier)
{
  double prec = eps * eps * norm;
  double tau = 0., c_1 = 0., c_2 = 0.;
  int epoch = 0;
  
  matrix_mult_sub_vector (a, x, b, r, ind, n, k, p, 1.0);
  pthread_barrier_wait (barrier);
  
  for (epoch = 0; epoch < max_epochs; epoch++)
    {
      solve_precond_system (a, r, v, n, k, p);
      pthread_barrier_wait (barrier);
      matrix_mult_sub_vector (a, v, b, u, ind, n, k, p, 0.0);
      pthread_barrier_wait (barrier);

      c_1 = scalar_product (u, r, n);
      c_2 = scalar_product (u, u, n);

      pthread_barrier_wait (barrier);
      
      //if (k == 0 && epoch > 0)
      //  {
      //    printf ("[SOLVER] epoch =%d, residual = %e\n", epoch, std::min (fabs (c_1), fabs (c_2)) / norm);
      //  }

      if (fabs (c_1) < prec || fabs (c_2) < prec)
        break;
      
      tau = c_1 / c_2;

      vector_linear_comb (v, x, n, k, p, -tau);
      vector_linear_comb (u, r, n, k, p, -tau);

      pthread_barrier_wait (barrier);
    }

  if (epoch >= max_epochs)
    return std::pair<int, double> (epoch, -1.0);

  return std::pair<int, double> (epoch, std::min (fabs (c_1), fabs (c_2)) / norm);
}
