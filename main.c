
#include <stdio.h>
#include <stdlib.h>
#include "nelder_mead.h"

#define NARGS 10

int main(int argc, const char *argv[]) {
    PRINT_ARGS(argc, argv, WHT);
    ASSERT(argc > NARGS);

    // read optimizer settings from command line and check values
    optimset opt = {
        .precision = (int)strtol(argv[1], NULL, BASE),
        .format = (int)strtol(argv[2], NULL, BASE),
        .verbose = (int)strtol(argv[3], NULL, BASE),
        .tol_x = strtold(argv[4], NULL),
        .tol_y = strtold(argv[5], NULL),
        .max_iter = (unsigned int)strtol(argv[6], NULL, BASE),
        .max_eval = (unsigned int)strtol(argv[7], NULL, BASE),
        .adaptive = (int)strtol(argv[8], NULL, BASE),
        .scale = strtold(argv[9], NULL)
    };
    ASSERT(opt.precision >= 3 && opt.precision <= 36);
    ASSERT(opt.verbose == 0 || opt.verbose == 1);
    ASSERT(opt.tol_x >= 1.0e-36L && opt.tol_x <= 1.0e-3L);
    ASSERT(opt.tol_y >= 1.0e-36L && opt.tol_y <= 1.0e-3L);
    ASSERT(opt.max_iter >= 1 && opt.max_iter <= 100000);
    ASSERT(opt.max_eval >= 1 && opt.max_eval <= 100000);
    ASSERT(opt.adaptive == 0 || opt.adaptive == 1);
    ASSERT(opt.scale >= 1.0e-12L && opt.scale <= 1.0e3L);

    // infer number of dimension from command args
    const size_t n = (size_t)(argc - NARGS);
    // assert number of dimensions is correct
    ASSERT(n == dimensions());

    // allocate input / output points
    point *inp = init_point(n);
    point *out = init_point(n);

    // set input point coordinates from command args
    for (size_t i = 0; i < n; i++) {
        inp->x[i] = strtod(argv[i + NARGS], NULL);
    }

    // initialize model and simplex
    model *mdl = init_model();
    simplex *smpl = init_simplex(n, opt.scale, inp);

    // optimize model mdl, using settings opt, starting 
    // from simplex smpl, and save result to point out
    nelder_mead(mdl, &opt, smpl, out);

    // print information
    cost(mdl, inp);
    printf("%s        Input%s ", WHT, NRM);
    print_point(n, inp, opt.precision, opt.format);
    printf("%s       Output%s ", WHT, NRM);
    print_point(n, out, opt.precision, opt.format);
    printf("%s  Tolerance-x%s", WHT, NRM); 
    print_value(tolerance_x(smpl), opt.precision, opt.format); 
    printf("\n");
    printf("%s  Tolerance-y%s", WHT, NRM); 
    print_value(tolerance_y(smpl), opt.precision, opt.format); 
    printf("\n");
    printf("%s   Iterations%s %d\n", WHT, NRM, smpl->num_iter);
    printf("%s  Evaluations%s %d\n", WHT, NRM, smpl->num_eval);

    // free memory and exit
    free_simplex(smpl);
    free_point(inp);
    free_point(out);
    free(mdl);
    return 0;
}
