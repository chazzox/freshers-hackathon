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

void runEntityLogic(struct entities *e, struct mapWalls *walls) {
    for (int i = 0; i < e->len; i++) {
        struct entity *ent = &e->list[i];

        // See how far the entity can move before collision
        struct vect2 /*velocityNormalised*/ vm = normalise(ent->velocity);

        int x = ceil(ent->position.x + ent->velocity.x);
        int y = ceil(ent->position.y + ent->velocity.y);

        for (int l = 0, lastX = x, lastY = y; l < ceil(mag(ent->velocity)); l++) {
            x = ceil(ent->position.x) + vm.x * l;
            y = ceil(ent->position.y) + vm.y * l;

            // If there is a collision stop the entity and
            if (walls->wallArr[x][y]) {
                ent->velocity.x = 0;
                ent->velocity.y = 0;
                x = lastX;
                y = lastY;
                break;
            }

            lastX = x;
            lastY = y;
        }

        ent->position.x = x;
        ent->position.y = y;;
    }
}

void initEntity(struct entity *e,
                char entityAsset[]) {
    e->entityAsset = al_load_bitmap(entityAsset);
    e->position.x = 0;
    e->position.y = 0;
    e->velocity.x = 0;
    e->velocity.y = 0;
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

    al_draw_bitmap(e->entityAsset, 0, 0, flags);
}

