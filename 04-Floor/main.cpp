
// ---------------------------------------------- Includes ----------------------------------------------

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <sstream>
#include "LTimer.h"

#define mapX  16      //map width
#define mapY  16      //map height
#define mapS 32      //map cube size
#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512
#define FOV 60
#define PI 3.14159265
#define PLAYER_WIDTH 5
#define PRAY 50
#define SCREEN_FPS 60
#define SCREEN_TICKS_PER_FRAME 1000 / SCREEN_FPS
#define DOF 16
#define RAYCOUNT 240

// ---------------------------------------------- Variables ----------------------------------------------

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font* font;


Uint32 totalFrameTicks = 0;
Uint32 frameCount = 0;

int frame = 0;
int player_speed = 1;

bool A = false,
     S = false,
     D = false,
     W = false;


int map[] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,
    1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,
    1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,
    1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

// int map[] = {
//     1,1,1,1,1,1,1,1,
//     1,0,1,0,0,0,0,1,
//     1,0,1,0,0,0,0,1,
//     1,0,1,0,0,0,0,1,
//     1,0,0,0,0,0,0,1,
//     1,0,0,0,0,1,0,1,
//     1,0,0,0,0,0,0,1,
//     1,1,1,1,1,1,1,1,	
// };



double px = WINDOW_HEIGHT/2,
    py = WINDOW_HEIGHT/2,
    pa = 90,
    pdx = 0,
    pdy = 0,
    pdt = 0,
    movent_multiplier = 0.1;

// ---------------------------------------------- Functions ----------------------------------------------

double degToRad(int angle) {
    return angle * (PI/180);
}

int radToDeg(double angle) {
    return angle * (180/PI);
}

double fixAngle(double angle) {
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

void drawRays()
{
    // Clear background
    SDL_SetRenderDrawColor(renderer, 0,255,255,255); 	
    SDL_Rect rect = {WINDOW_WIDTH, 0, WINDOW_WIDTH*2, WINDOW_HEIGHT/2};
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0,25,0,255); 
    SDL_Rect rect2 = {WINDOW_WIDTH, WINDOW_HEIGHT / 2, WINDOW_WIDTH*2, WINDOW_HEIGHT / 2};
    SDL_RenderFillRect(renderer, &rect2);
	
    int mx,my,mp,dof,side; float vx,vy,rx,ry,ra,xo,yo,disV,disH; 
 
    ra=fixAngle(pa+30); //ray set back 30 degrees
 
    int rayCount = 0;
    for(double r = 0; r<RAYCOUNT; r++)
    {

        //---Vertical--- 
        dof=0; side=0; disV=100000;
        float Tan=tan(degToRad(ra));

        if     (cos(degToRad(ra))> 0.001){ rx=(((int)px>>5)<<5)+mapS;      ry=(px-rx)*Tan+py; xo= mapS; yo=-xo*Tan;} //looking left
        else if(cos(degToRad(ra))<-0.001){ rx=(((int)px>>5)<<5) -0.0001; ry=(px-rx)*Tan+py; xo=-mapS; yo=-xo*Tan;} //looking right
        else {                             rx=px;                        ry=py; dof=DOF;}                          //looking up or down. no hit  

        while(dof<DOF) 
        { 
            mx=(int)(rx)>>5; my=(int)(ry)>>5; mp=my*mapX+mx;                     
            if(mp>0 && mp<mapX*mapY && map[mp]==1){ dof=DOF; disV=cos(degToRad(ra))*(rx-px)-sin(degToRad(ra))*(ry-py);} //hit         
            else{ rx+=xo; ry+=yo; dof+=1;}                                                                            //check next horizontal
        } 
        vx=rx; vy=ry;

        //---Horizontal---
        dof=0; disH=100000;
        Tan=1.0/Tan; 


        if     (sin(degToRad(ra))> 0.001){ ry=(((int)py>>5)<<5) -0.0001; rx=(py-ry)*Tan+px; yo=-mapS; xo=-yo*Tan;} //looking up 
        else if(sin(degToRad(ra))<-0.001){ ry=(((int)py>>5)<<5)+mapS;      rx=(py-ry)*Tan+px; yo= mapS; xo=-yo*Tan;} //looking down
        else{ rx=px; ry=py; dof=DOF;}                                                                              //looking straight left or right
        
        while(dof<DOF) 
        { 
            mx=(int)(rx)>>5; my=(int)(ry)>>5; mp=my*mapX+mx;                          
            if(mp>0 && mp<mapX*mapY && map[mp]==1){ dof=DOF; disH=cos(degToRad(ra))*(rx-px)-sin(degToRad(ra))*(ry-py);} //hit         
            else{ rx+=xo; ry+=yo; dof+=1;}                                                                            //check next horizontal
        } 
        
        int pixelX;
        int textureSize = 16;
        int wallCol[4] = {0,200,0,255};

        if(disV<disH){ // Looking at Vertical
            rx=vx; 
            ry=vy; 
            disH=disV; 
            
            pixelX = (int)ry % 64;

            // wallCol[0] = 0;
            // wallCol[1] = 150;
            // wallCol[2] = 0;
            // wallCol[3] = 255;
        }   //horizontal hit first
        else {
            pixelX = (int)rx % 64;
        }
        
        float normalizedPixelX = (float)(pixelX) / (64);
        // SDL_SetRenderDrawColor(renderer, wallCol[0],wallCol[1] * normalizedPixelX,wallCol[2],wallCol[3]);
        if (rx < WINDOW_WIDTH && ry < WINDOW_HEIGHT) SDL_RenderDrawLine(renderer, px, py, rx, ry);                                              //draw 2D ray
        

        // std::cout << "PixelX: " << normalizedPixelX  << std::endl;
            
        int ca=fixAngle(pa-ra); disH=disH*cos(degToRad(ca));                                                    //fix fisheye 
        int lineH = (mapS * WINDOW_HEIGHT) / (disH); if(lineH > WINDOW_HEIGHT){ lineH = WINDOW_HEIGHT;} //line height and limit
        int lineOff = (WINDOW_HEIGHT / 2) - (lineH>>1);                                                     //line offset
        
        for (double y = 0.0; y <= lineH; y += (lineH / (float)textureSize)) {
            int brightness = y; if (brightness > 255) brightness = 255;
            SDL_SetRenderDrawColor(renderer, wallCol[0], brightness,wallCol[2],wallCol[3]);
            if (normalizedPixelX < 0.1) {
                SDL_SetRenderDrawColor(renderer, 0, 25 * (brightness / 255), 0, 255);
            }
            if (y > lineH-(lineH / (float)textureSize)) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }
            SDL_Rect wall_pixel = { 
                (WINDOW_WIDTH) + (rayCount * ((WINDOW_WIDTH*2) / RAYCOUNT)), 
                lineOff + y + ((player_speed-1)*2*sin(frameCount)), 
                ((WINDOW_WIDTH*2) / (RAYCOUNT*2)), 
                (lineH / (float)textureSize) + 1
            };
            SDL_RenderFillRect(renderer, &wall_pixel);
        }
        // SDL_Rect rect3 = { 
        //     (WINDOW_WIDTH) + (rayCount * ((WINDOW_WIDTH*2) / RAYCOUNT)), 
        //     ((player_speed-1)*2*sin(frameCount)) + lineOff, 
        //     ((WINDOW_WIDTH*2) / RAYCOUNT), 
        //     lineH
        // };
        // SDL_RenderFillRect(renderer, &rect3);

        // int count = 0;
        // int verticalSubset = 16;
        // for (int y = 0; y < lineH; y += (lineH / (float)verticalSubset)) {
            
        //     SDL_SetRenderDrawColor(renderer, count ,wallCol[1] * normalizedPixelX,0,wallCol[3]);
        //     // SDL_RenderDrawPoint(renderer, (WINDOW_WIDTH / 2) + (rayCount * (WINDOW_WIDTH / RAYCOUNT)), lineOff + y);

        //     SDL_Rect rect3 = { (WINDOW_WIDTH / 2) + (rayCount * (WINDOW_WIDTH / RAYCOUNT)), lineOff + y, (WINDOW_WIDTH / RAYCOUNT), (lineH / (float)verticalSubset)};
        //     SDL_RenderFillRect(renderer, &rect3);
        //     count = ((y) / ((float)lineH)) * 100;
        // }

        rayCount++;
        ra=fixAngle(ra-(60.0 / RAYCOUNT)); //go to next ray
    }
    exit;
}

void renderText(std::string text, SDL_Rect dest) {
	SDL_Color fg = {255, 255, 255, 0};
	SDL_Surface* surf = TTF_RenderText_Solid(font, text.c_str(), fg);

	dest.w = surf->w;
	dest.h = surf->h;

	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

	SDL_RenderCopy(renderer, tex, NULL, &dest);
	SDL_DestroyTexture(tex);
	SDL_FreeSurface(surf);
}

void display() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawMap();
    drawRays();
    drawPlayer();

}

// ---------------------------------------------- Main ----------------------------------------------

int main () {

    TTF_Init();
    font = TTF_OpenFont("./FreeSans.ttf", 24);


    LTimer fpsTimer;
    LTimer capTimer;
    frameCount = 0;

    fpsTimer.start();

    if (font == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }

    // Simple SDL Screen
    bool quit = false;

    // Display Screen
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Frame Rate Limited", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH*3, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);


    while (!quit) {

        capTimer.start();
        
		// Start frame timing

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
                        case SDLK_LSHIFT: player_speed = 3; break;
                        case SDLK_ESCAPE: SDL_DestroyWindow(window); SDL_Quit(); exit(0); break;
                    }
                    break;
                case SDL_KEYUP:
                    switch(event.key.keysym.sym) {
                        case SDLK_w: W = false; break;
                        case SDLK_s: S = false; break;
                        case SDLK_a: A = false; break;
                        case SDLK_d: D = false; break;
                        case SDLK_LSHIFT: player_speed = 1; break;
                    }
                    break;
            }
        }

        if(A){ pa+=2; pa = fixAngle(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));} 	
        if(D){ pa-=2; pa = fixAngle(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));} 
        if(W){px+=(pdx*player_speed); py+=(pdy*player_speed); }
        if(S){px-=(pdx*player_speed); py-=(pdy*player_speed); }


        float avgFPS = frameCount / ( fpsTimer.getTicks() / 1000.f );
        if( avgFPS > 2000000 )
        {
            avgFPS = 0;
        }

		// Strings to display
		std::string fps = "Average FPS: " + std::to_string(avgFPS);
		// std::string avg = "Average FPS: " + std::to_string(1000.0f / ((float)totalFrameTicks / totalFrames));
		std::string perf = "Current Ticks: " + std::to_string(movent_multiplier);

        display(); // Display Frame

		// Display strings
		SDL_Rect dest = { 10, 10, 0, 0 };
		renderText(fps, dest);
		// dest.y += 40;
		// renderText(avg, dest);
		dest.y += 40;
		renderText(perf, dest);

        SDL_RenderPresent(renderer);

        ++frameCount;

        //If frame finished early
        int frameTicks = capTimer.getTicks();
        if( frameTicks < SCREEN_TICKS_PER_FRAME )
        {
            //Wait remaining time
            SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
        }
    }
    
    return 0;
}
