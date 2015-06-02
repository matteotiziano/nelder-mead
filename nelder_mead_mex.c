
#include "nelder_mead.h"

/* [x, fx] = nelder_mead_mex( x0, tolX, tolF, maxIter, maxEval, verbose ) */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    
    double     tolx, tolf;
    int        max_iter, max_eval, verbose;
    double     *x0;
    int        n;
    point_t    solution;
    optimset_t optimset;
    
    double     *x_min, *fx_min;
    
    x0         = mxGetPr(prhs[0]);
    n          = (int)mxGetN(prhs[0]);
    tolx       = nrhs>=2? mxGetScalar(prhs[1]) : TOL_X;
    tolf       = nrhs>=3? mxGetScalar(prhs[2]) : TOL_F;
    max_iter   = nrhs>=4? (int)mxGetScalar(prhs[3]) : MAX_ITER;
    max_eval   = nrhs>=5? (int)mxGetScalar(prhs[4]) : MAX_EVAL;
    verbose    = nrhs>=6? (int)mxGetScalar(prhs[5]) : VERBOSE;
    
    plhs[0]    = mxCreateDoubleMatrix(1, n, mxREAL);
    plhs[1]    = mxCreateDoubleMatrix(1, 1, mxREAL);
    x_min      = mxGetPr(plhs[0]);
    fx_min     = mxGetPr(plhs[1]);
    
    optimset.tolx     = tolx;
    optimset.tolf     = tolf;
    optimset.max_iter = max_iter;
    optimset.max_eval = max_eval;
	optimset.verbose  = verbose;
    
    nelder_mead(x0, n, optimset, &solution, NULL);
    
    memcpy(x_min, solution.x, n*sizeof(double));
    fx_min[0] = solution.fx;
    
}
