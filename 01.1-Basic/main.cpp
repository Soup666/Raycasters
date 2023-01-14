#include <iostream>
#include <SDL2/SDL.h>

#define FOV 60.0
#define WIDTH 512
#define HEIGHT 512
#define PI 3.14
#define DEBUG 0
#define PHEIGHT 240
#define PWIDTH 320

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool finished = false;

bool W = false;
bool S = false;
bool D = false;
bool A = false;

int room[64] = {
    1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,1,
    1,0,1,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,1,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,
};

double px, py, pa, projectionDist, pdx, pdy, pSpeed;

void fixAngle(double &a) {
    if (a < 0) a += 360.0;
    if (a > 360.0) a -= 360.0;
}

double degToRad(double a) {
    return a*(PI/180.0);
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
                        case SDLK_LSHIFT: pSpeed = 3; break;
                        case SDLK_ESCAPE: SDL_DestroyWindow(window); SDL_Quit(); exit(0); break;
                    }
                    break;
                case SDL_KEYUP:
                    switch(event.key.keysym.sym) {
                        case SDLK_w: W = false; break;
                        case SDLK_s: S = false; break;
                        case SDLK_a: A = false; break;
                        case SDLK_d: D = false; break;
                        case SDLK_LSHIFT: pSpeed = 1; break;
                    }
                    break;
            }
        }

        if(A) {pa-=2; fixAngle(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));}
        if(D) {pa+=2; fixAngle(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));}
        if(W){px+=(pdx) * pSpeed; py-=(pdy) * pSpeed; }
        if(S){px-=(pdx) * pSpeed; py+=(pdy) * pSpeed; }
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
    SDL_Rect rect = { ceil(px-2.5), ceil(py-2.5), 5, 5 };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderDrawLine(renderer, px, py, px + 10 * cos(degToRad(pa)), py + 10 * sin(degToRad(pa)));
}

double getHorizontalDistance(double ra) {
        
    double Ay = floor(py/64) * 64;
    if (ra >= PI) Ay -= 0.1; // Looking down
    else Ay += 64; // Looking up

    double Ax = px + (py-Ay) / -tan(ra);
    if (Ax > WIDTH || Ax < 0 || Ay > HEIGHT || Ay < 0) return 9999.0; // Out of bounds
    
    double dy = 64.0;
    double dx = 64/tan(ra);

    double rx = Ax;
    double ry = Ay;
    if (ra >= PI) { dx = -dx; dy = -dy; }

    if (DEBUG) SDL_RenderDrawPoint(renderer, rx, ry);

    for (int i=0; i<5; i++) {

        if (rx > HEIGHT || ry > WIDTH || rx < 0 || ry < 0) return 999999.0;

        // SDL_Rect rect2 = {(int)rx,(int)ry,4,4};
        // SDL_Rect rect2 = {(int)(floor(rx/64) * 64), floor(ry/64) * 64, 64, 64};
        // SDL_RenderFillRect(renderer, &rect2);


        if (DEBUG) SDL_RenderDrawPoint(renderer, rx, ry);

        int pos = ((int)floor(rx/64) % 8 + (8*((int)floor(ry / 64))));
        if (room[pos] != 0) {
            SDL_Rect rect = {(int)(floor(rx/64) * 64), floor(ry/64) * 64, 64, 64};
            if (DEBUG) SDL_RenderFillRect(renderer, &rect);
            // return abs(ry-py) / sin(ra);
            return sqrt(pow(rx-px, 2) + pow(ry-py, 2));
        }

        
        rx += dx;
        ry += dy;

    }

    return 999999.0;
}

double getVerticalDistance(double ra) {
        
    double Ax = floor(px/64) * 64;
    if (ra >= PI/2 && ra <= (3*PI)/2) Ax -= 0.1; // Looking left
    else Ax += 64; // Looking right

    double Ay = py + (px-Ax) / (-1/tan(ra));
    
    double dx = 64.0;
    double dy = 64/(1/tan(ra));

    double ry = Ay;
    double rx = Ax;
    if (ra >= PI/2 && ra <= (3*PI)/2) { dx = -dx; dy = -dy; }

    if (DEBUG) SDL_RenderDrawPoint(renderer, rx, ry);

    for (int i=0; i<5; i++) {

        if (rx > HEIGHT || ry > WIDTH || rx < 0 || ry < 0) return 999999.0;

        if (DEBUG) SDL_RenderDrawPoint(renderer, rx, ry);

        int pos = ((int)floor(rx/64) % 8 + (8*((int)floor(ry / 64))));
        if (room[pos] != 0) {
            SDL_Rect rect = {(int)(floor(rx/64) * 64), floor(ry/64) * 64, 64, 64};
            if (DEBUG) SDL_RenderFillRect(renderer, &rect);
            // return abs(ry-py) / sin(ra);
            return sqrt(pow(rx-px, 2) + pow(ry-py, 2));
        }

        
        rx += dx;
        ry += dy;
    }

    return 999999.0;
}

void drawRay() {

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    double rad = FOV / PWIDTH;
    double ra = pa - (FOV/2);

    for (int r = 0; r<PWIDTH; r++) {
        ra += rad;
        fixAngle(ra);

        if (ra != PI || ra != 0) { 
            double distH = getHorizontalDistance(degToRad(ra));
            // double distH = 0.0;
            double distV = getVerticalDistance(degToRad(ra));
            // double distV = 0.0;

            double dist = distH;

            SDL_SetRenderDrawColor(renderer, 255,255,255,255);
            if (distV < distH) {dist = distV; SDL_SetRenderDrawColor(renderer, 100,255,255,255);}

            double ca=pa-ra;
            fixAngle(ca); 
            dist=dist*cos(degToRad(ca));  

            if (!DEBUG) {
                double height = 64.0 / dist * projectionDist;
                SDL_Rect line = {r, (PHEIGHT/2)-(height/2), 1, height};
                SDL_RenderFillRect(renderer, &line);
            } 
            
        }
    }

    // std::cout << Ax << " " << tan(ra) << std::endl;
    return;
}

void drawGrid() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int index = 0; index<64; index++) {
        if (room[index] == 1) {
            int x = (int)(index%8)*64;
            int y = (int(floor(index/8)))*64;
            // std::cout << y << std::endl;
            SDL_Rect rect = {x,y,64,64};
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    for (int x = 0; x < 8*64; x += 64) {
        SDL_RenderDrawLine(renderer, 0, x, 8*64, x);
    }
    for (int y = 0; y < 8*64; y += 64) {
        SDL_RenderDrawLine(renderer, y, 0, y, 8*64);
    }
}

// Main Draw Loop
void display() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (DEBUG) drawGrid();
    if (DEBUG) drawPlayer();
    drawRay();

    SDL_RenderPresent(renderer);
}

// Main loop
int main(int argc, char *argv[])
{
    printf("Starting");

    px = 4*64 - 32;
    py = 4*64 - 32;
    pa = 300;
    pdx = 0;
    pdy = 0;
    pSpeed = 1;

    projectionDist = (PWIDTH/2) / tan(degToRad(FOV/2));

    if (createWindow() != 0) return 1;

    while (!finished) {
        SDL_Event event;
        handleInputs(event);

        display();


    }

    return 0;
}