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


struct vect2 dotProduct(struct vect2 a, struct vect2 b){
    a.x *= b.x;
    a.y *= b.y;
    return a;
}

double getAngleBetween(struct vect2 a, struct vect2 b) {
    return acos(mag(dotProduct(a,b)) / (mag(a) * mag(b)));
}

struct vect2 minus(struct vect2 a, struct vect2 b) {
    b.x -= a.x;
    b.y -= a.y;
    return b;
}

struct vect2 plus(struct vect2 a, struct vect2 b) {
    a.x += b.x;
    a.y += b.y;
    return a;
}
