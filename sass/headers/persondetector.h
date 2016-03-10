#ifndef PERSONDETECTOR_H
#define PERSONDETECTOR_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "human.h"

class PersonDetector
{
    int run(); // run independently 
    
    public:
    PersonDetector();
            
    std::vector<Human> detectHumans(cv::Mat img);
    
    /* Match new humans with the current stored humans */
    void keepSimilarHumans(std::vector<Human> & collection, std::vector<Human> & newHumans);
};

#endif