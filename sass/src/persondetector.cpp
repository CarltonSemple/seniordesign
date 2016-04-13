#include <iostream>
#include "opencv2/highgui.hpp"
#include <opencv2/core/utility.hpp>
#include <opencv2/core/ocl.hpp>
#include "persondetector.h"
 
using namespace std;
using namespace cv;

PersonDetector::PersonDetector() 
{
    
}

std::vector<Human> PersonDetector::detectHumans(cv::UMat sourceImg, bool drawBoxes) 
{
    vector<Human> humans;
    
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
       
    UMat scaledDown;
    Size sSize(320, 240);
    resize(sourceImg, scaledDown, sSize);
    
    double scale = sourceImg.size().height / scaledDown.size().height;
    //cout << "scale: " << scale << endl;
   
    int win_width = 24;//48;
    int win_stride_width = 8;
    int win_stride_height = 8;
    int gr_threshold = 8;
    int nlevels = 13;
    double hit_threshold = 1.4;
    double scaleHog = 1.05;
    bool gamma_corr = true;
    Size win_size(win_width, win_width * 2);
    Size win_stride(win_stride_width, win_stride_height);
    //HOGDescriptor hog(win_size, Size(16, 16), Size(8, 8), Size(8, 8), 9, 1, -1, HOGDescriptor::L2Hys, 0.2, gamma_corr, cv::HOGDescriptor::DEFAULT_NLEVELS);
 
    /*
    if  ( ! cv::ocl::haveOpenCL ())
    {
        cout  <<  "OpenCL IS not avaiable ..."  <<  endl ;
            //return 0;
    } else {
        cout << "openCL is available :) " << endl;
    }*/
    
    
    //cv::ocl::attachContext();
    // gpu version
    //ocl::HOGDescriptor gpu_hog;
 
    //namedWindow("video capture", CV_WINDOW_NORMAL);

    // make a copy of the image
    //UMat img = sourceImg;
    // jkkkkk
    //sourceImg.copyTo(img);
    
    //if (!img.data)
    //    return humans;

    vector<Rect> found;  //, found_filtered;
    hog.detectMultiScale(scaledDown, found, 0, Size(8,8), Size(32,32), 1.05, 2);
    //hog.detectMultiScale(scaledDown, found, 0, Size(8,8), Size(0,0), 1.05, 2);
    //hog.detectMultiScale(scaledDown, found, hit_threshold, win_stride, Size(0, 0), scaleHog, gr_threshold); // after win_stride is padding

    if(found.size() > 0){
        //cout << found.size() << " humans found" << endl;
    }
    
    size_t i, j;
    for (i=0; i<found.size(); i++)
    {
        Rect r = found[i];
        for (j=0; j < found.size(); j++)
            if (j != i && (r & found[j]) == r)
                break;
        if (j == found.size())
        {
            // Make human
            r.x += cvRound(r.width*0.1);
            r.x *= 2;
            r.width = cvRound(r.width*0.8);
            r.width *= 2;
            r.y += cvRound(r.height*0.06);
            r.y *= 2;
            r.height = cvRound(r.height*0.9);
            r.height *= 2;
            if(drawBoxes) {
                rectangle(sourceImg, r.tl(), r.br(), cv::Scalar(0,255,0), 3);//cv::Scalar(0,0,0), 2);
            }
            pair<int,int> topleft = make_pair(r.tl().x, r.tl().y);
            pair<int,int> bottomRight = make_pair(r.br().x, r.br().y);
            UMat humImg(sourceImg, r);
            Human nh(std::to_string(Human::idNumber), topleft, bottomRight, humImg);
            Human::idNumber++;
            humans.push_back(nh);
        }
    }
    /*
    for (i = 0; i < found_filtered.size(); i++)
    {
        Rect r = found_filtered[i];
        r.x += cvRound(r.width*0.1);
        r.width = cvRound(r.width*0.8);
        r.y += cvRound(r.height*0.06);
        r.height = cvRound(r.height*0.9);
        // show rectangle
        //rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 2); 
    }*/
    //imshow("video capture", img);
        
    //if (waitKey(20) >= 0)
    //    break;
    
    return humans;
}

void PersonDetector::keepSimilarHumans(std::vector<Human> & collection, std::vector<Human> & newHumans)
{
    
}

int PersonDetector::runIndependently()
{
    VideoCapture cap(1);//CV_CAP_ANY);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);    
    if (!cap.isOpened())
        return -1;
 
    Mat img;
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
 
    namedWindow("video capture", CV_WINDOW_NORMAL);
    while (true)
    {
        cap >> img;
        if (!img.data)
            continue;
 
        vector<Rect> found, found_filtered;
        hog.detectMultiScale(img, found, 0, Size(8,8), Size(32,32), 1.05, 2);
 
        size_t i, j;
        for (i=0; i<found.size(); i++)
        {
            Rect r = found[i];
            for (j=0; j<found.size(); j++)
                if (j!=i && (r & found[j])==r)
                    break;
            if (j==found.size())
            {
                //found_filtered.push_back(r);
                //Rect r = found_filtered[i];
                r.x += cvRound(r.width*0.1);
                r.width = cvRound(r.width*0.8);
                r.y += cvRound(r.height*0.06);
                r.height = cvRound(r.height*0.9);
                rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 2);
            }
        }
        /* // can be placed into the above loop
        for (i=0; i<found_filtered.size(); i++)
        {
            Rect r = found_filtered[i];
            r.x += cvRound(r.width*0.1);
            r.width = cvRound(r.width*0.8);
            r.y += cvRound(r.height*0.06);
            r.height = cvRound(r.height*0.9);
            rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 2);
        }*/
        imshow("video capture", img);
        if (waitKey(20) >= 0)
            break;
    }
    return 0;
}