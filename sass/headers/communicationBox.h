#ifndef COMMBOX_H
#define COMMBOX_H

#include <string>
#include "opencv2/core.hpp"
#include "util.h"

/**
** Struct containing global variables that are needed by several threads in sass.cpp and classes.
** Passing a reference to the communication box allows objects to access these public variables
** in a clean manner.
**/
struct CommunicationBox
{
    cv::Mat skyFrame;
    cv::Mat lowFrame;
    
    /* Latest video frame from the drone as updated by the receiveFrames thread */
    cv::Mat droneFrame;
    
    /* Character representing the type of movement to be sent to the drone.
    ** wasd format;
    ** w = move forward
    ** a = turn left
    ** d = turn right
    ** s = move backward
    */
    char droneMovement;
    
    /*
    ** The value representing the intensity/duration of the chosen command to be sent to the drone.
    ** If the command is forward or backward, this represents distance.
    ** If the command is turn left or turn right, this represents the relative amount to turn.
    */
    int droneMovementValue;
    
    /*
    ** Variable that stops the drone objects from trying to send too many commands
    ** in reaction to the same video frame. Once the drone objects send a command,
    ** this variable is set to false.  It is set to true once the command has been sent,
    ** and only then can the drone objects decide to send more commands.
    */
    bool okayToDecide; // Wait to write to the drone2Command string
    
    /**
    * Used to have a synchronized change from one drone to the other. The receiveFrames thread
    * and the sendCommands thread check the value of this variable at the beginning of their loops,
    * before the portions of code wherein a switch of the variable would be dangerous.
    */ 
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
