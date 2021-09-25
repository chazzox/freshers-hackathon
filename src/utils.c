#include <math.h>
#include <stdlib.h>
#include "utils.h"

double sqr(double a) {
    return a * a;
}

double getDist(struct vect2 a,
               struct vect2 b) {
    return sqrt(sqr(a.x - b.x) + sqr(a.y - b.y));
}

double mag(struct vect2 a) {
    return sqrt(sqr(a.x) + sqr(a.y));
}

struct vect2 normalise(struct vect2 a) {
    double m = mag(a);
    struct vect2 out = {a.x / m, a.y / m};
    return out;
}
