#ifndef ACKLEY_H
#define ACKLEY_H

#include "point.h"

//-----------------------------------------------------------------------------
// Implementation of a cost function f : R^n->R compatible with the fun_t  
// interface defined in cost.h; here we use the Ackley Function as it allows 
// us to demonstrate the use of the optional fixed arguments.
//-----------------------------------------------------------------------------

typedef struct {
  double a;
  double b;
  double c;
} ackley_param_t;

void ackley_fun(int, point_t *, const void *);

#endif // ACKLEY_H
