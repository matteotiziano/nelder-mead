#ifndef OPTIMSET_H
#define OPTIMSET_H

// define optimization settings
typedef struct {
  double tolx;    // tolerance on the simplex solutions coordinates
  double tolf;    // tolerance on the function value
  int max_iter;   // maximum number of allowed iterations
  int max_eval;   // maximum number of allowed function evaluations
  int verbose;    // toggle verbose output during minimization
} optimset_t;

#endif // OPTIMSET_H
