#include "backgroundSubtractor.h"
#include "util.h"

using namespace cv;
using namespace std;

BackgrndSubtractor::BackgrndSubtractor()
{
    // Setup SimpleBlobDetector parameters
    params.blobColor = 255;
	params.minThreshold = 0;
	params.maxThreshold = 255;
	params.filterByArea = true;
	params.minArea = 100;
	params.filterByCircularity = false;
	//params.minCircularity = 0.1;
	params.filterByConvexity = true;
	params.minConvexity = 0.0;
	params.filterByInertia = true;
	params.minInertiaRatio = 0.00;
	params.maxInertiaRatio = 0.7;
}

void BackgrndSubtractor::runIndependently()
{
    bool end = false;
    VideoCapture cap(1);//CV_CAP_ANY);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);    
    if (!cap.isOpened())
        return;
        
    Mat rgbMat;    
    Ptr<BackgroundSubtractor> pMOG2; //MOG2 Background subtractor
	Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
    pMOG2 = createBackgroundSubtractorMOG2(); //MOG2 approach
    
    namedWindow("rgb",CV_WINDOW_AUTOSIZE);
    namedWindow("background subtraction", CV_WINDOW_AUTOSIZE);
        
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
    vector<KeyPoint> keypoints;
    
    while(!end)
    {
        cap >> rgbMat;
        if (!rgbMat.data)
            continue;
            
        // Apply background subtraction
		pMOG2->apply(rgbMat, fgMaskMOG2);
        
        // detect blobs
        std::vector<KeyPoint> keypoints;
        detector->detect( fgMaskMOG2, keypoints);

        Mat blob_image;
		drawKeypoints(fgMaskMOG2, keypoints, blob_image, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

		cv::imshow("background subtraction", blob_image); //fgMaskMOG2);
		cv::imshow("rgb", rgbMat);
        
        char k = cvWaitKey(5);
		if( k == 27 ){
			cvDestroyWindow("rgb");
			cvDestroyWindow("background subtraction");
            
            for(KeyPoint key : keypoints)
            {
                cout << "x: " << key.pt.x << " y: " << key.pt.y << " size: " << key.size << endl;
            }
			break;
		}
    }
}