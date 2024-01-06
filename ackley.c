
#include <stdlib.h>
#include <math.h>
#include "model.h"

struct Model { real a, b, c; };

model *init_model () {
    model *mdl = malloc(sizeof (model));
    mdl->a = 20.0L;
    mdl->b = 0.2L;
    mdl->c = 2.0L * acosl(-1.0L);
    return mdl;
}

size_t dimensions () {
    return 3;
}

/*
 * Ackley function
 * http://www.sfu.ca/%7Essurjano/ackley.html
 */
void cost (const model *mdl, point *pnt) {
    const size_t n = dimensions();
    real sum_sqr = 0.0L;
    real sum_cos = 0.0L;
    for (size_t i = 0; i < n; i++) {
        sum_sqr += SQR(pnt->x[i]);
        sum_cos += cosl(mdl->c * pnt->x[i]);
    }
    pnt->y = -mdl->a * expl(-mdl->b * sqrtl(sum_sqr / n)) - expl(sum_cos / n) + mdl->a + expl(1.0L);
}
