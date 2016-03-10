#include <iostream>
#include "persondetector.h"
 
using namespace std;
using namespace cv;

PersonDetector::PersonDetector() 
{
    
}

std::vector<Human> PersonDetector::detectHumans(Mat img) 
{
    vector<Human> humans;
    
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
 
    //namedWindow("video capture", CV_WINDOW_NORMAL);

    if (!img.data)
        return humans;

    vector<Rect> found;  //, found_filtered;
    hog.detectMultiScale(img, found, 0, Size(8,8), Size(32,32), 1.05, 2);

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
            r.width = cvRound(r.width*0.8);
            r.y += cvRound(r.height*0.06);
            r.height = cvRound(r.height*0.9);
            rectangle(img, r.tl(), r.br(), cv::Scalar(0,255,0), 2);
            pair<int,int> topleft = make_pair(r.tl().x, r.tl().y);
            pair<int,int> bottomRight = make_pair(r.br().x, r.br().y);
            Mat humImg(img, r);
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

int PersonDetector::run()
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