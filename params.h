#ifndef PARAMS_H
#define PARAMS_H

#include <pthread.h>
#include <math.h>
#include <cstdio>
#include <vector>
#include <memory>
#include <functional>

#define DIM 3
#define MAX_VISUAL_N 64

struct point_2D
{
  double x = 0;
  double y = 0;

  point_2D () = default;

  point_2D (const double other_x, const double other_y):
   x(other_x), y(other_y) {}

  point_2D (const point_2D& point):
    x(point.x), y(point.y) {}

  point_2D
  operator+ (const point_2D& point) const
  {
    return {x + point.x, y + point.y};
  }

  point_2D
  operator- (const point_2D& point) const
  {
    return {x - point.x, y - point.y};
  }

  point_2D&
  operator = (const point_2D& point)
  {
    x = point.x;
    y = point.y;
    return (*this);
  }

  point_2D&
  operator = (const std::pair<double, double> point)
  {
    x = point.first;
    y = point.second;
    return (*this);
  }

  point_2D&
  operator *= (const double denom)
  {
    x *= denom;
    y *= denom;
    return (*this);
  }

  point_2D&
  operator /= (const double denom)
  {
    if (fabs(denom) < 1e-16)
      {
        printf("[ERROR] Division by zero is impossible on this device!\n");
        return (*this);
      }
    x /= denom;
    y /= denom;
    return (*this);
  }

  point_2D
  operator / (const double denom) const
  {
    point_2D tmp(*this);
    if (fabs(denom) < 1e-16)
      {
        printf("[ERROR] Division by zero is impossible on this device!\n");
        return tmp;
      }
    tmp.x /= denom;
    tmp.y /= denom;
    return tmp;
  }

  double
  get_norm () const
  {
    return sqrt(x * x + y * y);
  }
};

struct point_3D
{
  double x = 0;
  double y = 0;
  double z = 0;

  point_3D (const double other_x,
            const double other_y,
            const double other_z)
  {
    x = other_x;
    y = other_y;
    z = other_z;
  }

  point_3D
  operator+ (const point_3D& point)
  {
    return {x + point.x, y + point.y, z + point.z};
  }

  point_3D
  operator- (const point_3D& point)
  {
    return {x - point.x, y - point.y, z - point.z};
  }
};

struct params
{
  params ():
      mutex       (PTHREAD_MUTEX_INITIALIZER),
      ready_mutex (PTHREAD_MUTEX_INITIALIZER),
      calc_mutex  (PTHREAD_MUTEX_INITIALIZER),
      is_ready    (PTHREAD_COND_INITIALIZER)
    {
      functions.push_back ([] (double  , double  ) { return 1.0; });
      functions.push_back ([] (double x, double  ) { return x; });
      functions.push_back ([] (double  , double y) { return y; });
      functions.push_back ([] (double x, double y) { return x + y; });
      functions.push_back ([] (double x, double y) { return sqrt (x * x + y * y); });
      functions.push_back ([] (double x, double y) { return x * x + y * y; });
      functions.push_back ([] (double x, double y) { return exp (x * x - y * y); });
      functions.push_back ([] (double x, double y) { return 1.0 / (25.0 * (x * x + y * y) + 1); });
    }
  
  void lock()
    {
      pthread_cond_wait(&is_ready, &ready_mutex);
    }

  void unlock()
   {
     pthread_cond_broadcast(&is_ready);
   }

  void set_start_calc()
  {
    pthread_mutex_lock (const_cast<pthread_mutex_t*> (&calc_mutex));
    is_calc = true;
    pthread_mutex_unlock (const_cast<pthread_mutex_t*> (&calc_mutex));
  }

  void set_finish_calc()
  {
    pthread_mutex_lock (const_cast<pthread_mutex_t*> (&calc_mutex));
    is_calc = false;
    pthread_mutex_unlock (const_cast<pthread_mutex_t*> (&calc_mutex));
  }
  
  void set_finish() 
  {  
    pthread_mutex_lock (const_cast<pthread_mutex_t*> (&mutex));
    finish = true;
    pthread_mutex_unlock (const_cast<pthread_mutex_t*> (&mutex));
  }
  
  std::vector<std::function<double (double, double)>> functions = {};
  std::vector<std::array<point_3D, DIM>> function  = {};
  std::vector<std::array<point_3D, DIM>> approx    = {};
  std::vector<std::array<point_3D, DIM>> residuals = {};
  std::unique_ptr<double[]> a;
  std::unique_ptr<int[]>    index;
  std::unique_ptr<double[]> b;
  std::unique_ptr<double[]> x;
  std::unique_ptr<double[]> temp_1;
  std::unique_ptr<double[]> temp_2;
  std::unique_ptr<double[]> temp_3;
  point_2D top = {};
  point_2D bot = {};
  int nx = 0;
  int ny = 0;
  int k = 0;
  int p = 1;
  double eps = 1.;
  double max = 0.;
  double residual = 0.;
  bool finish  = false;
  bool is_calc = false;
  pthread_barrier_t * barrier;
  pthread_barrier_t * barrier_for_calc;
  
  private:
    int             status = 0;
    int             calc_status = 0;
    pthread_mutex_t mutex;
    pthread_mutex_t ready_mutex;
    pthread_mutex_t calc_mutex;
    pthread_cond_t  is_ready;
};

#endif // PARAMS_H
