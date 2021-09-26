#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include "assets.h"
#include "gamestate.h"

#define COIN_RENDER_BUFF_SIZE 21
#define COIN_OFFSET 500
#define COIN_Y 30
#define TEXT_LEFT_MARGIN 100

void initGameState(struct gameState* state) {
    state->compSocCoins = 100;
    state->levelProgression = 0;
    state->playerBaseHealth = 1000;
}

void renderUI(struct gameState *state) {
    char buff[COIN_RENDER_BUFF_SIZE];
    snprintf(buff, COIN_RENDER_BUFF_SIZE * sizeof(char), "%06d", state->compSocCoins);
        
    al_draw_bitmap(COMPSOC_COIN, COIN_OFFSET, COIN_Y, 0);
    al_draw_text(HACK_BOLD, al_map_rgb(255, 255, 255), COIN_OFFSET + COMP_SOC_COIN_WIDTH + TEXT_LEFT_MARGIN, COIN_Y, 0, buff);
}
