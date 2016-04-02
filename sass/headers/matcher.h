#ifndef MATCHER_H
#define MATCHER_H

#include <iostream>
#include <vector>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "human.h"
#include "robustMatcher.h"

class Matcher 
{
    float ratio;
    public:
    
    Matcher();
    Matcher(float AccuracyRatio);
    
    // RobustMatcher & SURF:
    // Returns the average number of matches between the array of
    // scanned target images and the potential target's images
    //
    //
    //
    //      IMAGES POSSIBLY SHOULD BE ADDED TO THE HUMAN TO MAKE THE 
    //      GUESSING MORE ACCURATE OVER TIME
    //
    //
    int surfCount(Human & scannedHuman, cv::Mat & potentialImage);
    
    
};

#endif