
#include <stdlib.h>
#include <math.h>
#include "model.h"

struct Model { real _; };

model *init_model () {
    model *mdl = malloc(sizeof (model));
    return mdl;
}

size_t dimensions () {
    return 1;
}

/*
 * Absolute value function
 */
void cost (const model *mdl, point *pnt) { (void)mdl;
    const size_t n = dimensions();
    pnt->y = 0L;
    for (size_t i = 0; i < n; i++) {
        pnt->y += fabsl(pnt->x[i]);
    }
}
