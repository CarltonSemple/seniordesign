#include "drone.h"

Drone::Drone()
{
    windowWidth = 640;
    windowHeight = 480;
    positionAcceptancePercentage = 0.3;
}






// turn in small increments, or move forward until the target is in the center range
void Drone::reactToTargetPosition(int x, int y)
{
    double centerLeft, centerRight;
    centerLeft = windowWidth * positionAcceptancePercentage;
    centerRight = (windowWidth * positionAcceptancePercentage) - windowWidth;
    // target is left of center range
    if(x < centerLeft) 
    {
        turnRight(1);
    }
    else if (x > centerRight) 
    {
        turnLeft(1);
    }
}