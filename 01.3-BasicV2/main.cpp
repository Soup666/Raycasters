#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include "./src/LEditor.h"
#include "./src/LTextures.h"

#define FOV 60.0
#define WIDTH 720
#define HEIGHT 640
#define PI 3.14
#define DEBUG 0
#define PHEIGHT 640
#define PWIDTH 720
#define DOF 64
#define MAPR 64
#define MAPC 64
#define texWidth 64
#define texHeight 64
#define mapMultiplier 10

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool finished = false;

bool W = false;
bool S = false;
bool D = false;
bool A = false;

int mode = 0;

array<u_int8_t, 64*64> room;
LTextures textures;
LEditor editor(textures);

double px, py, pa, projectionDist, pdx, pdy, pSpeed;

void fixAngle(double &a) {
    if (a < 0) a += 360.0;
    if (a > 360.0) a -= 360.0;
}

double degToRad(double a) {
    return a*(PI/180.0);
}

int getMapPos(double x, double y) {

    return((int)floor(x/64) % MAPC + (MAPC*((int)floor(y / 64))));
}

// Handles all the inputs
void handleInputs(SDL_Event event) {

    while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    finished = true;
                    break;
                case SDL_KEYDOWN: 
                    switch(event.key.keysym.sym) {
                        case SDLK_w: W = true; break;
                        case SDLK_s: S = true; break;
                        case SDLK_a: A = true; break;
                        case SDLK_d: D = true; break;
                        case SDLK_LSHIFT: pSpeed = 20; break;
                        case SDLK_ESCAPE: SDL_DestroyWindow(window); SDL_Quit(); exit(0); break;
                    }
                    break;
                case SDL_KEYUP:
                    switch(event.key.keysym.sym) {
                        case SDLK_w: W = false; break;
                        case SDLK_s: S = false; break;
                        case SDLK_a: A = false; break;
                        case SDLK_d: D = false; break;
                        case SDLK_LSHIFT: pSpeed = 10; break;
                        case SDLK_TAB: mode = (mode + 1) % 2; break;
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    x /= 8; // Again, 8 for the LEditor
                    y /= 8;
                    editor.incrememntSegment(x, y);
                    room = editor.getMap();
                    break;
            }
        }

        if(A) {pa-=2; fixAngle(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));}
        if(D) {pa+=2; fixAngle(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));}
        if(W) {
            if (room[getMapPos(px+(pdx*pSpeed), py-(pdy*pSpeed))] == 0 ) { px+=(pdx) * pSpeed; py-=(pdy) * pSpeed; }
        }
        if(S) {
            if (room[getMapPos(px-(pdx*pSpeed), py+(pdy*pSpeed))] == 0 ) { px-=(pdx) * pSpeed; py+=(pdy) * pSpeed; }
        }
}

// Creates the window and renderer
int createWindow() {
    window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Could not create window: %s", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        printf("Could not create renderer: %s", SDL_GetError());
        return 1;
    }

    return 0;
}

void drawPlayer() {

    int screenPx = floor(px / 8);
    int screenPy = floor(py / 8); // 8 because the LEditor uses 8 pixels for a square

    SDL_Rect rect = { screenPx, screenPy, 5, 5 };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderDrawLine(renderer, screenPx, screenPy, screenPx + 10 * cos(degToRad(pa)), screenPy + 10 * sin(degToRad(pa)));
}

double getHorizontalDistance(double ra, double &rp, int &mv) {
        
    double Ay = floor(py/64) * 64;
    if (ra >= PI) Ay -= 0.1; // Looking down
    else Ay += 64; // Looking up

    double Ax = px + (py-Ay) / -tan(ra);
    if (Ax > 64*DOF || Ax < 0 || Ay > 64*DOF || Ay < 0) return 9999.0; // Out of bounds
    
    double dy = 64.0;
    double dx = 64/tan(ra);

    double rx = Ax;
    double ry = Ay;
    if (ra >= PI) { dx = -dx; dy = -dy; }

    if (mode == 1) SDL_RenderDrawPoint(renderer, rx, ry);

    for (int i=0; i<DOF; i++) {

        if (rx > 64*DOF || ry > 64*DOF || rx < 0 || ry < 0) return 999999.0;

        if (mode == 1) SDL_RenderDrawPoint(renderer, rx, ry);

        int pos = ((int)floor(rx/64) % MAPC + (MAPC*((int)floor(ry / 64))));
        if (room[pos] != 0) {
            SDL_Rect rect = {(int)(floor(rx/64) * 64), floor(ry/64) * 64, 64, 64};
            if (mode == 1) SDL_RenderFillRect(renderer, &rect);
            // return abs(ry-py) / sin(ra);
            rp = rx;
            mv = room[pos];
            return sqrt(pow(rx-px, 2) + pow(ry-py, 2));
        }
        
        rx += dx;
        ry += dy;

    }

    return 999999.0;
}

double getVerticalDistance(double ra, double &rp, int &mv) {
        
    double Ax = floor(px/64) * 64;
    if (ra >= PI/2 && ra <= (3*PI)/2) Ax -= 0.1; // Looking left
    else Ax += 64; // Looking right

    double Ay = py + (px-Ax) / (-1/tan(ra));
    
    double dx = 64.0;
    double dy = 64/(1/tan(ra));

    double ry = Ay;
    double rx = Ax;
    if (ra >= PI/2 && ra <= (3*PI)/2) { dx = -dx; dy = -dy; }

    if (mode == 1) SDL_RenderDrawPoint(renderer, rx, ry);

    for (int i=0; i<DOF; i++) {

        if (rx > 64*DOF || ry > 64*DOF || rx < 0 || ry < 0) return 999999.0;

        if (mode == 1) SDL_RenderDrawPoint(renderer, rx, ry);

        int pos = ((int)floor(rx/64) % MAPC + (MAPC*((int)floor(ry / 64))));
        if (room[pos] != 0) {
            SDL_Rect rect = {(int)(floor(rx/64) * 64), floor(ry/64) * 64, 64, 64};
            if (mode == 1) SDL_RenderFillRect(renderer, &rect);
            // return abs(ry-py) / sin(ra);
            rp = ry;
            mv = room[pos];
            return sqrt(pow(rx-px, 2) + pow(ry-py, 2));
        }

        
        rx += dx;
        ry += dy;
    }

    return 999999.0;
}

/*
    ra : Ray Angle (Radians)
    rp : Ray Position (X or Y)
    mv : Map Value
*/

void drawRay() {

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    double rad = FOV / PWIDTH;
    double ra = pa - (FOV/2);
    double rpH, rpV;
    int mvH, mvV;

    for (int r = 0; r<PWIDTH; r++) {
        ra += rad;
        fixAngle(ra);

        if (ra != PI || ra != 0) { 
            double distH = getHorizontalDistance(degToRad(ra), rpH, mvH);
            double distV = getVerticalDistance(degToRad(ra), rpV, mvV);

            double dist = distH;
            double rp = rpH;
            int mapV = mvH;

            double brightness = 1.0;
            if (distV < distH) {dist = distV; rp = rpV; mapV = mvV; brightness = 0.2;}

            double ca=pa-ra;
            fixAngle(ca); 
            dist=dist*cos(degToRad(ca));  

            if (mode == 0) {
                double height = 64.0 / dist * projectionDist;
                if (height > HEIGHT) height = HEIGHT;

                rp = (int)rp % 64;
                double tStep = 1.0 * texHeight / height;                                                                 //texture step
                u_int8_t cr,cg,cb;

                int startY = (PHEIGHT/2)-(height/2);

                for (int h=0; h<height;h++) {

                    int texPos = (rp * 64) + (tStep * h); if (texPos > 4095) texPos = 4095; if (texPos < 0) texPos = 0; // Cap the index for the texture just incase
                    double brightness = (1/dist) * 500; if (brightness > 1) brightness = 1; // Fog value here

                    array<u_int8_t, 3> pixelValue = textures.textureToWall(mapV, rp, tStep * h);

                    SDL_SetRenderDrawColor(renderer, pixelValue[0] * brightness, pixelValue[1] * brightness, pixelValue[2] * brightness, 255);
                    SDL_RenderDrawPoint(renderer, r, startY+h);
                }
            }
            
        }
    }

    return;
}

void drawGrid() {

    // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // for (int index = 0; index<MAPR*MAPC; index++) {
    //     if (room[index] != 1) {
    //         int x = (index % MAPC) * 64 / mapMultiplier;
    //         int y = (index / MAPC) * 64 / mapMultiplier;
    //         SDL_Rect rect = {x,y,mapMultiplier,mapMultiplier};
    //         SDL_RenderFillRect(renderer, &rect);
    //     }
    // }
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // for (int x = 0; x < MAPR; x += 1) {
    //     SDL_RenderDrawLine(renderer, 0, x * mapMultiplier, WIDTH, x * mapMultiplier);
    // }
    // for (int y = 0; y < MAPC; y += 1) {
    //     SDL_RenderDrawLine(renderer, y * mapMultiplier, 0, y * mapMultiplier, HEIGHT);
    // }

}

// Main Draw Loop
void display() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (mode == 0) {

        SDL_SetRenderDrawColor(renderer, 25, 25, 50, 255);
        SDL_Rect floor = {0, PHEIGHT/2, PWIDTH, PHEIGHT};
        SDL_RenderFillRect(renderer, &floor);

        drawRay();
    }
    else if (mode == 1) {
        // drawGrid();
        editor.drawMap(renderer);
        drawPlayer();
    }

    SDL_RenderPresent(renderer);
}

// Main loop
int main(int argc, char *argv[])
{
    printf("Starting\n");

    px = 4*64 - 32;
    py = 4*64 - 32;
    pa = 300;
    pdx = 0;
    pdy = 0;
    pSpeed = 10;

    textures = LTextures();
    textures.generateTextures();

    editor.loadTemplate("./bin/file.bin");
    room = editor.getMap();

    projectionDist = (PWIDTH/2) / tan(degToRad(FOV/2));

    if (createWindow() != 0) return 1;

    while (!finished) {
        SDL_Event event;
        handleInputs(event);

        display();

    }

    return 0;
}