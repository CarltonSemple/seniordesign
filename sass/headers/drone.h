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

class Drone 
{
    cv::UMat frame;
    std::vector<cv::UMat> templateImages;
    bool displayTemplateMatching;
    int windowHeight;
    int windowWidth;
    int idNum;
        
    // percentage of distance from the window's sides that is acceptable
    // used to decide if the target is heading out of the frame
    double positionAcceptancePercentage; 
    
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