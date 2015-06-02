
#include "nelder_mead.h"

int compare(const void *arg1, const void *arg2) {
    double fx1, fx2;
    fx1 = (((point_t *)arg1)->fx);
    fx2 = (((point_t *)arg2)->fx);
    
    if(fx1==fx2) {
        return 0;
    } else {
        return (fx1<fx2)? -1 : 1;
    }
}

void get_centroid(int n, point_t *points, double *x_bar) {
    int i, j;
    for(j=0; j<n; j++) {
        x_bar[j] = 0;
        for(i=0; i<n; i++) {
            x_bar[j] += points[i].x[j];
        }
        x_bar[j] /= n;
    }
}

double modulus(double x) {
    return (x>0)? x : -x;
}

int continue_minimization(int n, point_t *points, int eval_count, int iter_count, optimset_t optimset) {
    int i,j;
    double condx = -1;
    double condf = -1;
    double temp;
    if(eval_count>optimset.max_eval || iter_count>optimset.max_iter) {
        // stop if #evals or #iters are greater than the max allowed
        return 0;
    }
    for(i=1; i<n+1; i++) {
        temp = modulus(points[0].fx-points[i].fx);
        if(condf<temp) {
            condf = temp;
        }
    }
    for(i=1; i<n+1; i++) {
        for(j=0; j<n; j++) {
            temp = modulus(points[0].x[j]-points[i].x[j]);
            if(condx<temp) {
                condx = temp;
            }
        }
    }
    // continue if both tolx or tolf condition is not met
    return condx>optimset.tolx || condf>optimset.tolf;
    
}

void get_point(int n, double *x, double *x_bar, double coeff, double *x_out) {
    int j;
    for(j=0; j<n; j++) {
        x_out[j] = (1+coeff)*x_bar[j] - coeff*x[j];
    }
}

void copy_point(int n, double *x_from, double *x_to, double fx_from, double *fx_to) {
    int j;
    for(j=0; j<n; j++) {
        x_to[j] = x_from[j];
    }
    *fx_to = fx_from;
}

void swap_points(int n, point_t *p1, point_t *p2) {
    int j;
    double temp;
    for(j=0; j<n; j++) {
        temp     = p1->x[j];
        p1->x[j] = p2->x[j];
        p2->x[j] = temp;
    }
    temp   = p1->fx;
    p1->fx = p2->fx;
    p2->fx = temp;
}

void print_min(int n, point_t *points) {
    int j;
    printf("[ ");
    for(j=0; j<n; j++) {
        printf("%.2f ", points[0].x[j]);
    }
    printf("]\t%.2f \n", points[0].fx);
}
