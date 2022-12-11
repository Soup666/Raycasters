#include <iostream>
#include <SDL2/SDL.h>

#define mapX  8      //map width
#define mapY  8      //map height
#define mapS 64      //map cube size
#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512
#define FOV 60
#define PI 3.14159265
#define PLAYER_WIDTH 5
#define PRAY 50
#define PLAYER_SPEED 2

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int frame = 0;

bool A = false,
     S = false,
     D = false,
     W = false;


int map[] = {
    1,1,1,1,1,1,1,1,
    1,0,1,0,0,0,0,1,
    1,0,1,0,0,0,0,1,
    1,0,1,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,1,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,	
};

int px = WINDOW_HEIGHT/2 - 32,
    py = WINDOW_HEIGHT/2 - 32,
    pa = 150;
double pdx = 0,
       pdy = 0;


double degToRad(int angle) {
    return angle * (PI/180);
}

int radToDeg(double angle) {
    return angle * (180/PI);
}

int fixAngle(int angle) {
    if (angle > 359) {
        angle -= 360;
    }
    if (angle < 0) {
        angle += 360;
    }
    return angle;
}

void drawMap() {
    int x, y, xo, yo;

    for(y = 0; y < mapY; y++) {
        for(x = 0; x < mapX; x++) {

            if (map[y * mapX + x] == 1){ 
                SDL_SetRenderDrawColor(renderer, 255,0,0,255);
            } 
            else
            { 
                SDL_SetRenderDrawColor(renderer, 0,0,0,255);
            }

            xo = x * mapS; yo = y * mapS;

            SDL_Rect rect = {xo+1, yo+1, mapS, mapS};
            SDL_RenderFillRect(renderer, &rect);
        } 
    }


    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    for (y = 0; y < mapY; y++ ) {
        SDL_RenderDrawLine(renderer, 0, y*mapS, WINDOW_WIDTH, y*mapS);
    }

    for (x = 0; x < mapX; x++) {
        SDL_RenderDrawLine(renderer, x*mapS, 0, x*mapS, WINDOW_HEIGHT);
    }
}

void drawPlayer() {
    SDL_SetRenderDrawColor(renderer, 0,255,0,255);
    SDL_Rect rect = {px-(PLAYER_WIDTH/2), py-(PLAYER_WIDTH/2), PLAYER_WIDTH, PLAYER_WIDTH};
    SDL_RenderFillRect(renderer, &rect);


    SDL_RenderDrawLine(renderer, px, py, px+(PRAY*cos(degToRad(pa))), py-(PRAY*sin(degToRad(pa))));
}

void drawRays() {
    int ra, xo, yo, ax, ay, dof, vx, vy, hx, hy, distV, distH, rx, ry;

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    vx = 0;
    vy = 0;
    hx = 0;
    hy = 0;
    rx = 0;
    ry = 0;
    distV = 0;
    distH = 0;

    // ra = pa-30;
    dof = 0;

    for (int i = 0; i < 60; i++) {

        ra = fixAngle(pa-30+i);
        // std::cout << i << " " << ra << std::endl;

        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);

        dof = 0;
        // Horizontal Lines
        if      (sin(degToRad(ra)) > 0.01)  { ay = floor(py/64) * 64 - 1; yo = -64; xo =  64/tan(degToRad(ra)); ax = px + (py - ay) / tan(degToRad(ra)); }
        else if (sin(degToRad(ra)) < -0.01) { ay = floor(py/64) * 64 + 64; yo = 64; xo = -64/tan(degToRad(ra)); ax = px + (py - ay) / tan(degToRad(ra)); }
        else                             { rx = px; ry = py; dof = 8; }
        
        while (dof < 8) {
            rx = ax+(xo*dof);
            ry = ay+(yo*dof);

            if (rx < WINDOW_WIDTH) SDL_RenderDrawLine(renderer, px, py, rx, ry);

            if (rx > WINDOW_WIDTH || ry > WINDOW_HEIGHT) { break; }

            SDL_Rect rect = {rx, ry, 5, 5};
            SDL_RenderFillRect(renderer, &rect);

            int mx, my;

            mx = floor(rx / mapS);
            my = floor(ry / mapS);

            if (mx >= mapX || my >= mapY || my < 0 || mx < 0) { break; }
            if (map[mx+(my*mapY)] != 0) { 
                hx = rx; hy = ry; 
                break; 
                }

            dof++;
        }

        // Vertical Lines
        dof = 0;

        if      (cos(degToRad(ra)) < -0.01)  { ax = floor(px/64) * 64 - 1; xo = -64; yo = 64*tan(degToRad(ra));  ay = py + (px - ax) * tan(degToRad(ra)); }
        else if (cos(degToRad(ra)) > 0.01) { ax = floor(px/64) * 64 + 64; xo = 64; yo = -64*tan(degToRad(ra)); ay = py + (px - ax) * tan(degToRad(ra)); }
        else                             { rx = px; ry = py; dof = 8; }
        

        while (dof < 8) {
            rx = ax+(xo*dof);
            ry = ay+(yo*dof);

            if (rx < WINDOW_WIDTH) SDL_RenderDrawLine(renderer, px, py, rx, ry);

            if (rx > WINDOW_WIDTH || ry > WINDOW_HEIGHT) { break; }

            SDL_Rect rect = {rx, ry, 5, 5};
            SDL_RenderFillRect(renderer, &rect);


            int mx, my;

            mx = rx >> 6;
            my = ry >> 6;

            if (mx >= mapX || my >= mapY || my < 0 || mx < 0) { break; }
            if (map[mx+(my*mapY)] != 0) { 
                vx = rx; vy = ry; 
                break; }

            dof++;
        }

        // Distance
        distV = sqrt((px-vx) * (px-vx) + (py-vy) * (py-vy));
        distH = sqrt((px-hx) * (px-hx) + (py-hy) * (py-hy));
        int dist;

        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        if (distV > distH) {
            dist = distH;
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_Rect rect1 = {(hx >> 6) << 6, (hy >> 6) << 6, 64, 64};
            SDL_RenderFillRect(renderer, &rect1);


        } else {
            dist = distV;
            SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
            SDL_Rect rect1 = {(vx >> 6) << 6, (vy >> 6) << 6, 64, 64};
            SDL_RenderFillRect(renderer, &rect1);
        }

        int ca=fixAngle(pa-ra); dist=dist*cos(degToRad(ca));                                                    //fix fisheye 
        int lineH = (mapS * WINDOW_HEIGHT) / (dist); if(lineH > WINDOW_HEIGHT){ lineH = WINDOW_HEIGHT;} //line height and limit
        int lineOff = (WINDOW_HEIGHT / 2) - (lineH>>1);                                                     //line offset
        
        SDL_Rect rect3 = { (WINDOW_WIDTH) + (i * (WINDOW_WIDTH / 60)), lineOff, (WINDOW_WIDTH / 60), lineH};
        SDL_RenderFillRect(renderer, &rect3);
    }

}


void display() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawMap();
    drawRays();
    drawPlayer();

}

int main () {
    // Simple SDL Screen
    bool quit = false;

    // Display Screen
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Basic Raycaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH*2, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN: 
                    switch(event.key.keysym.sym) {
                        case SDLK_w: W = true; break;
                        case SDLK_s: S = true; break;
                        case SDLK_a: A = true; break;
                        case SDLK_d: D = true; break;
                        case SDLK_ESCAPE: SDL_DestroyWindow(window); SDL_Quit(); exit(0); break;
                    }
                    break;
                case SDL_KEYUP:
                    switch(event.key.keysym.sym) {
                        case SDLK_w: W = false; break;
                        case SDLK_s: S = false; break;
                        case SDLK_a: A = false; break;
                        case SDLK_d: D = false; break;
                    }
                    break;
            }
        }

        if(A){ pa+=1; pa = fixAngle(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));} 	
        if(D){ pa-=1; pa = fixAngle(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));} 
        if(W){ px+=pdx*PLAYER_SPEED; py+=pdy*PLAYER_SPEED;}
        if(S){ px-=pdx*PLAYER_SPEED; py-=pdy*PLAYER_SPEED;}


        display();

        SDL_RenderPresent(renderer);

        frame++;


    }
    
    return 0;
}