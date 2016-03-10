#ifndef BACKGROUND_SUBTRACTOR_H
#define BACKGROUND_SUBTRACTOR_H

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

class BackgrndSubtractor
{
    cv::SimpleBlobDetector::Params params;
    public:
    BackgrndSubtractor();
    
    
    
    void runIndependently();
};

#endif