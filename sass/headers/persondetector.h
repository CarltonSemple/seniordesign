#ifndef PERSONDETECTOR_H
#define PERSONDETECTOR_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "human.h"

class PersonDetector
{
    int runIndependently(); // run independently 
    
    public:
    PersonDetector();
            
    // Create humans from the locations of detected humans, 
    // and optionally draw generic black boxes around the humans
    std::vector<Human> detectHumans(cv::Mat img, bool drawBoxes);
    
    /* Match new humans with the current stored humans */
    void keepSimilarHumans(std::vector<Human> & collection, std::vector<Human> & newHumans);
};

#endif