#include "utils.h"
#include "draw_tools.h"

double
get_residual (const point_2D & p1, const point_2D & p2, const point_2D & p3,
              const double f1, const double f2, const double f3,
              std::function <double (const point_2D & p)> f)
{
  double val_1 = fabs (f1 - f (p1));
  double val_2 = fabs (f2 - f (p2));
  double val_3 = fabs (f3 - f (p3));

  double val_4 = fabs ((f1 + f2) / 2.0 - f ((p1 + p2) / 2.0));
  double val_5 = fabs ((f1 + f3) / 2.0 - f ((p1 + p3) / 2.0));
  double val_6 = fabs ((f2 + f3) / 2.0 - f ((p2 + p3) / 2.0));

  double val_7 = fabs ((f1 + f2 + f3) / 3.0 - f ((p1 + p2 + p3) / 3.0));

  return std::max ({val_1, val_2, val_3, val_4, val_5, val_6, val_7});
}

void
get_residual_and_max (const std::unique_ptr<double[]> & x,
                      std::function <double (double, double)> func,
                      double & residual,   double & max,
	              const int threadNum, const int p, const int nx, const int ny,
	              std::function <point_2D (const int, const int)> simple_point)
{
  residual = 0.0;
  max = 0.0;
  auto f = [&] (const point_2D & p)
    {
      return func (p.x, p.y);
    };
  for (int i = threadNum; i < nx; i += p)
    {
      for (int j = 0; j < ny; j++)
        {
          residual = std::max (residual, get_residual (simple_point (i, j), simple_point (i + 1, j), simple_point (i, j + 1),
                                                       x[index (i, j, ny)], x[index (i + 1, j, ny)], x[ index (i, j + 1, ny)], f));
          max      = std::max ({max, fabs(f(simple_point (i, j))), fabs(f(simple_point (i + 1, j))), fabs(f(simple_point (i, j + 1)))});

          residual = std::max (residual, get_residual (simple_point (i + 1, j + 1), simple_point (i + 1, j), simple_point (i, j + 1),
                                                       x[index (i + 1, j + 1, ny)], x[index (i + 1, j, ny)], x[ index (i, j + 1, ny)], f));
          max      = std::max ({max, fabs(f(simple_point (i + 1, j + 1))), fabs(f(simple_point (i + 1, j))), fabs(f(simple_point (i, j + 1)))});
        }
    }
}

void
get_triangles (const std::unique_ptr<double[]> & x,
               std::function <double (double, double)> func,
	       const point_2D & bot, const point_2D & top,
               std::vector<std::array<point_3D, DIM>> & functions,
	       std::vector<std::array<point_3D, DIM>> & approx,
	       std::vector<std::array<point_3D, DIM>> & residual,
               const int nx, const int ny)
{
  int N_X = std::min (nx, MAX_VISUAL_N);
  int N_Y = std::min (ny, MAX_VISUAL_N);
  
  auto local_index = [&] (const int i, const int j)
    {
      return index (i * nx / N_X, j * ny / N_Y, ny);
    };

  auto local_point = [&] (const int i, const int j)
    {
      return point (i * nx / N_X, j * ny / N_Y, nx, ny, bot, top);
    };

  auto poly = [&] (const point_2D & p1, const point_2D & p2, const point_2D & p3,
                   const double f1, const double f2, const double f3)
    {
      auto f = [&] (const point_2D & p)
        {
          return func (p.x, p.y);
        };
      std::array<point_3D, DIM> temp = {point_3D(p1.x, p1.y, f (p1)),
	                                point_3D(p2.x, p2.y, f (p2)),
					point_3D(p3.x, p3.y, f (p3))};
      functions.push_back (temp);
      temp = {point_3D(p1.x, p1.y, f1),
	      point_3D(p2.x, p2.y, f2),
	      point_3D(p3.x, p3.y, f3)};
      approx.push_back (temp);
      temp = {point_3D(p1.x, p1.y, fabs (f (p1) - f1)),
	      point_3D(p2.x, p2.y, fabs (f (p2) - f2)),
	      point_3D(p3.x, p3.y, fabs (f (p3) - f3))};
      residual.push_back (temp);
    };

  functions.clear ();
  approx.clear ();
  residual.clear ();
  
  for (int i = 0; i < N_X; i++)
    {
      for (int j = 0; j < N_Y; j++)
        {
          poly (local_point   (i, j),  local_point   (i + 1, j),  local_point   (i, j + 1),
	        x[local_index (i, j)], x[local_index (i + 1, j)], x[local_index (i, j + 1)]);
          
	  poly (local_point   (i + 1, j + 1),  local_point (i + 1, j),    local_point   (i, j + 1),
	        x[local_index (i + 1, j + 1)], x[local_index (i + 1, j)], x[local_index (i, j + 1)]);
        }
    }
  return;
}
