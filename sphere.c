
#include <stdlib.h>
#include <math.h>
#include "model.h"

struct Model { real _; };

model *init_model () {
    return NULL;
}

size_t dimensions () {
    return 3;
}

/*
 * Sphere function
 * http://www.sfu.ca/%7Essurjano/spheref.html
 */
void cost (const model *mdl, point *pnt) { (void)mdl;
    const size_t n = dimensions();
    real sum = 0.0L;
    for (size_t i = 0; i < n; i++) {
        sum += SQR(pnt->x[i]);
    }
    pnt->y = sqrtl(sum / n);
}
