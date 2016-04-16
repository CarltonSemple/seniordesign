#include "drone.h"
#include <string>

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
    //type like w 3 if you want www
    switch(idNum)
    {
    case 1: 
        commBox.drone1Command = "a " + to_string(angle);
        break;
    case 2: 
        commBox.drone2Command = "a "+ to_string(angle);
        break;
    }
}

void Drone::turnRight(int angle)
{
    switch(idNum)
    {
    case 1: 
        commBox.drone1Command = "d "+ to_string(angle);
        break;
    case 2: 
        commBox.drone2Command = "d "+ to_string(angle);
        break;
    }
}
void Drone::moveForward(int dist)
{
    //type like w 3 if you want www
    switch(idNum)
    {
    case 1: 
        commBox.drone1Command = "w " + to_string(dist);
        break;
    case 2: 
        commBox.drone2Command = "w "+ to_string(dist);
        break;
    }
}

void Drone::moveBackward(int dist)
{
    switch(idNum)
    {
    case 1: 
        commBox.drone1Command = "s "+ to_string(dist);
        break;
    case 2: 
        commBox.drone2Command = "s "+ to_string(dist);
        break;
    }
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