#include "window.h"

Window::Window (params & initial, QWidget * parent)
  : QWidget (parent),  parametrs(initial)
{
  setMinimumSize (500, 500);
  ang = 0.0;
  temp_sin = 0.0;
  temp_cos = 1.0;
  method = 0;
  x_min = 0.0;
  x_max = 0.0;
  y_min = 0.0;
  y_max = 0.0;
  max_norm = 0.0;
  
  timer_for_calc.setInterval   (50);
  timer_for_calc.setSingleShot (false);
  connect (&timer_for_calc, SIGNAL (timeout ()), this, SLOT (check_if_ready ()));
  
  names.push_back("f(x,y) = 1");
  names.push_back("f(x,y) = x");
  names.push_back("f(x,y) = y");
  names.push_back("f(x,y) = x + y");
  names.push_back("f(x,y) = (x^2 + y^2) ^ (1/2)");
  names.push_back("f(x,y) = x^2 + y^2");
  names.push_back("f(x,y) = exp(x^2 - y^2)");
  names.push_back("f(x,y) = 1 / (25(x^2 + y^2) + 1)");
  pen = QPen (Qt::black, 2, Qt::SolidLine);
  brush.setColor (Qt::white);
  brush.setStyle (Qt::SolidPattern);
  
  init_border ();
  wait_update ();
}

void
Window::check_if_ready ()
{
  if (block && !parametrs.is_calc)
    {
       nx       = parametrs.nx;
       ny       = parametrs.ny;
       k        = parametrs.k;
       max      = parametrs.max;
       residual = parametrs.residual;
       std::swap (function , parametrs.function);
       std::swap (approx   , parametrs.approx);
       std::swap (residuals, parametrs.residuals);
       block = false;
       update ();
    }
  else if (block)
    {
      timer_for_calc.start();
    }
}

void
Window::update_ang (const int sign)
{
  ang = (ang + sign) % MAX_ANG_COUNT;
}

void
Window::wait_update ()
{
  block = true;
  parametrs.unlock();
  timer_for_calc.setInterval (std::min(std::max(nx, ny), 100));
  timer_for_calc.start ();
}

void
Window::change_func()
{
  if (block)
    return;
  parametrs.k = (parametrs.k + 1) % parametrs.functions.size();
  wait_update ();
}

void
Window::change_method()
{
  method = (method + 1) % 3;
  update ();
}

void
Window::scale_mutilpyX2()
{
  zoom *= 2.0;
  init_border ();
  update();
}

void
Window::scale_divideX2()
{
  zoom /= 2.0;
  init_border ();
  update();
}

void
Window::more_dots()
{
  if (block)
    return;
  parametrs.nx = (parametrs.nx * 2);
  parametrs.ny = (parametrs.ny * 2);
  wait_update ();
}

void
Window::less_dots()
{
  if (block)
    return;
  parametrs.nx = (std::max (1, parametrs.nx / 2));
  parametrs.ny = (std::max (1, parametrs.ny / 2));
  wait_update ();
}

void
Window::rotate_plus()
{
  update_ang(1);
  update ();
}

void
Window::rotate_minus()
{
  update_ang(-1);
  update ();
}

void
Window::plus_error()
{
  error += 0.1;
  update ();
}

void
Window::minus_error()
{
  error -= 0.1;
  update ();
}

void Window::set_title (QPainter &painter, const double module_max)
{
  char buffer[32];
  memset(buffer, 0, sizeof(buffer));
  printf("max{|F_min|, |F_max|} = %e\n", (module_max ));
  snprintf(buffer, sizeof(buffer), "%e", (module_max ));
  std::string temp(buffer);
  painter.setPen ("blue");
  std::string temp_1 = "k = " + std::to_string(k) + " "
	               + names[k];
  painter.drawText (0, 20, temp_1.c_str());
  temp = "max{|F_min|, |F_max|} = " + temp;
  painter.drawText (0, 40, temp.c_str());
  temp = "Scale = " + std::to_string(zoom);
  painter.drawText (0, 60, temp.c_str());
  temp = "Number of dots: n_x =  " + std::to_string(nx) + " n_y = " 
	 + std::to_string(ny);
  painter.drawText (0, 80, temp.c_str());
  temp = "Perturbation = " + std::to_string(error);
  painter.drawText (0, 100, temp.c_str());
  double ang_to_rad = (ang * 2 * PI_DEGREE / MAX_ANG_COUNT) % (2 * PI_DEGREE);
  temp = "Angle = " + std::to_string(ang_to_rad);
  painter.drawText (0, 120, temp.c_str());
}

void
Window::calc_border ()
{
  x_min = y_min =  1e300;
  x_max = y_max = -1e300;

  point_2D temp;
  for (auto & dot : border)
    {
      temp  = rotate (dot);
      x_min = std::min (x_min, point_3D_to_2D (point_3D (temp.x, temp.y,  0.0)).x);
      x_max = std::max (x_max, point_3D_to_2D (point_3D (temp.x, temp.y,  0.0)).x);
      y_min = std::min (y_min, point_3D_to_2D (point_3D (temp.x, temp.y, -1.0)).y);
      y_max = std::max (y_max, point_3D_to_2D (point_3D (temp.x, temp.y,  1.0)).y);
    }
}

void
Window::paintEvent (QPaintEvent *)
{
  QPainter painter (this);

  draw_box (painter);
  
  double angle_to_rad = ang * 2.0 * M_PI / MAX_ANG_COUNT;

  temp_sin = sin (angle_to_rad);
  temp_cos = cos (angle_to_rad);
  
  if (method != 2)
    {
      max_norm = max;
    }
  else
    {
      max_norm = residual;
    }

  max_norm = std::max (max_norm, EPS);
  temp_max = max_norm;
  calc_border ();

  switch (method)
    {
    case 0:
      pen = QPen (Qt::green, 1.5, Qt::SolidLine);
      painter.setPen (pen);
      draw (painter, function);
      break;
    case 1:
      pen = QPen (Qt::blue, 1.5, Qt::SolidLine);
      painter.setPen (pen);
      draw (painter, approx);
      break;
    case 2:
      pen = QPen (Qt::red, 1.5, Qt::SolidLine);
      painter.setPen (pen);
      draw (painter, residuals);
      break;
    }
 set_title(painter, max_norm);
}

point_2D
Window::point_3D_to_2D (const point_3D & p) const
{
  double u = 0., v = 0.;
  static double t_ang = M_PI / 6.0;
  static double local_cos = cos (t_ang);
  static double local_sin = sin (t_ang);

  point_2D middle = (parametrs.top + parametrs.bot) / 2;
  if (fabs(p.x - middle.x) < EPS && fabs(p.y - middle.y) < EPS)
    {
      v += error * temp_max;
    }

  u += -local_cos * p.x;
  v += -local_sin * p.x;
  
  u +=  local_cos * p.y;
  v += -local_sin * p.y;
  
  v += p.z;

  return {u, v};
}

std::optional<QPoint>
Window::scale (const point_2D & p) const
{
  int w = width () - 1;
  int h = height () - 1;  
  if (p.x + 1e-3 < x_min or y_max < p.y - 1e-3 or p.x - 1e-3 > x_max or y_min > p.y + 1e-3)
   {
     return {};
   }
  int x = w * (p.x - x_min) / (x_max - x_min);
  int y = h * (y_max - p.y) / (y_max - y_min);

  return QPoint (x, y);
}

void
Window::draw_figure (QPainter & painter, const QPolygon & polygon)
{
  QPainterPath path;
  path.addPolygon (polygon);
  painter.fillPath (path, brush);
  painter.drawPolygon (polygon);
}

void
Window::draw_box (QPainter & painter)
{
  QPolygon polygon;
  int margin = 15;
  polygon.push_back (QPoint (-margin,            -margin));
  polygon.push_back (QPoint (width () + margin,  -margin));
  polygon.push_back (QPoint (width () + margin,  height () + margin));
  polygon.push_back (QPoint (-margin,            height () + margin));
  draw_figure       (painter, polygon);
}


void
Window::init_border ()
{
  border.clear();
  point_2D center = (parametrs.top - parametrs.bot) / 2 + parametrs.bot;
  double radius   = zoom * (parametrs.top - parametrs.bot).get_norm() / 2.0;
  point_2D border_point;
  for (int i = 0; i < PI_DEGREE; i++)
    {
      border_point = std::pair<double, double>({cos (i * 2.0 * M_PI / PI_DEGREE), sin (i * 2.0 * M_PI / PI_DEGREE)});
      border_point *= radius;
      border.push_back (center + border_point);
    }
}



void
Window::draw (QPainter & painter, std::vector<std::array<point_3D, DIM>> & f)
{

  sort_triangles (f);

  auto get_point = [&] (const point_3D & p)
    {
      point_2D rot = rotate ({p.x, p.y});
      double f = (fabs (p.z) < EPS ? 0.0 : p.z);
      return scale (point_3D_to_2D (point_3D (rot.x, rot.y, f / max_norm)));
    };

  QPolygon p (3);
  for (auto & t : f)
    {
      auto t_1 = get_point (t[0]);
      auto t_2 = get_point (t[1]);
      auto t_3 = get_point (t[2]);
      if (!t_1 or !t_2 or !t_3)
        continue;
      p[0] = t_1.value();
      p[1] = t_2.value();
      p[2] = t_3.value();
      draw_figure (painter, p);
    };
}

point_2D
Window::rotate (const point_2D & p) const
{
  return {p.x * temp_cos - p.y * temp_sin, p.x * temp_sin + p.y * temp_cos};
}


double
Window::get_dot_priority(const std::array<point_3D, DIM> & t) const
{
  point_2D a = rotate ({t[0].x, t[0].y});
  point_2D b = rotate ({t[1].x, t[1].y});
  point_2D c = rotate ({t[2].x, t[2].y});
  double w_1 = a.x + b.x + c.x;
  double w_2 = a.y + b.y + c.y;
  return w_1 + w_2;
}

void
Window::sort_triangles (std::vector<std::array<point_3D, DIM>> & data) const
{
  std::sort (
    data.begin (), data.end (), 
    [&] (const std::array<point_3D, DIM> & left, const std::array<point_3D, DIM> & right)
    {
      return get_dot_priority (left) < get_dot_priority (right);
    }
  );
}
