
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "nelder_mead.h"

/*
 * Implementation of Nelder-Mead method for direct uncontrained optimization
 *   mdl:  model to optimize
 *   opt:  optimization settings
 *   smpl: simplex used for optimization (updated on exit)
 *   out:  minimizer found by the optimization (output argument)
 */
void nelder_mead (const model *mdl, const optimset *opt, simplex *smpl, point *out) {
    // initialize Nelder-Mead parameters
    const real alpha = 1.0L;
    const real gamma = opt->adaptive ? 1.0L + 2.0L / smpl->n : 2.0L;
    const real rho = opt->adaptive ? 0.75L - 0.5L / smpl->n : 0.5L;
    const real sigma = opt->adaptive ? 1.0L - 1.0L / smpl->n : 0.5L;

    // simplex contains n + 1 vertices, where n is the dimensionality of each point
    for (size_t i = 0; i < smpl->n + 1; i++) {
        cost(mdl, smpl->vertices + i);
        smpl->num_eval++;
    }
    sort(smpl);

    // internal labels - 
    point *best = smpl->vertices;
    point *worst = smpl->vertices + smpl->n;

    if (opt->verbose) {
        const int number_len = opt->precision + (opt->format ? 8 : 4);
        const int header_len = (int)smpl->n * number_len;
        printf("%s%s iter  eval  operation     tol_x%*ctol_y%*ccoordinates%*cvalue%*c%s\n", 
            GRYBG, WHT, number_len - 5, ' ', number_len - 5, ' ', 
            header_len - 6, ' ', number_len - 6, ' ', NRM);
    }

    while (!terminated(smpl, opt)) {
        smpl->num_iter++;
        int shrink = 0;
        
        update_centroid(smpl);
        update_simplex(alpha, smpl->centroid, smpl->centroid, worst, 
            mdl, smpl, smpl->reflected);

        print_verbose(opt->verbose, " %4d  %4d  ", smpl->num_iter, smpl->num_eval);
        
        if (smpl->reflected->y < best->y) {
            update_simplex(gamma, smpl->centroid, smpl->reflected, smpl->centroid, 
                mdl, smpl, smpl->expanded);
            if (smpl->expanded->y < smpl->reflected->y) {
                print_verbose(opt->verbose, "%sexpand      %s ", CYN, NRM);
                copy_point(smpl->n, smpl->expanded, worst);
            } else {
                print_verbose(opt->verbose, "%sreflect     %s ", GRN, NRM);
                copy_point(smpl->n, smpl->reflected, worst);
            }
        
        } else if (smpl->reflected->y < (worst - 1)->y) {
            print_verbose(opt->verbose, "%sreflect     %s ", GRN, NRM);
            copy_point(smpl->n, smpl->reflected, worst);
        
        } else if (smpl->reflected->y < worst->y) {
            update_simplex(rho, smpl->centroid, smpl->reflected, smpl->centroid, 
                mdl, smpl, smpl->contracted);
            shrink = smpl->contracted->y >= smpl->reflected->y;
            if (!shrink) {
                print_verbose(opt->verbose, "%scontract_out%s ", MGT, NRM);
                copy_point(smpl->n, smpl->contracted, worst);
            }
        
        } else {
            update_simplex(rho, smpl->centroid, worst, smpl->centroid, 
                mdl, smpl, smpl->contracted);
            shrink = smpl->contracted->y > worst->y;
            if (!shrink) {
                print_verbose(opt->verbose, "%scontract_in %s ", YLW, NRM);
                copy_point(smpl->n, smpl->contracted, worst);
            }
        }

        if (shrink) {
            print_verbose(opt->verbose, "%sshrink      %s ", RED, NRM);
            for (size_t i = 1; i < smpl->n + 1; i++) {
                update_simplex(sigma, best, smpl->vertices + i, best, 
                    mdl, smpl, smpl->vertices + i);
            }
        }
        sort(smpl);

        if (opt->verbose) {
            print_value(tolerance_x(smpl), opt->precision, opt->format);
            print_value(tolerance_y(smpl), opt->precision, opt->format);
            printf(" ");
            print_point(smpl->n, best, opt->precision, opt->format);
        }
    }

    // save best point in output argument
    copy_point(smpl->n, best, out);
}

/*
 * Euclidean distance between two points
 */
real distance (size_t n, const point *pnt0, const point *pnt1) {
    real sum = 0.0L;
    for (size_t i = 0; i < n; i++) {
        sum += SQR(pnt0->x[i] - pnt1->x[i]);
    }
    return sqrtl(sum);
}

/*
 * Simplex sorting
 */
int compare (const void *arg1, const void *arg2) {
    const real f1 = ((const point *)arg1)->y;
    const real f2 = ((const point *)arg2)->y;
    return (f1 > f2) - (f1 < f2);
}

void sort (simplex *smpl) {
    qsort((void *)(smpl->vertices), (size_t)smpl->n + 1, sizeof(point), compare);
}

/*
 * Initial point at centroid, all vertices equally spaced, trial points allocated
 */
simplex *init_simplex (size_t n, real scale, const point *inp) {
    simplex *smpl = malloc(sizeof (simplex));             ASSERT(smpl);
    smpl->vertices = malloc((n + 1) * sizeof (point));    ASSERT(smpl->vertices);
    smpl->n = n;
    for (size_t i = 0; i < n + 1; i++) {  // simplex vertices
        smpl->vertices[i].x = malloc(n * sizeof (real));  ASSERT(smpl->vertices[i].x);
        for (size_t j = 0; j < n; j++) {  // coordinates
            smpl->vertices[i].x[j] = 0.0L;
        }
    }
    real b = 0.0L;
    for (size_t j = 0; j < n; j++) {
        real c = sqrtl(1.0L - b);
        smpl->vertices[j].x[j] = c;
        real r = - (1.0L / n + b) / c;
        for (size_t i = j + 1; i < n + 1; i++) {
            smpl->vertices[i].x[j] = r;
        }
        b += SQR(r);
    }
    for (size_t i = 0; i < n + 1; i++) {
        for (size_t j = 0; j < n; j++) {
            smpl->vertices[i].x[j] = scale * smpl->vertices[i].x[j] + inp->x[j];
        }
    }
    smpl->reflected = init_point(n);
    smpl->expanded = init_point(n);
    smpl->contracted = init_point(n);
    smpl->centroid = init_point(n);
    smpl->num_iter = smpl->num_eval = 0;
    return smpl;
}

/*
 * Get centroid (average position) of simplex
 */
void update_centroid (simplex *smpl) {
    for (size_t j = 0; j < smpl->n; j++) {
        smpl->centroid->x[j] = 0.0L;
        for (size_t i = 0; i < smpl->n; i++) {
            smpl->centroid->x[j] += smpl->vertices[i].x[j];
        }
        smpl->centroid->x[j] /= smpl->n;
    }
}

/*
 * Extend a point from inp0 in the scaled direction of (inp1 - inp2)
 */
void update_simplex (real scale, const point *inp0, const point *inp1, const point *inp2, 
                    const model *mdl, simplex *smpl, point *out) {
    for (size_t j = 0; j < smpl->n; j++) {
        out->x[j] = inp0->x[j] + scale * (inp1->x[j] - inp2->x[j]);
    }
    cost(mdl, out);
    smpl->num_eval++;
}

/*
 * Compute tolerance on x (Euclidean distance between coordinates of best and worst point)
 */
real tolerance_x (const simplex *smpl) {
    return distance(smpl->n, smpl->vertices, smpl->vertices + smpl->n);
}

/*
 * Compute tolerance on y (difference between values of best and worst point)
 */
real tolerance_y (const simplex *smpl) {
    return smpl->vertices[smpl->n].y - smpl->vertices[0].y;
}

/*
 * Terminate or continue?
 */
int terminated (const simplex *smpl, const optimset *opt) {
    return (
        smpl->num_eval >= opt->max_eval ||
        smpl->num_iter >= opt->max_iter
    ) || (
        tolerance_x(smpl) <= opt->tol_x &&
        tolerance_y(smpl) <= opt->tol_y
    );
}

/*
 * Point utilities
 */
point *init_point (size_t n) {
    point *pnt = malloc(sizeof (point));  ASSERT(pnt);
    pnt->x = malloc(n * sizeof(real));    ASSERT(pnt->x);
    return pnt;
}

void copy_point (size_t n, const point *inp, point *out) {
    memcpy(out->x, inp->x, n * sizeof(real));
    out->y = inp->y;
}

void print_point (size_t n, const point *pnt, int precision, int format) {
    printf("[ ");
    for (size_t i = 0; i < n; i++) {
        print_value(pnt->x[i], precision, format);
    }
    printf("] ");
    print_value(pnt->y, precision, format);
    printf("\n");
}

void print_value(real value, int precision, int format) {
    printf(format ? "% .*Le " : "% .*Lf ", precision, value);
}

void print_verbose (int verbose, const char *format, ...) {
    if (verbose) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}

/*
 * Memory utilities
 */
void free_simplex (simplex* smpl) {
    for (size_t i = 0; i < smpl->n; i++) {
        free(smpl->vertices[i].x);
    }
    free(smpl->vertices);
    free_point(smpl->reflected);
    free_point(smpl->expanded);
    free_point(smpl->contracted);
    free_point(smpl->centroid);
    free(smpl);
}

void free_point (point *pnt) {
    free(pnt->x);
    free(pnt);
}
