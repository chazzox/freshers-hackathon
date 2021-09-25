#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"

struct mapWalls {
    bool wallArr[RES_X][RES_Y];
};

struct mapWalls *initMapWalls(char imageName[]);
