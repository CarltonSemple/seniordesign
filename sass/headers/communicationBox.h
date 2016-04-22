#ifndef COMMBOX_H
#define COMMBOX_H

#include <string>
#include "opencv2/core.hpp"
#include "util.h"

struct CommunicationBox
{
    cv::Mat skyFrame;
    cv::Mat lowFrame;
    cv::Mat droneFrame;
    
    char droneMovement;
    int droneMovementValue;
    bool okayToDecide; // Wait to write to the drone2Command string
    int desired_drone = 1;

    //Point of barrier Mark
    Position bMark;

    //Point of Drone 1
    Position d1Mark;

    //Point of Drone 2
    Position d2Mark;

    //Point of Human Target
    Position hMark;
};

#endif
