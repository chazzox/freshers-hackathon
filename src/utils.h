#pragma once

#ifndef RES_X
#define RES_X 3840
#endif

#ifndef RES_Y
#define RES_Y 2160
#endif

#ifndef FPS
#define FPS 60
#endif

#define TOWER_SIZE 100

struct vect2 {
    double x, y;
};

double sqr(double a);
double getDist(struct vect2 a, struct vect2 b);
double mag(struct vect2 a);
struct vect2 normalise(struct vect2 a);
