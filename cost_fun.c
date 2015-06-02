
#include "nelder_mead.h"

// cost function f : R^n->R
double cost_fun(int n, const double *x, void *arg) {
    int i;
    double out;
    
	// Sum Squares Function
    out = 0;
    for(i=0; i<n; i++) {
        out += SQUARE((i+1)*x[i]);
    }
    return 0.5 * out;
}
