#ifndef WINDOW_H
#define WINDOW_H

#include <assert.h>
#include <pthread.h>
#include <math.h>

#include <QWidget>
#include <QTimer>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QPolygon>
#include <QFont>
#include <QKeyEvent>
#include <QPainter>
#include <QPainterPath>
#include <vector>
#include <functional>
#include <string>
#include "params.h"

#define PI_DEGREE 180
#define MAX_ANG_COUNT 24
#define EPS 1e-12

class Window : public QWidget
{
  Q_OBJECT

public:

  Window (params & initial, QWidget * parent = nullptr);

public slots:
  
  void change_func();
  void change_method();
  void scale_mutilpyX2();
  void scale_divideX2();
  void more_dots();
  void less_dots();
  void plus_error();
  void minus_error();
  void rotate_plus();
  void rotate_minus();
  void check_if_ready ();

private:
  
  params &parametrs;
  std::vector<point_2D>    border = {};
  std::vector<std::string> names  = {};
  void paintEvent (QPaintEvent * event);

  QPen   pen;
  QBrush brush;

  bool   block    = false;
  int    method   = 0;
  int    ang      = 0;
  int    nx       = 1;
  int    ny       = 1;
  int    k        = 0;
  double temp_cos = 1.0;
  double temp_sin = 0.0;
  double error    = 0.0;

  double x_min    = 0.0;
  double x_max    = 0.0;
  double y_min    = 0.0;
  double y_max    = 0.0;
  double max_norm = 0.0;
  double zoom     = 1.0;
  double temp_max = 0.0;
  double residual = 0.0;
  double max      = 0.0;

  std::vector<std::array<point_3D, DIM>> function  = {};
  std::vector<std::array<point_3D, DIM>> approx    = {};
  std::vector<std::array<point_3D, DIM>> residuals = {};

  QTimer timer_for_calc;

  point_2D point_3D_to_2D (const point_3D & p) const;
  std::optional<QPoint> scale (const point_2D & p) const;
  void draw_figure (QPainter & painter, const QPolygon & polygon);
  void draw_box    (QPainter & painter);
  void draw        (QPainter & painter, std::vector<std::array<point_3D, DIM>> & f);

  point_2D rotate (const point_2D & point) const;
  void update_ang (const int sign);
  double get_dot_priority(const std::array<point_3D, DIM> & t) const;

  void sort_triangles (std::vector<std::array<point_3D, DIM>> & data) const;

  void init_border ();
  void calc_border ();

  void scale_multiply_X2 ();
  void scale_divide_X2 ();

  void wait_update ();

  void set_title (QPainter &painter, const double module_max);
};

#endif // WINDOW_H
