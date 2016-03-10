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

test::test()
{
	cout << "hello world" << endl;
}

int findAndSaveHumans()
{
    PersonDetector detector;
    
    VideoCapture cap(1);//CV_CAP_ANY);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);    
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
        vector<Human> humans = detector.detectHumans(img);
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
        if(cumulativeHumans.size() > 0) {
            Human::saveHumanImagesToFiles(humans, "t");
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

int main(int argc, char *argv[]) 
{
	test t;
    //scanner s;
    //s.run();
      
    //findAndSaveHumans();
    
    // Background Subtraction test
    backgroundSubtractionTest();
    
	return 0;
}
