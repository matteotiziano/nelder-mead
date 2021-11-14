#include <stdio.h>
#include "point.h"

//-----------------------------------------------------------------------------
// Utilities for point_t
//-----------------------------------------------------------------------------

void copy_point(int n, const point_t *src, point_t *dst) {
  for (int j = 0; j < n; j++) {
    dst->x[j] = src->x[j];
  }
  dst->fx = src->fx;
}

void swap_points(int n, point_t *p1, point_t *p2) {
  double temp;
  for (int j = 0; j < n; j++) {
    temp = p1->x[j];
    p1->x[j] = p2->x[j];
    p2->x[j] = temp;
  }
  temp = p1->fx;
  p1->fx = p2->fx;
  p2->fx = temp;
}

void print_point(int n, const point_t *point) {
  printf("x = [ ");
  for (int i = 0; i < n; i++) {
    printf("%.8f ", point->x[i]);
  }
  printf("], fx = %.8f \n", point->fx);
}
