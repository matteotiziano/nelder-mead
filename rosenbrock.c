
#include <stdlib.h>
#include "model.h"

struct Model { real a, b; };

model *init_model () {
    model *mdl = malloc(sizeof (model));
    mdl->a = 1.0L;
    mdl->b = 100.0L;
    return mdl;
}

size_t dimensions () {
    return 2;
}

/*
 * Rosenbrock function
 * http://www.sfu.ca/%7Essurjano/rosen.html
 */
void cost (const model *mdl, point *pnt) {
    pnt->y = SQR(mdl->a - pnt->x[0]) + mdl->b * SQR(pnt->x[1] - SQR(pnt->x[0]));
}
