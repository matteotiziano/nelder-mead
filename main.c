
#include "nelder_mead.h"

int main( int argc, const char* argv[] ) {
    int i, n;      // number of dimension of initial point
    double *x0;    // cooridnates of initial point
    
    point_t    solution;
    optimset_t optimset;
    
    if(argc==1) {
        PRINTF("%s: error: not enough inputs \n", argv[0]);
        return 0;
    }
    
	// reading initial point from command line
    n = argc-1;
    x0 = malloc(n*sizeof(double));
    for(i=0; i<n; i++) {
        x0[i] = atof(argv[i+1]);
    }
    
	// setting default options
    optimset.tolx     = TOL_X;
    optimset.tolf     = TOL_F;
    optimset.max_iter = MAX_ITER;
    optimset.max_eval = MAX_EVAL;
	optimset.verbose  = VERBOSE;
    
	// call optimization methods 
    nelder_mead(x0, n, optimset, &solution, NULL);
    
	// print solution
    PRINTF("SOLUTION\n");
    PRINTF("x=[ ");
    for(i=0; i<n; i++) {
        PRINTF("%.8f ", solution.x[i]);
    }
    PRINTF("], fx=%.8f \n", solution.fx);
    
    return 0;
}
