#ifndef COMMBOX_H
#define COMMBOX_H

#include <string>
#include "opencv2/core.hpp"

struct CommunicationBox
{
    cv::Mat skyFrame;
    cv::Mat lowFrame;
    cv::Mat drone1Frame;
    cv::Mat drone2Frame;
    
    // 
    std::string drone1Command;
    bool drone1Wait; // Wait to write to the drone1Command string
    
    // 
    std::string drone2Command;
    bool drone2Wait; // Wait to write to the drone2Command string
};

#endif