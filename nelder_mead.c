
#include "nelder_mead.h"

void nelder_mead(double *x0, int n, optimset_t optimset, point_t *solution, void *cost_fun_args) {
    
    int        i, j;
    int        iter_count, eval_count;
    int        shrink;
    double     *x_bar;
    double     *x_r, *x_e, *x_c;
    double     fx_r, fx_e, fx_c;
    point_t    *points;
    
    x_bar      = MALLOC(n*sizeof(double));
    x_r        = MALLOC(n*sizeof(double));
    x_e        = MALLOC(n*sizeof(double));
    x_c        = MALLOC(n*sizeof(double));
    points     = MALLOC((n+1)*sizeof(point_t));
    
    iter_count = 0;
    eval_count = 0;
    
    // initial simplex
    for(i=0; i<n+1; i++) {
        points[i].x = MALLOC(n*sizeof(double));
        for(j=0; j<n; j++) {
            points[i].x[j] = (i-1==j)? ( x0[j]!=0? 1.05*x0[j] : 0.00025 ) : x0[j];
        }
        points[i].fx = cost_fun(n, points[i].x, cost_fun_args);
        eval_count++;
    }
    qsort((void *)points, n+1, sizeof(point_t), compare);
    get_centroid(n, points, x_bar);
    iter_count++;
    
    // continue minimization until stop conditions are met
    while(continue_minimization(n, points, eval_count, iter_count, optimset)) {
        shrink = 0;
        
        if(optimset.verbose) {
            PRINTF("Iteration %04d \t", iter_count);
        }
        get_point(n, points[n].x, x_bar, RHO, x_r);
        fx_r = cost_fun(n, x_r, cost_fun_args);
        eval_count++;
        if(fx_r<points[0].fx) {
            get_point(n, points[n].x, x_bar, RHO*CHI, x_e);
            fx_e = cost_fun(n, x_e, cost_fun_args);
            eval_count++;
            if(fx_e<fx_r) {
                // expand
                if(optimset.verbose) {
                    PRINTF("expand \t\t");
                }
                copy_point(n, x_e, points[n].x, fx_e, &(points[n].fx));
            } else {
                // reflect
                if(optimset.verbose) {
                    PRINTF("reflect \t");
                }
                copy_point(n, x_r, points[n].x, fx_r, &(points[n].fx));
            }
        } else {
            if(fx_r<points[n-1].fx) {
                // reflect
                if(optimset.verbose) {
                    PRINTF("reflect \t");
                }
                copy_point(n, x_r, points[n].x, fx_r, &(points[n].fx));
            } else {
                if(fx_r<points[n].fx) {
                    get_point(n, points[n].x, x_bar, RHO*GAMMA, x_c);
                    fx_c = cost_fun(n, x_c, cost_fun_args);
                    eval_count++;
                    if(fx_c<=fx_r) {
                        // contract outside
                        if(optimset.verbose) {
                            PRINTF("contract out \t");
                        }
                        copy_point(n, x_c, points[n].x, fx_c, &(points[n].fx));
                    } else {
                        // shrink
                        if(optimset.verbose) {
                            PRINTF("shrink \t\t");
                        }
                        shrink = 1;
                    }
                } else {
                    get_point(n, points[n].x, x_bar, -GAMMA, x_c);
                    fx_c = cost_fun(n, x_c, cost_fun_args);
                    eval_count++;
                    if(fx_c<=points[n].fx) {
                        // contract inside
                        if(optimset.verbose) {
                            PRINTF("contract in \t");
                        }
                        copy_point(n, x_c, points[n].x, fx_c, &(points[n].fx));
                    } else {
                        // shrink
                        if(optimset.verbose) {
                            PRINTF("shrink \t\t");
                        }
                        shrink = 1;
                    }
                }
            }
        }
        if(shrink) {
            for(i=1; i<n+1; i++) {
                for(j=0; j<n; j++) {
                    points[i].x[j] = points[0].x[j] + SIGMA*(points[i].x[j]-points[0].x[j]);
                }
                points[i].fx = cost_fun(n, points[i].x, cost_fun_args);
                eval_count++;
            }
            qsort((void *)points, n+1, sizeof(point_t), compare);
        } else {
            for(i=n-1; i>=0 && points[i+1].fx<points[i].fx; i--) {
                swap_points(n, points+(i+1), points+i);
            }
        }
        get_centroid(n, points, x_bar);
        iter_count++;
        if(optimset.verbose) {
            print_min(n, points);
        }
    }
    
    // save solution in output argument
    solution->x = MALLOC(n*sizeof(double));
    copy_point(n, points[0].x, solution->x, points[0].fx, &(solution->fx));
    
}
