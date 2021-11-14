#ifndef POINT_H
#define POINT_H

// define a generic point containing a position (x) and a value (fx)
typedef struct {
  double *x;
  double fx;
} point_t;

void copy_point(int, const point_t *, point_t *);

void swap_points(int, point_t *, point_t *);

void print_point(int, const point_t *);

#endif // POINT_H
