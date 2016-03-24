#include <string>
#include <vector>
#include "test.h"
#include "backgroundSubtractor.h"
#include "human.h"
#include "persondetector.h"
#include "matcher.h"
#include "scanner.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <cstdlib>

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
void countSimilaritiesToScannedTarget(Human & target)
{
    PersonDetector detector;
    VideoCapture cap(cameraNumber);//CV_CAP_ANY);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT);    
    if (!cap.isOpened())
        return;
    
    Mat img;
    while(true)
    {
        cap >> img;
        if (!img.data)
            continue;
            
        namedWindow("video capture", CV_WINDOW_NORMAL);
                
        vector<Human> detectedHumans = detector.detectHumans(img, false);
        // look at the humans and see the similarities
        
        for(Human hu : detectedHumans)
        {
            Matcher matcher;
            std::vector<cv::Mat> & hImages = hu.getImages();
            int a = 0;
            for(int p = 0; p < hu.getImageCount(); p++) 
            {
                //cout << "p = " << p << endl;
                a += matcher.surfCount(target, hImages[p]);
            }
            //a = a / hu.getImageCount();
            cout << "a = " << a << endl;
        }
        
        imshow("video capture", img);
        /*
        // display loaded images
        vector<cv::Mat> & selfies = target.getImages();
        //cout << selfies.size() << " selfies" << endl;
        for(int i = 0; i < selfies.size(); i++)
        {
            //cout << "image: " << i << endl;
            imshow("video capture", selfies[i]);//img);
            waitKey(10);
        }*/
        char key = waitKey(1);
    }
}

void printInitialMenu()
{
    cout << "s: scan new human" << endl;
    cout << "l: load previous scan" << endl;
}

int main(int argc, char *argv[]) 
{
    if(argc == 2) {
        cameraNumber = atoi(argv[1]);
    } else {
        printHelp();
        return -1;
    }
    
    Scanner s;
    //s.runIndependently();
      
    //findAndSaveHumans(true, false);
    
    // Background Subtraction test
    //backgroundSubtractionTest();
    
    Human scannedHuman = s.loadScannedHuman(2);
    cout << "loaded " << scannedHuman.getImageCount() << " pictures" << endl;
    countSimilaritiesToScannedTarget(scannedHuman);
    
    
	return 0;
}
