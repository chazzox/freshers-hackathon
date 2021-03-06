#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>

#include "gamestate.h"
#include "mapwalls.h"
#include "entity.h"
#include "utils.h"
#include "assets.h"

__EXTERN__ASSET__DEFS__();

void spawnEnemy(struct entities *ents) {
    struct entity *enemy = addEntity(ents);
    initEntity(enemy, ENEMY_1);
    enemy->type = ENEMY;
    enemy->health = 100;
    enemy->velocity.x = rand() % 10;
    enemy->velocity.y = rand() % 10;
    
    enemy->position.x = 1497;//abs(rand() % RES_X);
    enemy->position.y = 1000;//abs(rand() % RES_Y);
    
    enemy->dimensions.x = ENEMY_SIZE;
    enemy->dimensions.y = ENEMY_SIZE;
}

// This always runs headless and contextless
int main() {
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;

    // Initialize allegro
    if (!al_init()) {
        fprintf(stderr, "Failed to initialize allegro.\n");
        return 1;
    }

    // Initialize the timer
    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        fprintf(stderr, "Failed to create timer.\n");
        return 2;
    }

    // Create the display
    display = al_create_display(RES_X, RES_Y);
    if (!display) {
        fprintf(stderr, "Failed to create display.\n");
        return 3;
    }

    // Install Mouse drivers
    if(!al_install_mouse()){
        fprintf(stderr, "Failed to install Mouse");
        return 9;
    }

    // Install Touch Driver
    if(!al_install_touch_input()){
        fprintf(stderr, "Failed to install touch");
    }

    // Create the event queue
    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "Failed to create event queue.");
        return 4;
    }

    // Init plugins
    al_init_ttf_addon();
    al_init_image_addon();
    __INIT__ASSETS__();

    // Init game
    struct gameState state;
    initGameState(&state);
    struct entities ents;
    initEntities(&ents);
    
    // GENERATE RANDOM ENTITIES FOR TESTING
    srand(time(NULL));
    
    struct entity *base = addEntity(&ents);
    initEntity(base, PLAYER_BASE);
    base->type = BASE;
    base->health = 1000;
    
    base->position.x = BASE_X;
    base->position.y = BASE_Y;
    
    base->dimensions.x = TOWER_SIZE;
    base->dimensions.y = TOWER_SIZE;
    
    for (int i = 0; i < 10; i++) {
        spawnEnemy(&ents);
    }

    // Register event sources
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_mouse_event_source());
	
	// Touch inputs will register as mouse events
	if(al_is_touch_input_installed()){
		al_set_mouse_emulation_mode(ALLEGRO_MOUSE_EMULATION_EXCLUSIVE);
		al_register_event_source(event_queue, al_get_touch_input_mouse_emulation_event_source());
	}

    // Display a black screen
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(HACK_BOLD, al_map_rgb(255, 255, 255), 30, 30, 0, "LOADING...");
    al_flip_display();
    
    printf("Loading collision boxes...\n");
    struct mapWalls *mapWalls = initMapWalls(IMG_BACKGROUND_COLLISIONS);
    printf("Loaded collision boxes.\n");

    // Start the timer
    al_start_timer(timer);

    // Game loop
    unsigned long frameCount = 0, last = 0;
    bool running = true;
    bool redraw = true;
    while (running) {
        ALLEGRO_EVENT event;
        ALLEGRO_TIMEOUT timeout;

        // Initialize timeout
        al_init_timeout(&timeout, 0.06);

        // Fetch the event (if one exists)
        bool get_event = al_wait_for_event_until(event_queue, &event, &timeout);

        struct entity *clickSummon_real;
        struct entity towerSummonTmp;

        // Handle the event
        if (get_event) {
            switch (event.type) {
                case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                    towerSummonTmp.position.x = event.mouse.x;
                    towerSummonTmp.position.y = event.mouse.y;

                    towerSummonTmp.dimensions.x = TOWER_SIZE;
                    towerSummonTmp.dimensions.y = TOWER_SIZE;

                    if (isFullyInWall(&towerSummonTmp, mapWalls) && state.compSocCoins >= TOWER_COST) {
                        clickSummon_real = addEntity(&ents);
                        initEntity(clickSummon_real, PLASMA_TOWER);
                        initTower(clickSummon_real);

                        clickSummon_real->position.x = event.mouse.x;
                        clickSummon_real->position.y = event.mouse.y;
                        
                        state.compSocCoins -= TOWER_COST;

                        printf("Created a test tower entity.\n");
                    }

                    printf("Click event.\n");
                    break;
                case ALLEGRO_EVENT_TIMER:
                    redraw = true;
                    frameCount++;
                    if (frameCount - last > 5000 / (frameCount * frameCount)) {
                        spawnEnemy(&ents);
                        last = frameCount;
                    }
                    break;
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    running = false;
                    break;
                case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
                case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
                    fprintf(stderr, "Display Switch Event\n");
                    break;
				// Events that we dont care about
                case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
                case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
				case ALLEGRO_EVENT_MOUSE_WARPED:
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                case ALLEGRO_EVENT_MOUSE_AXES:
                    break;
                default:
                    fprintf(stderr, "Unsupported event received: %d\n", event.type);
                    break;
            }
        }

        // Check if we need to redraw
        if (redraw && al_is_event_queue_empty(event_queue)) {
            // Draw the frame
            // Draw the background
            al_draw_bitmap(BACKGROUND, 0, 0, 0);

            /* //DEBUG FOR TESTING COLLISION MAP
            for (int x = 0; x < RES_X; x++) {
                for (int y = 0; y < RES_Y; y++) {
                    if (mapWalls->wallArr[x][y]) al_draw_pixel(x, y, al_map_rgb(0, 0, 255));
                }
            }*/
            
            // Draw all entites
            runEntityLogic(&ents, mapWalls, &state);
            
            // Display the UI overlay
            renderUI(&state);
            
            // flip display
            al_flip_display();
            redraw = false;
        }
    }

    // Clean up
    __FREE__ASSETS__();
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    // Clean up game
    destroyEntities(&ents);
    free(mapWalls);

    return 0;
}
