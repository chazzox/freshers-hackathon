#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mapwalls.h"
#include "entity.h"
#include "utils.h"

void initEntities(struct entities *e) {
    e->list = malloc(sizeof(struct entity) * ENTITY_LIST_BLOCK_LENGTH);
    e->arrMaxLen = ENTITY_LIST_BLOCK_LENGTH;
    e->len = 0;
}

void destroyEntities(struct entities *e) {
    for (int i = 0; i < e->len; i++) {
        destroyEntity(&e->list[i]);
    }
    free(e->list);
}

struct entity *addEntity(struct entities *e) {
    if (e->len + 1 >= e-> arrMaxLen) {
        void *new = realloc(e->list, sizeof(struct entity) * (e->arrMaxLen + ENTITY_LIST_BLOCK_LENGTH));
        if (new != e->list) {
            memcpy(new, e->list, sizeof(struct entity) * e->arrMaxLen);
        }

        e->arrMaxLen += ENTITY_LIST_BLOCK_LENGTH;
    }

    e->len++;
    return &e->list[e->len - 1];
}

void removeEntity(struct entities *e,
                  int index) {
    destroyEntity(&e->list[index]);
    for (int i = index; i < e->len - 1; i++) {
        e->list[index] = e->list[index + 1];
    }
    e->len--;
}

bool isInWall(struct entity *ent, struct mapWalls *walls) {
    // If there is a collision stop the entity and move it back
    for (int xx = ent->position.x; xx < ceil(ent->position.x + ent->dimensions.x); xx++) {
        // check top of hitbox
        if (walls->wallArr[(int) ceil(xx)][(int) ceil(ent->position.y)]) {
            return true;
        }
        // check bottom of hitbox
        if (walls->wallArr[(int) ceil(xx)][(int) ceil(ent->position.y + ent->dimensions.y)]) {
            return true;
        }
    }

    for (int yy = ent->position.x; yy < ceil(ent->position.x + ent->dimensions.y); yy++) {
        // check top of hitbox
        if (walls->wallArr[(int) ceil(ent->position.x + ent->dimensions.x)][(int) ceil(yy)]) {
            return true;
        }
        // check bottom of hitbox
        if (walls->wallArr[(int) ceil(ent->position.x + ent->dimensions.x)][(int) ceil(yy)]) {
            return true;
        }
    }
    return false;
}


#define SET_VELO_ZERO ent->velocity.x = 0;ent->velocity.y = 0;
void runEntityLogic(struct entities *e, struct mapWalls *walls) {
    for (int i = 0; i < e->len; i++) {
        struct entity *ent = &e->list[i];

        // See how far the entity can move before collision
        struct vect2 /*velocityNormalised*/ vm = normalise(ent->velocity);
        double vmag = mag(ent->velocity);

        double x = ceil(ent->position.x + ent->velocity.x),
               y = ceil(ent->position.y + ent->velocity.y),
               lastX = x,
               lastY = y;

        for (double l = 0; l < ceil(vmag); l++) {
            // Calculate the x, y of the caycast for l pixels away from position
            x = ceil(ent->position.x) + vm.x * l;
            y = ceil(ent->position.y) + vm.y * l;

            // Check that I do not leave the map
            if (x < 0) {
                x = 0;
                SET_VELO_ZERO
                break;
            }

            if (y < 0) {
                y = 0;
                SET_VELO_ZERO
                break;
            }

            if (x + ent->dimensions.x >= RES_X) {
                x = RES_X - 1 - ent->dimensions.x;
                SET_VELO_ZERO
                break;
            }

            if (y + ent->dimensions.y >= RES_Y) {
                y = RES_Y - 1 - ent->dimensions.y;
                SET_VELO_ZERO
                break;
            }

            // If in a wall move back and stop
            if (isInWall(ent, walls)) {
                x = lastX;
                y = lastY;
                SET_VELO_ZERO
            }

            lastX = x;
            lastY = y;
        }

        ent->position.x = x;
        ent->position.y = y;

        drawEntity(ent);
    }
}

void initEntity(struct entity *e,
                char entityAsset[]) {
    e->entityAsset = al_load_bitmap(entityAsset);
    e->position.x = 0;
    e->position.y = 0;
    e->velocity.x = 0;
    e->velocity.y = 0;
    e->dimensions.x = 0;
    e->dimensions.y = 0;
    e->health = 0;
    e->type = BASE;
    e->entityData = NULL;
}

void destroyEntity(struct entity* e) {
    al_destroy_bitmap(e->entityAsset);
    if (e->entityData != NULL) {
        free(e->entityData);
    }
}

void drawEntity(struct entity* e) {
    int flags = 0;
    if (e->velocity.x < 0)
        flags |= ALLEGRO_FLIP_HORIZONTAL;
    if (e->velocity.y < 0)
        flags |= ALLEGRO_FLIP_VERTICAL;

    al_draw_bitmap(e->entityAsset, e->position.x, e->position.y, flags);
}

