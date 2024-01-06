
#include <stdlib.h>
#include "model.h"

struct Model { real a, b, c; };

model *init_model () {
    model *mdl = malloc(sizeof (model));
    mdl->a = 1.5L;
    mdl->b = 2.25L;
    mdl->c = 2.625L;
    return mdl;
}

size_t dimensions () {
    return 2;
}

/*
 * Beale function
 * http://www.sfu.ca/%7Essurjano/beale.html
 */
void cost (const model *mdl, point *pnt) {
    pnt->y =  SQR(mdl->a - pnt->x[0] + pnt->x[0] * pnt->x[1]) \
            + SQR(mdl->b - pnt->x[0] + pnt->x[0] * SQR(pnt->x[1])) \
            + SQR(mdl->c - pnt->x[0] + pnt->x[0] * pnt->x[1] * SQR(pnt->x[1]));
}
