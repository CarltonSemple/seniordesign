#ifndef EYEINSKY_H
#define EYEINSKY_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/ocl.hpp> // opencl
#include "communicationBox.h"
#include "util.h"

using namespace cv;
class eyeinsky
{
	int startPixel;
	int newPixel;
	double startWidth;
	double startDist;
	double newDist;
	double focalLength;
public:
	eyeinsky(int method, CommunicationBox & commBox);

    
    double getDronePixelWidth(UMat & img);

	Position colorBlobDistanceCalibration(int iLowH, int iHighH, int iLowS, int iHighS, int iLowV, int iHighV, Mat imgOriginal,int droneId, CommunicationBox & commBox); 
    
    // get pixel width of drones
    Position getDronePosition(int droneId, Mat & img, CommunicationBox & commBox);
    
    void drawDrone(Position dronePosition, Mat & img);
};

#endif
