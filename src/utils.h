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

#define FIRE_RATE 1
#define TOWER_COST 10
#define TOWER_RANGE 2500
#define TOWER_SIZE 100
#define BALL_SIZE 30
#define ENEMY_SIZE 50

#define BASE_X 295
#define BASE_Y 100

// define M_PI. it is not defined in POSIX compliant implementations so it needs defining
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct vect2 {
    double x, y;
};

double sqr(double a);
double getDist(struct vect2 a, struct vect2 b);
double mag(struct vect2 a);
struct vect2 normalise(struct vect2 a);
struct vect2 dotProduct(struct vect2 a, struct vect2 b);
struct vect2 minus(struct vect2 a, struct vect2 b);
struct vect2 plus(struct vect2 a, struct vect2 b);
double getAngleBetween(struct vect2 a, struct vect2 b);
