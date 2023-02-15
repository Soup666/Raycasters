/*
    Main game manager class.
    Handles all game logic and rendering the screen.
    The bulk of the raycasters code is here.
*/

#include "LGameManager.h"

LGameManager::LGameManager() {}

int LGameManager::getMapPos(double x, double y) {

    return((int)floor(x/64) % MAPC + (MAPC*((int)floor(y / 64))));
}

// Creates the window and renderer
int LGameManager::createWindow() {
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

LGameManager::~LGameManager() {}

void LGameManager::Init()
{

    IMG_Init(IMG_INIT_PNG);

    // sin and cos tables
    for (int i = 0; i < 360; i++) {
        sinTable[i] = sin(degToRad(i));
        cosTable[i] = cos(degToRad(i));
    }

    player = LPlayer();

    if (createWindow() != 0) finished = true;

    Uint32 pf = SDL_GetWindowPixelFormat(window);
    textures = LTextures(pf, renderer);

    editor = LEditor(textures);

    editor.loadTemplate("./bin/file.bin");
    room = editor.getMap();

    gunTexture = SDL_LoadBMP("./assets/gun1.bmp");

    mode = 0;
}

void LGameManager::Update()
{
    // bobbingAmount = (sin(degToRad(SDL_GetTicks64())) * LSettings::getISetting("pSpeed") * W * 0.5);
    // TODO fix W

    SDL_Event event;
    handleInputs(event);

    player.update();
    Render();
}

void LGameManager::Render()
{

    SDL_SetRenderDrawColor(renderer, 150, 100, 200, 255);
    SDL_RenderClear(renderer);

    if (mode == 0) {

        SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
        SDL_Rect floor = {0, PHEIGHT/2, PWIDTH, PHEIGHT};
        // SDL_RenderFillRect(renderer, &floor);

        drawRays();

        SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, gunTexture);  
        SDL_Rect rect = {WINDOW_PADDING_WIDTH + (PWIDTH/2) - 77, WINDOW_PADDING_HEIGHT + PHEIGHT - 200, 144, 200};      
        SDL_RenderCopy(renderer, texture, NULL, &rect);


    }
    else if (mode == 1) {
        editor.drawMap(renderer);
        drawPlayer();
    }

    SDL_RenderPresent(renderer);
}

void LGameManager::CleanUp()
{
    IMG_Quit();
    SDL_Quit();
}

void LGameManager::handleInputs(SDL_Event event)
{

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
                        case SDLK_e: editor.saveRoom(); break;
                        case SDLK_LSHIFT: player.updateSpeed(40); break;
                        case SDLK_ESCAPE: SDL_DestroyWindow(window); SDL_Quit(); exit(0); break;
                    }
                    break;
                case SDL_KEYUP:
                    switch(event.key.keysym.sym) {
                        case SDLK_w: W = false; break;
                        case SDLK_s: S = false; break;
                        case SDLK_a: A = false; break;
                        case SDLK_d: D = false; break;
                        case SDLK_LSHIFT: player.updateSpeed(20); break;
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

        if(A) {player.rotate(-8); player.updateDeltaX(cosTable[player.getRotation()]); player.updateDeltaY(-sinTable[player.getRotation()]);}
        if(D) {player.rotate(8);  player.updateDeltaX(cosTable[player.getRotation()]); player.updateDeltaY(-sinTable[player.getRotation()]);}
        if(W) {
            if (room[
                getMapPos(player.getPlayerX()+(player.getDeltaX()*player.getSpeed()), 
                          player.getPlayerY()-(player.getDeltaY()*player.getSpeed()))
                ] == 0 ) { player.moveX((player.getDeltaX()) * player.getSpeed()); 
                           player.moveY(-(player.getDeltaY()) * player.getSpeed()); }
        }
        if(S) {
            if (room[
                getMapPos(player.getPlayerX()-(player.getDeltaX()*player.getSpeed()), 
                          player.getPlayerY()+(player.getDeltaY()*player.getSpeed()))
                ] == 0 ) { player.moveX(-(player.getDeltaX()) * player.getSpeed()); 
                           player.moveY((player.getDeltaY()) * player.getSpeed()); }
        }


//         if(A) {pa-=8; fixAngle(pa); pdx=cosTable[pa]; pdy=-sinTable[pa];}
//         if(D) {pa+=8; fixAngle(pa); pdx=cosTable[pa]; pdy=-sinTable[pa];}
//         if(W) {
//             if (room[getMapPos(px+(pdx*pSpeed), py-(pdy*pSpeed))] == 0 ) { px+=(pdx) * pSpeed; py-=(pdy) * pSpeed; }
//         }
//         if(S) {
//             if (room[getMapPos(px-(pdx*pSpeed), py+(pdy*pSpeed))] == 0 ) { px-=(pdx) * pSpeed; py+=(pdy) * pSpeed; }
//         }
}

bool LGameManager::isFinished()
{
    return finished;
}

double LGameManager::getHorizontalDistance(double ra, double &rp, int &mv) {
        
    double Ay = floor(player.getPlayerY()/64.0) * 64.0;
    if (ra >= M_PI) Ay -= 0.1; // Looking down
    else Ay += 64; // Looking up

    double Ax = player.getPlayerX() + (player.getPlayerY()-Ay) / -tan(ra);
    if (Ax > 64*DOF || Ax < 0 || Ay > 64*DOF  || Ay < 0) return 9999.0; // Out of bounds
    
    double dy = 64.0;
    double dx = 64/tan(ra);

    double rx = Ax;
    double ry = Ay;
    if (ra >= M_PI) { dx = -dx; dy = -dy; }

    if (mode == 1) SDL_RenderDrawPoint(renderer, rx, ry);

    // std::cout << "ra: " << ra << std::endl;

    for (int i=0; i<DOF; i++) {

        // std::cout << "Wall at " << rx << ", " << ry << std::endl;

        if (rx > 64*DOF || ry > 64*DOF || rx < 0 || ry < 0) return 999999.0;

        if (mode == 1) SDL_RenderDrawPoint(renderer, rx, ry);

        int pos = ((int)floor(rx/64) % MAPC + (MAPC*((int)floor(ry / 64))));
        if (room[pos] != 0) {
            // std::cout << "Distance: " << sqrt(pow(rx-player.getPlayerX(), 2) + pow(ry-player.getPlayerY(), 2)) << std::endl;
            SDL_Rect rect = {(int)(floor(rx/64) * 64), floor(ry/64) * 64, 64, 64};
            if (mode == 1) SDL_RenderFillRect(renderer, &rect);
            // return abs(ry-py) / sin(ra);
            rp = rx;
            mv = room[pos];
            return sqrt(pow(rx-player.getPlayerX(), 2) + pow(ry-player.getPlayerY(), 2));
        }
        
        rx += dx;
        ry += dy;

    }

    return 999999.0;
}

double LGameManager::getVerticalDistance(double ra, double &rp, int &mv) {
        
    double Ax = floor(player.getPlayerX()/64) * 64;
    if (ra >= M_PI/2 && ra <= (3*M_PI)/2) Ax -= 0.1; // Looking left
    else Ax += 64; // Looking right

    double Ay = player.getPlayerY() + (player.getPlayerX()-Ax) / (-1/tan(ra));
    
    double dx = 64.0;
    double dy = 64/(1/tan(ra));

    double ry = Ay;
    double rx = Ax;
    if (ra >= M_PI/2 && ra <= (3*M_PI)/2) { dx = -dx; dy = -dy; }

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
            return sqrt(pow(rx-player.getPlayerX(), 2) + pow(ry-player.getPlayerY(), 2));
        }

        
        rx += dx;
        ry += dy;
    }

    return 999999.0;
}

void LGameManager::drawRays() {

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    double rad = FOV / PWIDTH;
    double ra = (double)(player.getRotation()) - (FOV/2);
    double rpH, rpV;
    int mvH, mvV;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int r = 0; r<PWIDTH; r += 2) {

        SDL_RenderDrawPoint(renderer, 110 + r, 450);
    }

    for (int r = 0; r<PWIDTH; r++) {
                

        // Walls

        ra += rad;
        fixAngle(ra);

        double height;

        if (ra != M_PI || ra != 0) { 
            double distH = getHorizontalDistance(degToRad(ra), rpH, mvH);
            double distV = getVerticalDistance(degToRad(ra), rpV, mvV);

            double dist = distH;
            double rp = rpH;
            int mapV = mvH;

            double brightnessM = 1.0;
            if (distV < distH) {dist = distV; rp = rpV; mapV = mvV; brightnessM = 0.5;}

            int ca=player.getRotation()-(int)ra;
            fixAngle(ca); 
            dist=dist*cosTable[ca];  

            if (mode == 0) {
                height = 64.0 / dist * player.getProjectionDist();
                if (height > PHEIGHT) height = PHEIGHT;

                rp = (int)rp % 64;
                double tStep = 1.0 * texHeight / height;                                                                 //texture step
                u_int8_t cr,cg,cb;

                int startY = (PHEIGHT/2)-(height/2);

                for (int h=0; h<height;h++) {

                    int texPos = (rp * 64) + (tStep * h); if (texPos > 4095) texPos = 4095; if (texPos < 0) texPos = 0; // Cap the index for the texture just incase
                    double brightness = (1/dist) * 500; if (brightness > 1) brightness = 1; // Fog value here

                    SDL_Color pixelValue = textures.textureToWall(mapV, rp, tStep * h);

                    SDL_SetRenderDrawColor(renderer, pixelValue.r * brightness * brightnessM, pixelValue.g * brightness * brightnessM, pixelValue.b * brightness * brightnessM, 255);
                    SDL_RenderDrawPoint(renderer, WINDOW_PADDING_WIDTH + r, WINDOW_PADDING_HEIGHT + startY+h);
                }
            }

            // Floor

            for (int i = (PHEIGHT/2)+(height/2); i < PHEIGHT; i++) {

                double dr = i - PHEIGHT / 2;
                double dist = player.getHeight() * 277 / dr;
                int ang = player.getRotation()-ra;
                fixAngle(ang);
                double alpha = abs(ang);
                double d = dist / cos(degToRad(alpha));
                int floorX = player.getPlayerX() + (d * cos(degToRad(ra)));
                int floorY = player.getPlayerY() + (d * sin(degToRad(ra)));

                int textureX = (int)floor(floorX) % 64;
                int textureY = (int)floor(floorY) % 64;

                double brightness = (1/dist) * 500; if (brightness > 1) brightness = 1; // Fog value here

                SDL_Color pixelValue = textures.textureToWall(2, textureX, textureY);
                SDL_SetRenderDrawColor(renderer, pixelValue.r * brightness, pixelValue.g * brightness, pixelValue.b * brightness, 255);
                SDL_RenderDrawPoint(renderer, WINDOW_PADDING_WIDTH + r, WINDOW_PADDING_HEIGHT + i);
                pixelValue = textures.textureToWall(6, textureX, textureY);
                SDL_SetRenderDrawColor(renderer, pixelValue.r * brightness, pixelValue.g * brightness, pixelValue.b * brightness, 255);
                SDL_RenderDrawPoint(renderer, WINDOW_PADDING_WIDTH + r, WINDOW_PADDING_HEIGHT + PHEIGHT-i);
            }
        }
    }

    return;
}

void LGameManager::drawPlayer() {

    int screenPx = floor(player.getPlayerX() / 8);
    int screenPy = floor(player.getPlayerY() / 8); // 8 because the LEditor uses 8 pixels for a square

    SDL_Rect rect = { screenPx, screenPy, 5, 5 };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderDrawLine(renderer, screenPx, screenPy, screenPx + 10 * cosTable[player.getRotation()], screenPy + 10 * sinTable[player.getRotation()]);
}

// Path: src/LGameManager.h