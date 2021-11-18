#include <stdio.h>
#include <string.h>
#include "point.h"

//-----------------------------------------------------------------------------
// Utilities for point_t
//-----------------------------------------------------------------------------

void swap_points(point_t *p1, point_t *p2) {
  point_t temp = *p1;
  *p1 = *p2;
  *p2 = temp;
}

void copy_point(int n, const point_t *src, point_t *dst) {
  memcpy(dst->x, src->x, sizeof(double) * n);
  dst->fx = src->fx;
}

void print_point(int n, const point_t *point) {
  printf("x = [ ");
  for (int i = 0; i < n; i++) {
    printf("%.8f ", point->x[i]);
  }
  printf("], fx = %.8f \n", point->fx);
}
