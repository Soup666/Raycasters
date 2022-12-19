#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <sstream>

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
#define SCREEN_FPS 60
#define SCREEN_TICKS_PER_FRAME 1000 / SCREEN_FPS

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font* font;

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


// ---------------------------------------------- Timer ----------------------------------------------

class LTimer
{
    public:
		//Initializes variables
		LTimer();

		//The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();

		//Gets the timer's time
		Uint32 getTicks();

		//Checks the status of the timer
		bool isStarted();
		bool isPaused();

    private:
		//The clock time when the timer started
		Uint32 mStartTicks;

		//The ticks stored when the timer was paused
		Uint32 mPausedTicks;

		//The timer status
		bool mPaused;
		bool mStarted;
};

LTimer::LTimer()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}


void LTimer::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
    }
}

void LTimer::unpause()
{
    //If the timer is running and paused
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;
    }
}

Uint32 LTimer::getTicks()
{
	//The actual timer time
	Uint32 time = 0;

    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

bool LTimer::isStarted()
{
	//Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused()
{
	//Timer is running and paused
    return mPaused && mStarted;
}


// ---------------------------------------------- Main ----------------------------------------------

double px = WINDOW_HEIGHT/2 - 32,
    py = WINDOW_HEIGHT/2 - 32,
    pa = 150,
    pdx = 0,
    pdy = 0,
    pdt = 0.;


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

            if (mx >= mapX || my >= mapY || my < 0 || mx < 0 || ( mx == 0 && my == 0)) { break; }
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

            if (mx >= mapX || my >= mapY || my < 0 || mx < 0 || ( mx == 0 && my == 0)) { break; }
            if (map[mx+(my*mapY)] != 0) { 
                vx = rx; vy = ry; 
                break; }

            dof++;
        }

        // Distance
        distV = sqrt((px-vx) * (px-vx) + (py-vy) * (py-vy));
        distH = sqrt((px-hx) * (px-hx) + (py-hy) * (py-hy));
        double dist;

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
        
        SDL_Rect rect3 = { (WINDOW_WIDTH * 2) - (i * (WINDOW_WIDTH / 60)), lineOff, (WINDOW_WIDTH / 60), lineH};
        SDL_RenderFillRect(renderer, &rect3);
    }

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

LTimer fpsTimer;
LTimer capTimer;
int frameCount = 0;

int main () {

    TTF_Init();
    font = TTF_OpenFont("./FreeSans.ttf", 24);

    fpsTimer.start();

    if (font == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }

	Uint32 totalFrameTicks = 0;
	Uint32 totalFrames = 0;
    // Simple SDL Screen
    bool quit = false;

    // Display Screen
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Frame Rate Limited", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH*2, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


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

        if(A){ pa+=2; pa = fixAngle(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));} 	
        if(D){ pa-=2; pa = fixAngle(pa); pdx=cos(degToRad(pa)); pdy=-sin(degToRad(pa));} 
        if(W){ px+=pdx*PLAYER_SPEED; py+=pdy*PLAYER_SPEED; }
        if(S){ px-=pdx*PLAYER_SPEED; py-=pdy*PLAYER_SPEED; }


        float avgFPS = frameCount / ( fpsTimer.getTicks() / 1000.f );
        if( avgFPS > 2000000 )
        {
            avgFPS = 0;
        }

		// Strings to display
		std::string fps = "Current FPS: " + std::to_string(avgFPS);
		// std::string avg = "Average FPS: " + std::to_string(1000.0f / ((float)totalFrameTicks / totalFrames));
		// std::string perf = "Current Perf: " + std::to_string(framePerf);


        display(); // Display Frame

		// Display strings
		SDL_Rect dest = { 10, 10, 0, 0 };
		renderText(fps, dest);
		// dest.y += 40;
		// renderText(avg, dest);
		// dest.y += 40;
		// renderText(perf, dest);

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
