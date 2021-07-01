#include "integrate.h"
	
double
integral (const point_2D & p1, const point_2D & p2, const point_2D & p3,
          double f1, double f2, double f3,
          double g1, double g2, double g3)
{
  double t1 = fabs ((p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y));
  double t2 = (f1 - f3) * (2.0 * g1 + g2 + g3) + (f2 - f3) * (g1 + 2.0 * g2 + g3) + 4.0 * f3 * (g1 + g2 + g3);

  return t1 * t2 / 24.0;
}

double
integral (const point_2D & p1, const point_2D & p2, const point_2D & p3,
          std::function <double (double, double)> func)
{
  point_2D p12 = (p1 + p2) / 2.0;
  point_2D p13 = (p1 + p3) / 2.0;
  point_2D p23 = (p2 + p3) / 2.0;
  auto f = [&] (const point_2D & p)
    {
      return func (p.x, p.y);
    };

  double i1 = integral (p1, p12, p13,  1.0,  0.5, 0.5, f (p1),   f (p12), f (p13));
  double i2 = integral (p2, p12, p23,  0.0,  0.5, 0.0, f (p2),   f (p12), f (p23));
  double i3 = integral (p3, p13, p23,  0.0,  0.5, 0.0, f (p3),   f (p13), f (p23));
  double i4 = integral (p12, p23, p13, 0.5,  0.0, 0.5, f (p12),  f (p23), f (p13));

  return i1 + i2 + i3 + i4;
}
