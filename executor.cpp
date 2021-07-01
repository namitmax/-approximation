#include "utils.h"
#include "solver.h"
#include "init_iter_alg.h"
#include "draw_tools.h"
#include "executor.h"

double
get_max (const double val,
         const int threadNum, const int p,
         pthread_barrier_t * barrier)
{
  static double buffer[MAX_THREAD];
  buffer[threadNum] = val;
  pthread_barrier_wait (barrier);
  double max = 0.0;
  for (int i = 0; i < p; i++)
    max = std::max (max, buffer[i]);
  return max;
}

void *
execute (void * arg)
{
  std::pair<int, params &> data = *((std::pair<int, params&> *) arg);
  params &temp = data.second;
  int threadNum = data.first;
  int p = temp.p;

  auto index_simple = [&] (const int i, const int j)
    {
      return index (i, j, temp.ny);
    };

  auto point_simple = [&] (const int i, const int j)
    {
      return point (i, j, temp.nx, temp.ny, temp.bot, temp.top);
    };

  while (!temp.finish)
    {
      if (threadNum == 0)
        {
          temp.lock();
	  if (!temp.finish)
	    temp.set_start_calc();
        }
      pthread_barrier_wait (temp.barrier_for_calc);
      if (temp.finish)
        break;
      int nx = temp.nx;
      int ny = temp.ny;
      if (threadNum == 0)
        {
          temp.residual = 0.0;
          temp.max      = 0.0;
          temp.a        = std::make_unique<double[]> (get_len     (nx, ny));
          temp.index    = std::make_unique<int[]>    (get_len     (nx, ny));
          temp.b        = std::make_unique<double[]> (matrix_size (nx, ny));
          temp.x        = std::make_unique<double[]> (matrix_size (nx, ny));
          temp.temp_1   = std::make_unique<double[]> (matrix_size (nx, ny));
          temp.temp_2   = std::make_unique<double[]> (matrix_size (nx, ny));
          temp.temp_3   = std::make_unique<double[]> (matrix_size (nx, ny));
        }
      
      pthread_barrier_wait (temp.barrier_for_calc);
      init_matrix (temp.a, temp.index,               nx, ny, threadNum,    index_simple, point_simple);
      init_b      (temp.b, temp.functions[temp.k],   nx, ny, threadNum, p, index_simple, point_simple);

      int i1 = matrix_size (nx, ny) * threadNum / p;
      int i2 = matrix_size (nx, ny) * (threadNum + 1) / p;

      for (int i = i1; i < i2; i++)
        temp.x[i] = 0.0;

      double norm = 0.0;

      for (int i = temp.index[i1]; i < temp.index[i2]; i++)
        norm = std::max (norm, temp.a[i]);
      
      norm = get_max (norm, threadNum, p, temp.barrier_for_calc);
      pthread_barrier_wait (temp.barrier_for_calc);
      int N = matrix_size (nx, ny);
      int epochs = 0;
      int res_epoch = 0;
      double resid;
      for (int glob_epoch = 0; glob_epoch < GLOBAL_MAX; glob_epoch++)
	{
          auto res = minimal_residual_msr (temp.a.get (),      temp.b.get (),      temp.x.get (),
                                         temp.temp_1.get (), temp.temp_2.get (), temp.temp_3.get (), 
		 	                 temp.index.get (),  norm,               temp.eps, MAX,
		           		 threadNum,          p, N, temp.barrier_for_calc);

          epochs = res.first;
	  resid  = res.second;
	  if (epochs < 0)
            {
	      res_epoch += MAX;
	      continue;
            }
	  res_epoch += epochs;
	  break;
        }
      if (res_epoch >= GLOBAL_MAX * MAX)
        {
          if (threadNum == 0)
          {
            printf ("[ERROR] Algorithm does not converge (done %d iterations)!\n", res_epoch);
            temp.set_finish_calc();
	  }
	}
      if (threadNum == 0)
        {
          printf ("Algoritm converges : epochs = %d, residual = %e\n", res_epoch, resid);
        }
      pthread_barrier_wait (temp.barrier_for_calc);
      double residual;
      double max;
      get_residual_and_max (temp.x, temp.functions[temp.k], residual, max,
                            threadNum, p, nx, ny, point_simple);
      residual = get_max (residual, threadNum, p, temp.barrier_for_calc);
      pthread_barrier_wait (temp.barrier_for_calc);
      max      = get_max (max, threadNum, p, temp.barrier_for_calc);
      
      if (threadNum == 0)
        {
          temp.residual = residual;
          temp.max = max;
          get_triangles (temp.x,        temp.functions[temp.k], temp.bot,       temp.top,
                         temp.function, temp.approx,            temp.residuals, nx, ny);
	  temp.set_finish_calc();
       }
    }
  pthread_barrier_wait (temp.barrier);
  return nullptr;
}
