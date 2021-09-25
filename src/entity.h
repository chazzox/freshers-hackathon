#pragma once
#include "utils.h"
#include <allegro5/bitmap.h>
#include <stdbool.h>

#define ENTITY_LIST_BLOCK_LENGTH 4096

enum entityType {
    NONE, BASE, TOWER, ENEMY_1, ENEMY_2, PROJECTILE, BOUNCING_PROJECTILE
};

// This is a dynamic list that will grow if there are too many entites and shrink if it can
struct entities {
    int len, arrMaxLen;
    struct entity *list;
};

void initEntities(struct entities *e);
void destroyEntities(struct entities *e);
struct entity *addEntity(struct entities *entities);
void removeEntity(struct entities *e, int index);
void runEntityLogic(struct entities *e, struct mapWalls *walls);

struct entity {
    ALLEGRO_BITMAP *entityAsset;
    struct vect2 position, velocity, dimensions;
    int health;
    enum entityType type;
    void *entityData;
};

void initEntity(struct entity *e, ALLEGRO_BITMAP *bitmap);
void destroyEntity(struct entity *e);
void drawEntity(struct entity *e);
bool isPartiallyInWall(struct entity *e, struct mapWalls *walls);
bool isFullyInWall(struct entity *ent, struct mapWalls *walls);
bool isCollidingWith(struct entity *a, struct entity *b);

// Init code for different entity types
struct bouncingProjectileData {
    struct vect2 lastVelocity;
    int bounces;
};
void initBouncingProjectile(struct entity *e, int maxBounces);
