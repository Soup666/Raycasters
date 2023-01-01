
// ---------------------------------------------- Includes ----------------------------------------------

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include "src/LTimer.h"

#define mapX  32      //map width
#define mapY  32      //map height
#define mapS 32      //map cube size
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define FOV 60
#define PI 3.14159265
#define PLAYER_WIDTH 5
#define PRAY 50
#define SCREEN_FPS 60
#define SCREEN_TICKS_PER_FRAME 1000 / SCREEN_FPS
#define DOF 16
#define RAYCOUNT 640
#define DEBUG 1
#define texWidth 64
#define texHeight 64

// ---------------------------------------------- Variables ----------------------------------------------

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Window *playerWindow = NULL;
SDL_Renderer *playerRenderer = NULL;
TTF_Font* font;


Uint32 totalFrameTicks = 0;
Uint32 frameCount = 0;

std::vector<uint32_t> texture[8];

int frame = 0;
int player_speed = 1;

bool A = false,
     S = false,
     D = false,
     W = false;


// int map[] = {
//     1,1,1,1,1,2,3,4,5,6,7,1,1,1,1,1,
//     1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
//     1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
//     1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
//     1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
//     1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
//     1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,
//     1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,
//     1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,
//     1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,
//     1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
//     1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
//     1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
//     1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
//     1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
//     1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
// };

int map[] = 
{
    4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7,7,7,7,7,7,7,7,7,
    4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,7,7,7,7,7,7,
    4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,7,7,7,7,7,7,
    4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7,7,7,7,7,7,7,7,7,
    4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7,7,7,7,7,7,7,7,7,
    4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1,7,7,7,7,7,7,7,7,
    4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,7,7,7,7,7,7,7,7,7,
    4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1,7,7,7,7,7,7,7,7,
    4,0,7,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,7,7,7,7,7,7,7,7,7,
    4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1,7,7,7,7,7,7,7,7,
    4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1,7,7,7,7,7,7,7,7,
    6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,
    7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,7,7,7,7,7,7,7,7,
    6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,
    4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3,7,7,7,7,7,7,7,7,
    4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2,7,7,7,7,7,7,7,7,
    4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2,7,7,7,7,7,7,7,7,
    4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2,7,7,7,7,7,7,7,7,
    4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2,7,7,7,7,7,7,7,7,
    4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2,7,7,7,7,7,7,7,7,
    4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2,7,7,7,7,7,7,7,7,
    4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2,7,7,7,7,7,7,7,7,
    4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3,7,7,7,7,7,7,7,7,
    4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2,7,7,7,7,7,7,7,7,
    4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2,7,7,7,7,7,7,7,7,
    4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2,7,7,7,7,7,7,7,7,
    4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2,7,7,7,7,7,7,7,7,
    4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2,7,7,7,7,7,7,7,7,
    4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2,7,7,7,7,7,7,7,7,
    4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2,7,7,7,7,7,7,7,7,
    4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3,7,7,7,7,7,7,7,7
};

double px = WINDOW_HEIGHT/2,
    py = WINDOW_HEIGHT/2,
    pa = 45,
    pdx = 0,
    pdy = 0,
    pdt = 0,
    movent_multiplier = 0.1;

// ---------------------------------------------- Functions ----------------------------------------------

double degToRad(double angle) {
    return angle * (PI/180);
}

double radToDeg(double angle) {
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

void generateTextures(std::vector<uint32_t> texture[]) {
    //generate some textures
  for(int x = 0; x < texWidth; x++)
  for(int y = 0; y < texHeight; y++)
  {
    int xorcolor = (x * 256 / texWidth) ^ (y * 256 / texHeight);
    //int xcolor = x * 256 / texWidth;
    int ycolor = y * 256 / texHeight;
    int xycolor = y * 128 / texHeight + x * 128 / texWidth;
    texture[0][texWidth * y + x] = 65536 * 254 * (x != y && x != texWidth - y); //flat red texture with black cross
    texture[1][texWidth * y + x] = xycolor + 256 * xycolor + 65536 * xycolor; //sloped greyscale
    texture[2][texWidth * y + x] = 256 * xycolor + 65536 * xycolor; //sloped yellow gradient
    texture[3][texWidth * y + x] = xorcolor + 256 * xorcolor + 65536 * xorcolor; //xor greyscale
    texture[4][texWidth * y + x] = 256 * xorcolor; //xor green
    texture[5][texWidth * y + x] = 65536 * 192 * (x % 16 && y % 16); //red bricks
    texture[6][texWidth * y + x] = 65536 * ycolor; //red gradient
    texture[7][texWidth * y + x] = 128 + 256 * 128 + 65536 * 128; //flat grey texture
  }
}

void drawMap() {
    int x, y, xo, yo;

    int cubeSize = 16;

    for(y = 0; y < mapY; y++) {
        for(x = 0; x < mapX; x++) {

            if (map[y * mapX + x] != 0){ 
                SDL_SetRenderDrawColor(playerRenderer, 255,0,0,255);
            } 
            else
            { 
                SDL_SetRenderDrawColor(playerRenderer, 0,0,0,255);
            }

            xo = x * cubeSize; yo = y * cubeSize;

            SDL_Rect rect = {xo+1, yo+1, cubeSize, cubeSize};
            SDL_RenderFillRect(playerRenderer, &rect);
        } 
    }


    // SDL_SetRenderDrawColor(playerRenderer, 255,255,255,255);
    // for (y = 0; y < mapY; y++ ) {
    //     SDL_RenderDrawLine(playerRenderer, 0, y*cubSize, WINDOW_WIDTH*2, y*cubSize);
    // }

    // for (x = 0; x < mapX; x++) {
    //     SDL_RenderDrawLine(playerRenderer, x*cubSize, 0, x*cubSize, WINDOW_HEIGHT);
    // }
}

void drawPlayer() {
    SDL_SetRenderDrawColor(playerRenderer, 0,255,0,255);
    SDL_Rect rect = {
        ((int)px>>1)-(PLAYER_WIDTH>>1), 
        ((int)py>>1)-(PLAYER_WIDTH>>1),
        PLAYER_WIDTH, 
        PLAYER_WIDTH
        };
    SDL_RenderFillRect(playerRenderer, &rect);


    SDL_RenderDrawLine(playerRenderer, (int)px>>1, (int)py>>1, ((int)px>>1)+(PRAY*cos(degToRad(pa))), ((int)py>>1)-(PRAY*sin(degToRad(pa))));
}

void drawRays()
{
    // Clear background
	
    int mx,my,mp,dof,side; float vx,vy,rx,ry,ra,xo,yo,disV,disH; 
 
    ra=fixAngle(pa+30); //ray set back 30 degrees
 
    int rayCount = 0;
    for(int r = 0; r<=RAYCOUNT; r++)
    {

        //---Vertical--- 
        dof=0; side=0; disV=100000;
        float Tan=tan(degToRad(ra));
        int mapH, mapV;

        if     (cos(degToRad(ra))> 0.001){ rx=(((int)px>>5)<<5)+mapS;      ry=(px-rx)*Tan+py; xo= mapS; yo=-xo*Tan;} //looking left
        else if(cos(degToRad(ra))<-0.001){ rx=(((int)px>>5)<<5) -0.0001; ry=(px-rx)*Tan+py; xo=-mapS; yo=-xo*Tan;} //looking right
        else {                             rx=px;                        ry=py; dof=DOF;}                          //looking up or down. no hit  

        while(dof<DOF) 
        { 
            mx=(int)(rx)>>5; my=(int)(ry)>>5; mp=my*mapX+mx;                     
            if(mp>0 && mp<mapX*mapY && map[mp]!=0){ dof=DOF; disV=cos(degToRad(ra))*(rx-px)-sin(degToRad(ra))*(ry-py); mapV = map[mp]; } //hit         
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
            if(mp>0 && mp<mapX*mapY && map[mp]!=0){ dof=DOF; disH=cos(degToRad(ra))*(rx-px)-sin(degToRad(ra))*(ry-py); mapH = map[mp]; } //hit         
            else{ rx+=xo; ry+=yo; dof+=1;}                                                                            //check next horizontal
        } 
        
        int pixelX;
        int mapValue;

        if(disV<disH){ // Looking at Vertical
            rx=vx; 
            ry=vy; 
            disH=disV; 
            mapValue = mapV;
            
            pixelX = (int)ry % 64; // Needs improving, slow

        }   //horizontal hit first
        else {
            pixelX = (int)rx % 64;
            mapValue = mapH;
        }
        
        int ca=fixAngle(pa-ra); disH=disH*cos(degToRad(ca));                                                    //fix fisheye 
        double lineH = (mapS * WINDOW_HEIGHT) / (disH); if(lineH > WINDOW_HEIGHT){ lineH = WINDOW_HEIGHT;}      //line height and limit
        double lineOff = (WINDOW_HEIGHT / 2) - (lineH / 2);                                                     //line offset
        double tStep = 1.0 * texHeight / lineH;                                                                 //texture step
        u_int8_t cr,cg,cb;

        for (int pyo = 0; pyo < lineH; pyo++) {
            int texPos = (pixelX * 64) + (tStep * pyo); if (texPos > 4095) texPos = 4095; // Cap the index for the texture just incase
            double brightness = (1/disH) * 50; if (brightness > 1) brightness = 1; // Fog value here
            cr = texture[mapValue][texPos] >> 24 & 0xFF; // color is stored as 32 bit int, with first 3 bytes being r,g,b and the 4th being extra. we can shift and AND to split it
            cg = texture[mapValue][texPos] >> 16 & 0xFF;
            cb = texture[mapValue][texPos] >> 8 & 0xFF;

            SDL_SetRenderDrawColor(renderer, cr*brightness, cg*brightness, cb*brightness, 255);
            SDL_RenderDrawPoint(renderer, rayCount, lineOff + pyo);
        }

        // int pYIndex = 0;
        // double pixelHeight = (lineH / 16.0);

        // for (double y = 0.0; y < lineH; y += pixelHeight) { // Y is pixel count height
        //     int brightness = 255-(disH / 2); if (brightness > 255) brightness = 255; if (brightness < 0) brightness = 0; // y is the height of the line drawn. so biger = closer
        //     SDL_SetRenderDrawColor(renderer, wallCol[0], brightness, wallCol[2], wallCol[3]);
        //     // if (normalizedPixelX <= 10) {
        //     //     SDL_SetRenderDrawColor(renderer, 0, 25 * (brightness / 255), 0, 255);
        //     // }
        //     // SDL_SetRenderDrawColor(renderer, 0, 255*, 0, 255);
        //     // if (y > lineH-(lineH / (float)textureSize)) {
        //     //     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Bottom Border
        //     // }
        //     SDL_SetRenderDrawColor(renderer, 0, (255*(wallArr[(pYIndex++*16)+(int)(((double)pixelX / 16.0)*4)]) / 255) * brightness, 0, 255); 

        //     int bobbingAmount = ((player_speed-1)*2*sin(frameCount));
        //     int segmentOff = (int)y;

        //     int lineWidth = WINDOW_WIDTH / RAYCOUNT;

        //     SDL_Rect wall_pixel = { 
        //         (rayCount * (lineWidth)), 
        //         (int)(lineOff) + segmentOff + bobbingAmount, 
        //         lineWidth, 
        //         (int)pixelHeight + 1
        //     };
        //     SDL_RenderFillRect(renderer, &wall_pixel);
        // }

        rayCount++;
        // std::cout << disH << " - " << lineH << std::endl;
        ra=fixAngle(ra-(60.0 / RAYCOUNT)); //go to next ray
    }

    // std::cout << "a" << std::endl;
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

    SDL_SetRenderDrawColor(renderer, 0,255,255,255); 	
    SDL_Rect rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT/2};
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0,25,0,255); 
    SDL_Rect rect2 = {0, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT / 2};
    SDL_RenderFillRect(renderer, &rect2);

    drawRays();

    if (DEBUG) {
        drawMap();
        drawPlayer();

    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect crosshair = {WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 5, 5};
    SDL_RenderFillRect(renderer, &crosshair);

}

// ---------------------------------------------- Main ----------------------------------------------

int main () {

    TTF_Init();
    font = TTF_OpenFont("./assets/FreeSans.ttf", 24);


    LTimer fpsTimer;
    LTimer capTimer;
    frameCount = 0;

    fpsTimer.start();

    // if( !wallTexture.loadPixelsFromFile( "assets/foo.png" ) )
    // {
    //     printf( "Unable to load Foo' texture!\n" );
    //     exit(EXIT_FAILURE);
    // }
    //  else
    // {
    //     //Get pixel data
    //     Uint32* pixels = wallTexture.getPixels32();
    //     int pixelCount = wallTexture.getPitch32() * wallTexture.getHeight();

    //      //Color key pixels
    //     for( int i = 0; i < pixelCount; ++i )
    //     {
    //         std::cout << pixels[i] << std::endl;
    //     }

    //     //Create texture from manually color keyed pixels
    //     if( !wallTexture.loadFromPixels() )
    //     {
    //         printf( "Unable to load Foo' texture from surface!\n" );
    //     }
    // }


    for(int i = 0; i < 8; i++) texture[i].resize(texWidth * texHeight);

    generateTextures(texture);  // Temp Textures to use

    if (font == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }

    // Simple SDL Screen
    bool quit = false;

    // Display Screen
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Frame Rate Limited", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (DEBUG) { playerWindow = SDL_CreateWindow("Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 512, 512, SDL_WINDOW_SHOWN);
    playerRenderer = SDL_CreateRenderer(playerWindow, 0, 0); }

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
        if (DEBUG) SDL_RenderPresent(playerRenderer);

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
