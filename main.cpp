#include "env.h"

int
main (int argc, char * argv [])
{
  feenableexcept (FE_INVALID|FE_DIVBYZERO|FE_OVERFLOW);
  if (argc != 7)
    {
      printf ("[FAIL] Use: %s FILE N_X N_Y K E P\n", argv[0]);
      return -1;
    }
  QApplication app(argc, argv);
  params init_parametrs;
  const char * file  = argv[1];  
  init_parametrs.nx  = atoi (argv[2]);
  init_parametrs.ny  = atoi (argv[3]);
  init_parametrs.k   = atoi (argv[4]);
  init_parametrs.eps = atof (argv[5]);
  init_parametrs.p   = atoi (argv[6]);

  if (init_parametrs.p <= 0)
    {
      printf ("[FAIL] Threads count (%d) <= 0\n", init_parametrs.p);
      return -1;
    }

  if (init_parametrs.nx <= 0 || init_parametrs.ny < 0 || init_parametrs.eps <= 0)
    {
      printf ("[FAIL] Incorrect inpit params\n");
    }

  auto values = read(file);
  if (!values)
    {
      printf("[FAIL] Input file is bad!!!\n");
      return 0;
    }
  pthread_barrier_t barrier_for_calc;
  pthread_barrier_t barrier;
  set_parametrs(init_parametrs, values.value(), &barrier, &barrier_for_calc);
  
  std::vector<std::pair<int, params &>> data;
  data.reserve(init_parametrs.p);
  
  for (int i = 0; i < init_parametrs.p; i++)
    {
      pthread_t tid;
      data.push_back ({i, init_parametrs});
      pthread_create (&tid, 0, execute, &data[i]);
    }
  
  QMainWindow *window = new QMainWindow;
  QMenuBar *tool_bar = new QMenuBar (window);
  Window *graph_area = new Window (init_parametrs, window);
  preinstall_env(window, tool_bar, graph_area);
  window->show ();
  int res = app.exec ();
  clean(init_parametrs);
  delete window;
  return res;
}
