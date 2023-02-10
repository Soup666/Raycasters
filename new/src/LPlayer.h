#ifndef LPlayer_h
#define LPlayer_h

#include "LSettings.h"

class LPlayer
{
    public:
        LPlayer();
        void update();
        double getPlayerX() { return px; }
        double getPlayerY() { return py; }
        void setPlayerX(double x) { px = x; }
        void setPlayerY(double y) { py = y; }
        void moveX(double x) { px += x; }
        void moveY(double y) { py += y; }
        void updateSpeed(double speed) { pSpeed = speed; }
        double getSpeed() { return pSpeed; }
        void rotate(int angle) { pa += angle; fixAngle(pa); }
        void updateDeltaX(double dx) { pdx = dx; }
        void updateDeltaY(double dy) { pdy = dy; }
        double getDeltaX() { return pdx; }
        double getDeltaY() { return pdy; }
        double getProjectionDist() { return projectionDist; }
        int getRotation() { return pa; }
        int getHeight() { return playerHeight; }
    private:
        int playerHeight, pa;
        double px, py, projectionDist, pdx, pdy, pSpeed;
};

#endif