#include <stdio.h>
#include <stdlib.h>

#include "ackley.h"
#include "nelder_mead.h"

#define PI 3.1415926535897932384626433832795

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------

int main(int argc, const char *argv[]) {
  if (argc == 1) {
    printf("%s: error: not enough inputs \n", argv[0]);
    return 0;
  }

  // reading initial point from command line
  const int n = argc - 1;
  point_t start;
  start.x = malloc(n * sizeof(double));
  for (int i = 0; i < n; i++) {
    start.x[i] = atof(argv[i + 1]);
  }

  // optimisation settings
  optimset_t optimset;
  optimset.tolx = 0.001;
  optimset.tolf = 0.001;
  optimset.max_iter = 1000;
  optimset.max_eval = 1000;
  optimset.verbose = 1;

  // cost function parameters
  ackley_param_t ackley_params;
  ackley_params.a = 20.0;
  ackley_params.b = 0.2;
  ackley_params.c = 2.0 * PI;

  // call optimization method
  point_t solution;
  nelder_mead(n, &start, &solution, &ackley_fun, &ackley_params, &optimset);

  // evaluate and print starting point
  printf("Initial point\n");
  ackley_fun(n, &start, &ackley_params);
  print_point(n, &start);
  // print solution
  printf("Solution\n");
  print_point(n, &solution);

  // free memory
  free(start.x);
  free(solution.x);

  return 0;
}
