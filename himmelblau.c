
#include <stdlib.h>
#include "model.h"

struct Model { real a, b; };

model *init_model () {
    model *mdl = malloc(sizeof (model));
    mdl->a = 11.0L;
    mdl->b = 7.0L;
    return mdl;
}

size_t dimensions () {
    return 2;
}

/*
 * Himmelblau function
 * https://en.wikipedia.org/wiki/Himmelblau%27s_function
 */
void cost (const model *mdl, point *pnt) {
    pnt->y = SQR(SQR(pnt->x[0]) + pnt->x[1] - mdl->a) + SQR(pnt->x[0] + SQR(pnt->x[1]) - mdl->b);
}
