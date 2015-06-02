
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef MATLAB_MEX_FILE
#define PRINTF mexPrintf
#define MALLOC mxMalloc
#include "mex.h"
#include "matrix.h"
#else
#define PRINTF printf
#define MALLOC malloc
#endif

#define VERBOSE  0     // enable verbose output during minimization

#define TOL_X    0.001 // tolerance on the simplex solutions coordinates
#define TOL_F    0.001 // tolerance on the function value
#define MAX_ITER 1000  // maximum number of allowed iterations
#define MAX_EVAL 1000  // maximum number of allowed function evaluations

#define RHO      1
#define CHI      2
#define GAMMA    0.5
#define SIGMA    0.5

#define SQUARE(x) ((x)*(x))

// define the point in the simplex (x) and its functional value (fx)
typedef struct{
    double *x;
    double fx;
} point_t;

// define optimization settings
typedef struct{
    double tolx;
    double tolf;
    int max_iter;
    int max_eval;
	int verbose;
} optimset_t;

// Nelder-Mead algorithm and template cost function 
void nelder_mead(double *, int, optimset_t, point_t *, void *);
double cost_fun(int, const double *, void *);

// helper functions
int compare(const void *, const void *);
void get_centroid(int, point_t *, double *);
double modulus(double);
int continue_minimization(int, point_t *, int, int, optimset_t);
void get_point(int, double *, double *, double, double *);
void copy_point(int, double *, double *, double, double *);
void swap_points(int, point_t *, point_t *);
void print_min(int, point_t *);
