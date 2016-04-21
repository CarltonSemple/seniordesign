#include "drone.h"
#include <string>

using namespace std;
using namespace cv;

Drone::Drone(int ID, CommunicationBox & communicationBox)
: commBox(communicationBox) 
{
    windowWidth = 640;
    windowHeight = 480;
    positionAcceptancePercentage = 0.47;
    
}

void Drone::turnLeft(int angle)
{
    commBox.droneMovement = 'a';
    commBox.droneMovementValue = angle;
    commBox.okayToDecide = false;
}

void Drone::turnRight(int angle)
{
    commBox.droneMovement = 'd';
    commBox.droneMovementValue = angle;
    commBox.okayToDecide = false;
}
void Drone::moveForward(int dist)
{
    commBox.droneMovement = 'w';
    commBox.droneMovementValue = dist;
    commBox.okayToDecide = false;
}

void Drone::moveBackward(int dist)
{
    commBox.droneMovement = 's';
    commBox.droneMovementValue = dist;
    commBox.okayToDecide = false;
}



void Drone::decide(UMat & currentFrame)
{
    //commBox.droneFrame.copyTo(frame);
    frame = currentFrame;
    
    // Get the average target position, in terms of x & y for now
    Point position = getHogTargetPositionAverage();//getTemplateTargetPositionAverage();
    if(position.x == 0 && position.y == 0) {
        return;
    }
    reactToTargetPosition(position.x, position.y);  
    
    // show images after processing them
    //if(display) {
        //imshow("Active Drone Feed", frame);
    //}  
}

Point Drone::getHogTargetPositionAverage()
{
    ObjectDetector og;
    auto humanVector = og.detectHumans(frame, true);
    
    Point ret(0,0);
       
    // Assuming there is only one human
    if(humanVector.size() > 0) {
        // look at x position of first human
        Human & human = humanVector[0];
        double xCenter = human.getCenterX();
        ret.x = xCenter;
        cout << "center of person: " << ret.x << endl;
        ret.y = human.getBottomRightPoint().y;
        moveForward(1);
    }
    else {
        moveBackward(2);
    }
    return ret;
}

Point Drone::getTemplateTargetPositionAverage()
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
    double centerLeft, centerRight, centerBottom, centerTop;
    centerLeft = windowWidth * positionAcceptancePercentage;
    centerRight = windowWidth - (windowWidth * positionAcceptancePercentage);
    centerBottom = windowHeight * 0.5;
    centerTop = windowHeight * 0.6;
    
    int incAmount = 1;
    
    // target is left of center range
    if(x < centerLeft) 
    {
        turnLeft(incAmount);
    }
    else if (x > centerRight) 
    {
        turnRight(incAmount);
    }
    
   /* if(y < centerBottom) {
        moveBackward(1);
    } 
    else if(y > centerTop) 
    {
        moveForward(1);
    }*/
}