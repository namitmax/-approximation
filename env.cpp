#include "env.h"

void clean(params & parametrs)
{
  parametrs.set_finish();
  parametrs.unlock();
  pthread_barrier_wait (parametrs.barrier);
  pthread_barrier_destroy (parametrs.barrier);
  pthread_barrier_destroy (parametrs.barrier_for_calc);
}

void preinstall_env(QMainWindow *window, QMenuBar *tool_bar, Window *graph_area)
{
  QAction *action = nullptr;

  action = tool_bar->addAction ("Change function (0)", graph_area, SLOT (change_func ()));
  action->setShortcut (QString ("0"));

  action = tool_bar->addAction ("Change method (1)", graph_area, SLOT (change_method ()));
  action->setShortcut (QString ("1"));

  action = tool_bar->addAction ("Scale /= 2 (2)", graph_area, SLOT (scale_mutilpyX2 ()));
  action->setShortcut (QString ("2"));

  action = tool_bar->addAction ("Scale *= 2 (3)", graph_area, SLOT (scale_divideX2 ()));
  action->setShortcut (QString ("3"));

  action = tool_bar->addAction ("Increase dots x2 (4)", graph_area, SLOT (more_dots ()));
  action->setShortcut (QString ("4"));

  action = tool_bar->addAction ("Decrease dots x2 (5)", graph_area, SLOT (less_dots ()));
  action->setShortcut (QString ("5"));

  action = tool_bar->addAction ("Plus delta-function (6)", graph_area, SLOT (plus_error ()));
  action->setShortcut (QString ("6"));

  action = tool_bar->addAction ("Minus delta-function (7)", graph_area, SLOT (minus_error ()));
  action->setShortcut (QString ("7"));

  action = tool_bar->addAction ("Rotate clockwise (8)", graph_area, SLOT (rotate_minus ()));
  action->setShortcut (QString ("8"));

  action = tool_bar->addAction ("Rotate anticlockwise (9)", graph_area, SLOT (rotate_plus ()));
  action->setShortcut (QString ("9"));

  action = tool_bar->addAction ("Exit (Esc)", window, SLOT (close ()));
  action->setShortcut (QString ("Escape"));

  tool_bar->setMaximumHeight (30);
  window->setMenuBar (tool_bar);
  window->setCentralWidget (graph_area);
  window->setWindowTitle ("Graph");
}

void set_parametrs(params & init, std::vector<std::pair<double, double>> & init_dots,
                   pthread_barrier_t * barrier, pthread_barrier_t * barrier_for_calc)
{
  if (init_dots[0].second < init_dots[1].second)
    {
      init.bot = init_dots[0];
      init.top = init_dots[1];
    }
  else
    {
      init.bot = init_dots[1];
      init.top = init_dots[0];
    }
  pthread_barrier_init (barrier, nullptr, init.p + 1);
  pthread_barrier_init (barrier_for_calc, nullptr, init.p);
  init.barrier = barrier;
  init.barrier_for_calc = barrier_for_calc;
}
