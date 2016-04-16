#include "drone.h"

using namespace std;
using namespace cv;

Drone::Drone(int ID, CommunicationBox & communicationBox)
: commBox(communicationBox) 
{
    windowWidth = 640;
    windowHeight = 480;
    positionAcceptancePercentage = 0.3;
    
}

void Drone::turnLeft(int angle)
{
    
}

void Drone::turnRight(int angle)
{
    
}

void Drone::decide()
{
    switch(idNum)
    {
    case 1: commBox.drone1Frame.copyTo(frame);
        break;
    case 2: commBox.drone2Frame.copyTo(frame);
        break;
    }
    
    // Get the average target position, in terms of x & y for now
    Point position = getTargetPositionAverage();
        
    reactToTargetPosition(position.x, position.y);    
}

Point Drone::getTargetPositionAverage()
{
    double xSum = 0, ySum = 0;
    
    Matcher matcher;
    for(UMat temImg : templateImages)
    {
        Point p = matcher.templateMatchingWithoutCallBack(std::ref(temImg), std::ref(frame));
        xSum += p.x;
        ySum += p.y;
    }
    
    Point rpoint(xSum/templateImages.size(), ySum/templateImages.size());
    return rpoint;
}

// turn in small increments, or move forward until the target is in the center range
void Drone::reactToTargetPosition(int x, int y)
{
    double centerLeft, centerRight;
    centerLeft = windowWidth * positionAcceptancePercentage;
    centerRight = (windowWidth * positionAcceptancePercentage) - windowWidth;
    
    int incAmount = 1;
    
    // target is left of center range
    if(x < centerLeft) 
    {
        turnRight(incAmount);
    }
    else if (x > centerRight) 
    {
        turnLeft(incAmount);
    }
}