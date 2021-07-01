#ifndef CREATE_ENV_H
#define CREATE_ENV_H

#include <stdlib.h>
#include <sys/sysinfo.h>
#include <pthread.h>
#include <fenv.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>

#include <QApplication>
#include <QStyleFactory>

#include "read_file.h"
#include "window.h"
#include "executor.h"

void clean(params & parametrs);
void preinstall_env(QMainWindow *window, QMenuBar *tool_bar, Window *graph_area);
void set_parametrs(params & init, std::vector<std::pair<double, double>> & init_dots,
                   pthread_barrier_t * barrier, pthread_barrier_t * barrier_for_calc);


#endif // CREATE_ENV_H
