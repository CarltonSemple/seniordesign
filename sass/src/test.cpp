#include <string>
#include <vector>
#include "test.h"
#include "backgroundSubtractor.h"
#include "human.h"
#include "persondetector.h"
#include "scanner.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

#define CAMERA_WIDTH 320
#define CAMERA_HEIGHT 240

int cameraNumber = 0;

test::test()
{
	cout << "hello world" << endl;
}

void printHelp()
{
    cout << "enter the camera number when running" << endl;
}

// this is a test of cloud 9

int findAndSaveHumans(bool save, bool drawBoxes)
{
    PersonDetector detector;
    
    VideoCapture cap(cameraNumber);//CV_CAP_ANY);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT);    
    if (!cap.isOpened())
        return -1;
    
    vector<Human> cumulativeHumans;
    int size = cumulativeHumans.size();
    
    namedWindow("video capture", CV_WINDOW_NORMAL);
    Mat img;
    while (true)
    {
        cap >> img;
        if (!img.data)
            continue;
        // look for humans
        vector<Human> humans = detector.detectHumans(img, drawBoxes);
        cumulativeHumans.reserve(cumulativeHumans.size() + humans.size()); 
        cumulativeHumans.insert(cumulativeHumans.end(),humans.begin(),humans.end());
        if(cumulativeHumans.size() > size) {
            size = cumulativeHumans.size();
            cout << cumulativeHumans.size() << " humans so far" << endl;
        }        
        
        imshow("video capture", img);        
        char key = waitKey(20);
        /*
        if(key == 's') 
        {
            if(humans.size() > 0) {
                Human::saveHumanImagesToFiles(humans, "t");
                break;
            }
        }*/
        
        // Save the humans to files if this is desired
        if(save)
        {
            if(cumulativeHumans.size() > 0) {
                Human::saveHumanImagesToFiles(humans, "t");
            }
        }
    }
    
    return 0;
}

int Human::idNumber = 0;

int backgroundSubtractionTest()
{
    BackgrndSubtractor bs;
    bs.runIndependently();
    return 0;
}

/* Run video and use the person detector (HOG) to show humans and
   print how many surf similarities each "human" has to the scanned person */
void countSimilaritiesToScannedTarget()
{
    PersonDetector detector;
    VideoCapture cap(cameraNumber);//CV_CAP_ANY);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHTgc);    
    if (!cap.isOpened())
        return -1;
    
    Mat img;
    while(true)
    {
        cap >> img;
        if (!img.data)
            continue;
        
        vector<Human> detectedHumans = detector.detectHumans(img, false);
        // look at the humans and see the similarities
        for(Human hu : detectedHumans)
        {
            Matcher checker;
            std::vector<cv::Mat> & hImages = hu.getImages();
            for(int p = 0; p < hu.getImageCount(); p++) 
            {
                checker.surfCount(target, hImages[p]);
            }
        }
    }
}

int main(int argc, char *argv[]) 
{
    if(argc == 2) {
        cameraNumber = argv[1];
    } else {
        printHelp();
        return -1;
    }
    
    //scanner s;
    //s.run();
      
    //findAndSaveHumans(true, false);
    
    // Background Subtraction test
    //backgroundSubtractionTest();
    
	return 0;
}
