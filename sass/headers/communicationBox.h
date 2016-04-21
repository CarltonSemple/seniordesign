#ifndef COMMBOX_H
#define COMMBOX_H

#include <string>
#include "opencv2/core.hpp"

struct CommunicationBox
{
    cv::Mat skyFrame;
    cv::Mat lowFrame;
    cv::Mat droneFrame;
    
    char droneMovement;
    int droneMovementValue;
    bool okayToDecide; // Wait to write to the drone2Command string
};

#endif