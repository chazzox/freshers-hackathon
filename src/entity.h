#pragma once
#include <utils.h>
#include <allegro5/bitmap.h>

#define ENTITY_LIST_BLOCK_LENGTH 4096

enum entityType {
    BASE
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
    struct vect2 position, velocity;
    int health;
    enum entityType type;
    void *entityData;
};

void initEntity(struct entity *e, char entityAsset[]);
void destroyEntity(struct entity *e);
void drawEntity(struct entity *e);