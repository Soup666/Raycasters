/*
    Defines the update loop for the player, ran every frame.
*/

#include "LPlayer.h"

LPlayer::LPlayer()
{
    px = 4*64 - 32;
    py = 4*64 - 32;
    pa = 300;
    pdx = 0;
    pdy = 0;
    pSpeed = 20;
    playerHeight = 32;

}

void LPlayer::update()
{
    projectionDist = (LSettings::getISetting("PWIDTH")/2) / tan(degToRad(LSettings::getDSetting("FOV")/2));
}