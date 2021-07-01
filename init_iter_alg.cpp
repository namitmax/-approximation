#include "utils.h"
#include "integrate.h"
#include "init_iter_alg.h"

	
void
init_matrix (std::unique_ptr<double[]> & a, std::unique_ptr<int[]> & index,
             const int n_x, const int n_y,
             const int k,
             std::function <int (const int, const int)> index_simple,
             std::function <point_2D (const int, const int)> point_simple)
{

  if (k != 0)
    {
      return;
    }

  int l = 0;
  index[l] = matrix_size (n_x, n_y) + 2;
  
  for (int i = 0; i <= n_x; i++)
    {
      for (int j = 0; j <= n_y; j++)
        {
          if (i == n_x && j == n_y)
            {
              a[l]                =   integral (point_simple(i, j), point_simple(i, j - 1),      point_simple(i - 1, j),
			                        1.0, 0.0, 0.0, 1.0, 0.0, 0.0);

              a[index[l] + 0]     =   integral (point_simple(i, j), point_simple(i, j - 1),      point_simple(i - 1, j),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 0] =   index_simple (i, j - 1);

              a[index[l] + 1]     =   integral (point_simple(i, j), point_simple(i - 1, j),      point_simple(i, j - 1),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 1] =   index_simple (i - 1, j);

              index[l + 1]        =   index[l] + 2;
            }
          else if (i == 0 && j == 0)
            {
              a[l]                =   integral (point_simple(i, j), point_simple(i, j + 1),      point_simple(i + 1, j),
                                                1.0, 0.0, 0.0, 1.0, 0.0, 0.0);
              a[index[l] + 0]     =   integral (point_simple(i, j), point_simple(i + 1, j),      point_simple(i, j + 1),
                                                1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 0] =   index_simple (i + 1, j);
              a[index[l] + 1]     =   integral (point_simple(i, j), point_simple(i, j + 1),      point_simple(i + 1, j),
                                                1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 1] =   index_simple (i, j + 1);
              index[l + 1]        =   index[l] + 2;
            }
          else if (i == n_x && j == 0)
            {
              a[l]                =   integral (point_simple(i, j), point_simple(i - 1, j),      point_simple(i - 1, j + 1),
			                        1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i - 1, j + 1),  point_simple(i, j + 1),
				                1.0, 0.0, 0.0, 1.0, 0.0, 0.0);

              a[index[l] + 0]     =   integral (point_simple(i, j), point_simple(i - 1, j),      point_simple(i - 1, j + 1),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 0] =   index_simple (i - 1, j);

              a[index[l] + 1]     =   integral (point_simple(i, j), point_simple(i - 1, j + 1),  point_simple(i - 1, j),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i - 1, j + 1),  point_simple(i, j + 1),
				     	        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 1] =   index_simple (i - 1, j + 1);

              a[index[l] + 2]     =   integral (point_simple(i, j), point_simple(i, j + 1),      point_simple(i - 1, j + 1),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 2] =   index_simple (i, j + 1);

              index[l + 1]        =   index[l] + 3;
            }
          else if (i == 0 && j == n_y)
            {
              a[l]                =   integral (point_simple(i, j), point_simple(i + 1, j),      point_simple(i + 1, j - 1), 
                                                1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i + 1, j - 1),  point_simple(i, j - 1),
				                1.0, 0.0, 0.0, 1.0, 0.0, 0.0);
              a[index[l] + 0]     =   integral (point_simple(i, j), point_simple(i + 1, j),      point_simple(i + 1, j - 1),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 0] =   index_simple (i + 1, j);
              a[index[l] + 1]     =   integral (point_simple(i, j), point_simple(i + 1, j - 1),  point_simple(i + 1, j),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i + 1, j - 1),  point_simple(i, j - 1),
                                                1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 1] =   index_simple (i + 1, j - 1);

              a[index[l] + 2]     =   integral (point_simple(i, j), point_simple(i, j - 1),      point_simple(i + 1, j - 1),
                                                1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 2] =   index_simple (i, j - 1);

              index[l + 1]        =   index[l] + 3;
            }
          else if (i == 0)
            {
              a[l]                =   integral (point_simple(i, j), point_simple(i + 1, j),      point_simple(i + 1, j - 1),
			                        1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i + 1, j - 1),  point_simple(i, j - 1),
				                1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i, j + 1),      point_simple(i + 1, j),
				                1.0, 0.0, 0.0, 1.0, 0.0, 0.0);

              a[index[l] + 0]     =   integral (point_simple(i, j), point_simple(i + 1, j),      point_simple(i, j + 1),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i + 1, j),      point_simple(i + 1, j - 1),
			  		        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 0] =   index_simple (i + 1, j);

              a[index[l] + 1]     =   integral (point_simple(i, j), point_simple(i + 1, j - 1),  point_simple(i + 1, j),
			                       1.0, 0.0, 0.0, 0.0, 1.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i + 1, j - 1),  point_simple(i, j - 1),
				        	1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 1] =   index_simple (i + 1, j - 1);

              a[index[l] + 2]     =   integral (point_simple(i, j), point_simple(i, j - 1),      point_simple(i + 1, j - 1),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 2] =   index_simple (i, j - 1);

              a[index[l] + 3]     =   integral (point_simple(i, j), point_simple(i, j + 1),      point_simple(i + 1, j),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 3] =   index_simple (i, j + 1);

              index[l + 1]        =   index[l] + 4;
            }
          else if (j == 0)
            {
              a[l]                =   integral (point_simple(i, j), point_simple(i - 1, j),      point_simple(i - 1, j + 1),
	                		        1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i - 1, j + 1),  point_simple(i, j + 1),
			          	        1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i, j + 1),      point_simple(i + 1, j),
				                1.0, 0.0, 0.0, 1.0, 0.0, 0.0);

              a[index[l] + 0]     =   integral (point_simple(i, j), point_simple(i + 1, j),      point_simple(i, j + 1),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 0] =   index_simple (i + 1, j);

              a[index[l] + 1]     =   integral (point_simple(i, j), point_simple(i - 1, j),      point_simple(i - 1, j + 1),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 1] =   index_simple (i - 1, j);

              a[index[l] + 2]     =   integral (point_simple(i, j), point_simple(i - 1, j + 1),  point_simple(i - 1, j),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i - 1, j + 1),  point_simple(i, j + 1),
				   	        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 2] =   index_simple (i - 1, j + 1);

              a[index[l] + 3]     =   integral (point_simple(i, j), point_simple(i, j + 1),      point_simple(i - 1, j + 1),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i, j + 1),      point_simple(i + 1, j),
				                1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 3] =   index_simple (i, j + 1);

              index[l + 1]        =   index[l] + 4;
            }
          else if (i == n_x)
            {
              a[l]                =   integral (point_simple(i, j),  point_simple(i, j - 1),     point_simple(i - 1, j),
			                        1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
                                    + integral (point_simple(i, j),  point_simple(i - 1, j),     point_simple(i - 1, j + 1),
				                1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
                                    + integral (point_simple(i, j),  point_simple(i - 1, j + 1), point_simple(i, j + 1),
			                	1.0, 0.0, 0.0, 1.0, 0.0, 0.0);

              a[index[l] + 0]     =   integral (point_simple(i, j),  point_simple(i, j - 1),     point_simple(i - 1, j),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 0] =   index_simple (i, j - 1);

              a[index[l] + 1]     =   integral (point_simple(i, j),  point_simple(i - 1, j),     point_simple(i, j - 1),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0)
                                    + integral (point_simple(i, j),  point_simple(i - 1, j),     point_simple(i - 1, j + 1),
				        	1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 1] =   index_simple (i - 1, j);

              a[index[l] + 2]     =   integral (point_simple(i, j),  point_simple(i - 1, j + 1), point_simple(i - 1, j),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0)
                                    + integral (point_simple(i, j),  point_simple(i - 1, j + 1), point_simple(i, j + 1),
				  	        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 2] =   index_simple (i - 1, j + 1);

              a[index[l] + 3]     =   integral (point_simple(i, j),  point_simple(i, j + 1),     point_simple(i - 1, j + 1),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 3] = index_simple (i, j + 1);

              index[l + 1] = index[l] + 4;
            }
          else if (j == n_y)
            {
              a[l]               =    integral (point_simple(i, j),  point_simple(i + 1, j),     point_simple(i + 1, j - 1),
			                       1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
                                    + integral (point_simple(i, j),  point_simple(i + 1, j - 1), point_simple(i, j - 1),
				               1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
                                    + integral (point_simple(i, j),  point_simple(i, j - 1),     point_simple(i - 1, j),
				               1.0, 0.0, 0.0, 1.0, 0.0, 0.0);

              a[index[l] + 0]     =   integral (point_simple(i, j), point_simple(i + 1, j),      point_simple(i + 1, j - 1),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 0] =   index_simple (i + 1, j);

              a[index[l] + 1]     =   integral (point_simple(i, j), point_simple(i + 1, j - 1),  point_simple(i + 1, j),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i + 1, j - 1),  point_simple(i, j - 1),
					        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 1] =   index_simple (i + 1, j - 1);

              a[index[l] + 2]     =   integral (point_simple(i, j), point_simple(i, j - 1),      point_simple(i + 1, j - 1),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i, j - 1),      point_simple(i - 1, j),
				       	        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 2] =   index_simple (i, j - 1);

              a[index[l] + 3]     =   integral (point_simple(i, j), point_simple(i - 1, j),      point_simple(i, j - 1),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 3] =   index_simple (i - 1, j);

              index[l + 1]        =   index[l] + 4;
            }
          else
            {
              a[l]                =   integral (point_simple(i, j), point_simple(i + 1, j),      point_simple(i + 1, j - 1),
			                        1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i + 1, j - 1),  point_simple(i, j - 1),
                                                1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i, j - 1),      point_simple(i - 1, j),
				                1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i - 1, j),      point_simple(i - 1, j + 1),
				                1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i - 1, j + 1),  point_simple(i, j + 1),
                                                1.0, 0.0, 0.0, 1.0, 0.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i, j + 1),      point_simple(i + 1, j),
				                1.0, 0.0, 0.0, 1.0, 0.0, 0.0);

              a[index[l] + 0]     =   integral (point_simple(i, j), point_simple(i + 1, j),      point_simple(i, j + 1),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i + 1, j),      point_simple(i + 1, j - 1),
					        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 0] =   index_simple (i + 1, j);

              a[index[l] + 1]     =   integral (point_simple(i, j), point_simple(i + 1, j - 1),  point_simple(i + 1, j),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i + 1, j - 1),  point_simple(i, j - 1),
				    	        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 1] =   index_simple (i + 1, j - 1);

              a[index[l] + 2]     =   integral (point_simple(i, j), point_simple(i, j - 1),      point_simple(i + 1, j - 1),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i, j - 1),      point_simple(i - 1, j),
					        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 2] = index_simple (i, j - 1);

              a[index[l] + 3]     =   integral (point_simple(i, j), point_simple(i - 1, j),      point_simple(i, j - 1),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i - 1, j),      point_simple(i - 1, j + 1),
					        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 3] =   index_simple (i - 1, j);

              a[index[l] + 4]     =   integral (point_simple(i, j), point_simple(i - 1, j + 1),  point_simple(i - 1, j),
			                        1.0, 0.0, 0.0, 0.0, 1.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i - 1, j + 1),  point_simple(i, j + 1),
					        1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 4] =   index_simple (i - 1, j + 1);

              a[index[l] + 5]     =   integral (point_simple(i, j), point_simple(i, j + 1),      point_simple(i - 1, j + 1),
			                       1.0, 0.0, 0.0, 0.0, 1.0, 0.0)
                                    + integral (point_simple(i, j), point_simple(i, j + 1),      point_simple(i + 1, j),
					       1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
              index[index[l] + 5] =   index_simple (i, j + 1);

              index[l + 1]        =   index[l] + 6;
            }
          l++;
        }
    }
}

void
init_b (std::unique_ptr<double[]> & b, std::function <double (double, double)> func,
          const int n_x, const int n_y,
          const int k, const int p,
          std::function <int (const int, const int)> index_simple,
	  std::function <point_2D (const int, const int)> point_simple)
{
  for (int i = k; i <= n_x; i += p)
    {
      int l = index_simple (i, 0);
      for (int j = 0; j <= n_y; j++)
        {
          if (i == 0 && j == 0)
            {
              b[l] =   integral (point_simple (i, j), point_simple (i, j + 1),     point_simple (i + 1, j),     func);
            }
          else if (i == 0 && j == n_y)
            {
              b[l] =   integral (point_simple (i, j), point_simple (i + 1, j),     point_simple (i + 1, j - 1), func)
                     + integral (point_simple (i, j), point_simple (i + 1, j - 1), point_simple (i, j - 1),     func);
            }
          else if (i == n_x && j == 0)
            {
              b[l] =   integral (point_simple (i, j), point_simple (i - 1, j),     point_simple (i - 1, j + 1), func)
                     + integral (point_simple (i, j), point_simple (i - 1, j + 1), point_simple (i, j + 1),     func);
            }
          else if (i == n_x && j == n_y)
            {
              b[l] =   integral (point_simple (i, j), point_simple (i, j - 1),     point_simple (i - 1, j),     func);
            }
          else if (i == 0)
            {
              b[l] =   integral (point_simple (i, j), point_simple (i + 1, j),     point_simple (i + 1, j - 1), func)
                     + integral (point_simple (i, j), point_simple (i + 1, j - 1), point_simple (i, j - 1),     func)
                     + integral (point_simple (i, j), point_simple (i, j + 1),     point_simple (i + 1, j),     func);
            }
          else if (j == 0)
            {
              b[l] =   integral (point_simple (i, j), point_simple (i - 1, j),     point_simple (i - 1, j + 1), func)
                     + integral (point_simple (i, j), point_simple (i - 1, j + 1), point_simple (i, j + 1),     func)
                     + integral (point_simple (i, j), point_simple (i, j + 1),     point_simple (i + 1, j),     func);
            }
          else if (i == n_x)
            {
              b[l] =   integral (point_simple (i, j), point_simple (i, j - 1),     point_simple (i - 1, j),     func)
                     + integral (point_simple (i, j), point_simple (i - 1, j),     point_simple (i - 1, j + 1), func)
                     + integral (point_simple (i, j), point_simple (i - 1, j + 1), point_simple (i, j + 1),     func);
            }
          else if (j == n_y)
            {
              b[l] =   integral (point_simple (i, j), point_simple (i + 1, j),     point_simple (i + 1, j - 1), func)
                     + integral (point_simple (i, j), point_simple (i + 1, j - 1), point_simple (i, j - 1),     func)
                     + integral (point_simple (i, j), point_simple (i, j - 1),     point_simple (i - 1, j),     func);
            }
          else
            {
              b[l] =   integral (point_simple (i, j), point_simple (i + 1, j),     point_simple (i + 1, j - 1), func)
                     + integral (point_simple (i, j), point_simple (i + 1, j - 1), point_simple (i, j - 1),     func)
                     + integral (point_simple (i, j), point_simple (i, j - 1),     point_simple (i - 1, j),     func)
                     + integral (point_simple (i, j), point_simple (i - 1, j),     point_simple (i - 1, j + 1), func)
                     + integral (point_simple (i, j), point_simple (i - 1, j + 1), point_simple (i, j + 1),     func)
                     + integral (point_simple (i, j), point_simple (i, j + 1),     point_simple (i + 1, j),     func);
            }
          l++;
        }
    }
}
