#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include "assets.h"
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
    if (e->len >= e-> arrMaxLen) {
        e->list = realloc(e->list, sizeof(struct entity) * (e->arrMaxLen + ENTITY_LIST_BLOCK_LENGTH));
        e->arrMaxLen += ENTITY_LIST_BLOCK_LENGTH;
    }

    e->len++;
    return &e->list[e->len - 1];
}

void removeEntity(struct entities *e,
                  int index) {
    destroyEntity(&e->list[index]);
    for (int i = index; i < e->len - 1; i++) {
        e->list[i] = e->list[i + 1];
    }
    e->len--;
}

bool isPartiallyInWall(struct entity *ent, struct mapWalls *walls) {
    for (int x = ceil(ent->position.x); x < ceil(ent->position.x + ent->dimensions.x); x++) {
        // check top/bottom of hitbox
        if (walls->wallArr[x][(int) ceil(ent->position.y)]) return true;
        if (walls->wallArr[x][(int) ceil(ent->position.y + ent->dimensions.y)]) return true;
    }

    for (int y = ceil(ent->position.y); y < ceil(ent->position.y + ent->dimensions.y); y++) {
        // check sides of hitbox
        if (walls->wallArr[(int) ceil(ent->position.x)][y]) return true;
        if (walls->wallArr[(int) ceil(ent->position.x + ent->dimensions.x)][y]) return true;
    }
    return false;
}

bool isFullyInWall(struct entity *ent, struct mapWalls *walls) {
    for (int x = ceil(ent->position.x); x < ceil(ent->position.x + ent->dimensions.x); x++) {
        // check tops/bottom of hitbox
        if (!walls->wallArr[x][(int) ceil(ent->position.y)]) return false;
        if (!walls->wallArr[x][(int) ceil(ent->position.y + ent->dimensions.y)]) return false;
    }

    for (int y = ceil(ent->position.y); y < ceil(ent->position.y + ent->dimensions.y); y++) {
        // check sides of hitbox
        if (!walls->wallArr[(int) ceil(ent->position.x)][y]) return false;
        if (!walls->wallArr[(int) ceil(ent->position.x + ent->dimensions.x)][y]) return false;
    }

    return true;
}

#define SET_VELO_ZERO ent->velocity.x = 0;ent->velocity.y = 0;
void runEntityLogic(struct entities *e, struct mapWalls *walls, struct gameState *state) {
    // Move the entity
    for (int i = 0; i < e->len; i++) {
        struct entity *ent = &e->list[i];
        double x, y;
        
        if (ent->type == PROJECTILE) {
            x = ent->position.x + ent->velocity.x;
            x = ent->position.y + ent->velocity.y;
            
            // Check that I do not leave the mapdata
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
        } else {
            // See how far the entity can move before collision
            struct vect2 /*velocityNormalised*/ vm = normalise(ent->velocity);
            double vmag = mag(ent->velocity);

            x = ceil(ent->position.x + ent->velocity.x),
            y = ceil(ent->position.y + ent->velocity.y);
            double lastX = x,
                lastY = y;

            for (double l = 0; l < ceil(vmag); l++) {
                // Calculate the x, y of the raycast for l pixels away from position
                x = ceil(ent->position.x) + vm.x * l;
                y = ceil(ent->position.y) + vm.y * l;

                // Check that I do not leave the mapdata
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
                if (isPartiallyInWall(ent, walls)) {
                    x = lastX;
                    y = lastY;
                    SET_VELO_ZERO
                }

                lastX = x;
                lastY = y;
            }
        }

        ent->position.x = x;
        ent->position.y = y;

        drawEntity(ent);
    }

    // Run custom logic for each entity
    for (int i = 0; i < e->len; i++) {
        struct entity *ent = &e->list[i], *enemy = NULL, *projectileEnt = NULL;
        struct towerEntityData *towerData;
        struct vect2 projectileVector;
        
        // Run the entity logic
        switch (ent->type) {
            case NONE:
            case BASE:
                break;
            case TOWER:
                towerData = (struct towerEntityData *) ent->entityData;
                int min = INT_MAX, minIndex = -1;
                for (int j = 0; j < e->len; j++) {
                    if (j != i) {
                        double dist = getDist(ent->position, e->list[j].position);
                        if (dist < min) {
                            min = dist;
                            minIndex = j;
                        }
                    }
                }
                
                if (minIndex != -1 && time(NULL) - towerData->lastShotAt >= FIRE_RATE) {
                    enemy = &e->list[minIndex];
                    projectileVector = advancedAimingAlg(ent, enemy);
                    
                    projectileEnt = addEntity(e);
                    initEntity(projectileEnt, PLASMA_BALL);
                    projectileEnt->velocity = projectileVector;
                    projectileEnt->type = PROJECTILE;
                    projectileEnt->dimensions.x = BALL_SIZE;
                    projectileEnt->dimensions.y = BALL_SIZE;                    
                    towerData->lastShotAt = time(NULL);
                }
                break;
            case ENEMY:
                if (ent->health <= 0) {
                    removeEntity(e, i);
                    i--;
                }
                break;
            case PROJECTILE:
                ent->velocity.x *= 0.95;
                ent->velocity.y *= 0.95;
                if(ent->velocity.x < 0.1 && ent->velocity.y < 0.1) {
                    ent->health = 0;
                }
                
                // Iterate over all over particles to get projectile collisions with enemies
                for (int j = 0; j < e->len; j++) {
                    if (j != i) {
                        struct entity *ent2 = &e->list[j];
                        if (isEnemy(ent2)) 
                        if (isCollidingWith(ent, ent2)) {
                            state->compSocCoins++;
                            ent2->health--;
                            ent->health--;
                            
                            if (ent->health <= 0) {
                                break;
                            }
                        }
                    }
                }
                
                if (ent->health <= 0) {
                    removeEntity(e, i);
                    i--;
                }
                break;            
        }
    }
}

void initEntity(struct entity *e,
                ALLEGRO_BITMAP *bitmap) {
    e->entityAsset = bitmap;
    e->position.x = 0;
    e->position.y = 0;
    e->velocity.x = 0;
    e->velocity.y = 0;
    e->dimensions.x = 0;
    e->dimensions.y = 0;
    e->facing.x = 0;
    e->facing.y = 0;
    e->type = NONE;
    e->entityData = NULL;
}

void destroyEntity(struct entity* e) {
    if (e->entityData != NULL) {
        free(e->entityData);
        e->entityData = NULL;
    }
}

void drawEntity(struct entity* e) {
    double angle;
    
    // Check for undefined output
    if (e->facing.x == 0 && e->facing.y == 0) {
        angle = 0;
    } else if (e->facing.x == 0) {
        if (e->facing.y >= 0) {
            angle = 1.5 * M_PI;
        } else {
            angle = - M_PI / 2;
        }
    } else if (e->facing.y == 0) {
        if (e->facing.x >= 0) {
            angle = 0;
        } else {
            angle = M_PI;
        }
    }
    
    // Check which quadrant it is being drawn in
    else {
        angle = atan(e->facing.y / e->facing.x);
    
        // atan has domain -pi/2 < atan < pi/2
        // therefore the output may be offset
        if (e->facing.x > 0) {
            // +, +
            if (e->facing.y > 0) {
                angle = 2 * M_PI - angle;
            }
            // +, -
            else {
                angle *= -1;
            }
        } else {
            // -, +
            if (e->facing.y > 0) {
                angle += 1.5 *  M_PI;
            } 
            // -, -
            else {
                angle += M_PI / 2;
            }
        }
    }
    
    al_draw_rotated_bitmap(e->entityAsset, 
                           e->dimensions.x / 2, e->dimensions.y / 2,
                           e->position.x, e->position.y,
                           angle, 0);
}

bool isCollidingWith(struct entity* a, struct entity* b) {
    bool xMatch = a->position.x >= b->position.x
        && a->position.x + a->dimensions.x <= b->position.x + b->dimensions.x;
    bool yMatch = a->position.y >= b->position.y
        && a->position.y + a->dimensions.y <= b->position.y + b->dimensions.y;
    return xMatch && yMatch;
}

bool isEnemy(struct entity* e) {
    return e->type == ENEMY;
}

// TODO: Make this an advanced algorithm
struct vect2 advancedAimingAlg(struct entity *tower, struct entity *target) {
    return minus(tower->position, plus(target->position, target->velocity));
}

void initTower(struct entity* e) {    
    e->type = TOWER;
    e->dimensions.x = TOWER_SIZE;
    e->dimensions.y = TOWER_SIZE;
    struct towerEntityData *data = (struct towerEntityData *) malloc(sizeof(struct towerEntityData));
    data->lastShotAt = 0;
    e->entityData = data;    
}

