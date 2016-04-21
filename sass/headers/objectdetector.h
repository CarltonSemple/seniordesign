#ifndef OBJECTDETECTOR_H
#define OBJECTDETECTOR_H

#include <vector>
#include "drone.h"
#include "human.h"
#include "util.h"
#include "opencv2/core.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>

#include <tuple>

class ObjectDetector
{
    int runIndependently(); // run independently 
    
    public:
    ObjectDetector();
            
    // Create humans from the locations of detected humans, 
    // and optionally draw generic black boxes around the humans
    std::vector<Human> detectHumans(cv::UMat sourceImg, bool drawBoxes, bool drawLine);
    
    /* Match new humans with the current stored humans */
    void keepSimilarHumans(std::vector<Human> & collection, std::vector<Human> & newHumans);
    
    void blobDetect(cv::UMat & img);
    
    /** Returns the start width and the end width of a color filtered object.
        - pair.first = start width, the width at the lower end of the picture.
        - pair.second = end width, the width at the higher end of the picture.
        The end width naturally should be smaller, as the ending side of the object
        (if the object is flat) will be further away from the camera
    **/
    std::pair<double, double> colorBlobDistanceCalibration(int videoCameraNumber);
};

#endif