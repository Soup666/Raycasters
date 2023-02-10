#ifndef LGameManager_h
#define LGameManager_h

#include <math.h>
#include <SDL2/SDL.h>
#include "LSettings.h"
#include "LPlayer.h"
#include "LTextures.h"
#include "LEditor.h"

class LGameManager
{
public:
    LGameManager();
    ~LGameManager();
    void Init();
    void Update();
    void Render();
    void CleanUp();
    bool isFinished();
private:
    bool W, A, S, D, finished;
    void handleInputs(SDL_Event event);
    void drawRays();
    void drawPlayer();
    double sinTable[360];
    double cosTable[360];
    double getVerticalDistance(double ra, double &rp, int &mv);
    double getHorizontalDistance(double ra, double &rp, int &mv);
    int getMapPos(double x, double y);
    int createWindow();
    int mode;
    double bobbingAmount;
    array<u_int8_t, 64*64> room;
    LTextures textures;
    LEditor editor;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    LPlayer player;
    SDL_Texture *texture = NULL;
    SDL_Surface *gunTexture;
};

#endif