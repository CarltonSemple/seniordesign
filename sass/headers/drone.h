#ifndef DRONE_H
#define DRONE_H

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>

class Drone {
    cv::UMat frame;
    int windowHeight;
    int windowWidth;
    // percentage of distance from the window's sides that is acceptable
    // used to decide if the target is heading out of the frame
    double positionAcceptancePercentage; 
    
public:
    
    Drone();
    void turnLeft(int16_t angle);
    void turnRight(int16_t angle);
    
    void decide();

private:
    void reactToTargetPosition();
};

#endif