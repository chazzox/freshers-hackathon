#pragma once
#define STARTING_BASE_HEALTH 1000

struct gameState {
    int compSocCoins;
    int levelProgression;
    int playerBaseHealth;
};

void initGameState(struct gameState *state);
void renderUI(struct gameState *state);
