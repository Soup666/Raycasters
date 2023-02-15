#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <cmath>
#include "./src/LEditor.h"
#include "./src/LTextures.h"
#include "./src/LGameManager.h"
#include "./src/LSettings.h"


LGameManager gameManager;
int mode = 0;

// Main loop
int main(int argc, char *argv[])
{

    // Game Manager is used to hanlde everything graphically and inputs
    gameManager = LGameManager();
    gameManager.Init(); // Initialize the game manager

    printf("Starting\n");

    // Main Game Loop
    while (!gameManager.isFinished()) {  gameManager.Update();  }

    gameManager.CleanUp(); // Clean up the game manager

    return 0;
}