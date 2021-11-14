#ifndef NELDER_MEAD_H
#define NELDER_MEAD_H

#include "optimset.h"
#include "point.h"
#include "cost.h"

//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------

#define RHO 1.0
#define CHI 2.0
#define GAMMA 0.5
#define SIGMA 0.5

// define a simplex struct containing an array of n+1 points (p)
// each having dimension (n)
typedef struct {
  point_t *p;
  int n;
} simplex_t;

//-----------------------------------------------------------------------------
// Nelder-Mead algorithm
//-----------------------------------------------------------------------------

void nelder_mead(int, const point_t *, point_t *, fun_t, const void *, const optimset_t *);

#endif // NELDER_MEAD_H
