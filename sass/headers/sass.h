#ifndef SASS_H
#define SASS_H

#include <iostream>
#include <string>
#include <vector>
#include "test.h"
#include "backgroundSubtractor.h"
#include "communicationBox.h"
#include "drone.h"
#include "human.h"
#include "persondetector.h"
#include "matcher.h"
#include "scanner.h"
#include "util.h"
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/ocl.hpp> // opencl
#include <cstdlib>
#include <unistd.h>
#include <thread>

// following is specifically for server code
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>

class Sass 
{
public:
    //CommunicationBox commBox;
    //Drone a; // drone that corresponds to #1
    //Drone b; // drone that corresponds to #2
    
    Sass();    
    Sass(int matchingMethod);
    
    void runSystem();
    
    
    //int determineNeedToSendCommands();
    
    
};

#endif