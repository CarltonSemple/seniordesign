#ifndef DRONE_H
#define DRONE_H

#include <functional>   // std::ref
#include <iostream>
#include <vector>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "communicationBox.h"
#include "objectdetector.h"
#include "matcher.h"

/**
** Class containing the drone decision logic and higher level movement functions.
** It accepts the communication box struct as a reference during construction so that
** it can access the latest drone image frame and send commands to the Send Command server
** via the communication box's droneMovement and droneMovementValue variables.
*/
class Drone 
{
    cv::UMat frame;
    
    /** Collection of images representing the target that the drone must track. This collection
    ** is what the drone uses when running detection algorithms from the matcher class. 
    */
    std::vector<cv::UMat> templateImages;
    bool displayTemplateMatching;
    int windowHeight;
    int windowWidth;
    
    /** @deprecated. The communication box's desired_drone variable should now be used
    to communicate which drone needs to be controlled. **/
    int idNum;
        
    // percentage of distance from the window's sides that is acceptable
    // used to decide if the target is heading out of the frame
    
    /** Percentage of the total drone frame window width from the window's sides
    ** that is used to say if the target is heading to the left or right of the window.
    ** If the target is within this percentage (positionAcceptancePercentage * windowWith) 
    ** from the left edge of the window or the right edge, it is concluded that the drone should
    ** turn slightly to make sure the target is in the center of the image again.
    **/
    double positionAcceptancePercentage; 
    
    /** The local reference to the global communication box struct. The drone can get a copy
    ** of the latest drone vision Mat frame from this struct, and send commands to the Send Commands
    ** server via this
    **/
    CommunicationBox & commBox;
    
public:
    
    Drone(int ID, CommunicationBox & communicationBox);
    void turnLeft(int angle);
    void turnRight(int angle);
    void moveForward(int dist);
    void moveBackward(int dist);
    void decide(cv::UMat & currentFrame);
    void setID(int idnum);
    void setTemplateImages(Human & targetHuman);
    void setDisplayTemplateMatching(bool display);
    

private:
    void reactToTargetPosition(int x, int y);
    cv::Point getTemplateTargetPositionAverage();
    cv::Point getHogTargetPositionAverage();
};

#endif