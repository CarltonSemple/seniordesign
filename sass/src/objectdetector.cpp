#include <iostream>
#include "opencv2/highgui.hpp"
#include <opencv2/core/utility.hpp>
#include <opencv2/core/ocl.hpp>
#include "objectdetector.h"
#include "communicationBox.h"
using namespace std;
using namespace cv;

ObjectDetector::ObjectDetector() 
{
    
}

std::vector<Human> ObjectDetector::detectHumans(cv::UMat sourceImg, bool drawBoxes, bool drawLine, CommunicationBox & commBox) 
{
    vector<Human> humans;
    
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
       
    cv::UMat scaledDown;
    Size sSize(320, 240);
    resize(sourceImg, scaledDown, sSize);
    
    double hscale = sourceImg.size().height / scaledDown.size().height;
    double wscale = sourceImg.size().height / scaledDown.size().height;
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
            r.x *= wscale;
            r.width = cvRound(r.width*0.8);
            r.width *= wscale;
            r.y += cvRound(r.height*0.06);
            r.y *= hscale;
            
            r.height = cvRound(r.height*0.9);
            r.height *= hscale;
            if(drawBoxes) {
                rectangle(sourceImg, r.tl(), r.br(), cv::Scalar(0,255,0), 3);//cv::Scalar(0,0,0), 2);
            }
            if(drawLine){
                //update location of human
                commBox.hMark.x2d = r.br().x - (r.width/2);
                commBox.hMark.y2d = r.br().y;
                //Draw line at humans feet
                line( sourceImg, Point(0,r.br().y), Point(1920,r.br().y), Scalar(0,0,255), 2, 8, 0 );//draw red line at bottom of rectangle
            
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

void ObjectDetector::keepSimilarHumans(std::vector<Human> & collection, std::vector<Human> & newHumans)
{
    
}

int ObjectDetector::runIndependently()
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

void ObjectDetector::blobDetect(cv::UMat & img)
{
    cv::SimpleBlobDetector::Params params;
    
    // Setup SimpleBlobDetector parameters
    //params.blobColor = 255;
	params.minThreshold = 0;
	params.maxThreshold = 255;
	params.filterByArea = true;
	params.minArea = 0;
	params.filterByCircularity = false;
	//params.minCircularity = 0.1;
	params.filterByConvexity = false;
	params.minConvexity = 0.5;
	params.filterByInertia = true;
	params.minInertiaRatio = 0.0;
	params.maxInertiaRatio = 1.0;
    params.minDistBetweenBlobs = 000;
    
    // detect blobs
    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
    std::vector<KeyPoint> keypoints;
    detector->detect(img, keypoints);
    
    drawKeypoints(img, keypoints, img, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

}

// Finds the number of pixels of a certain color.
// make sure that the threshholded image is clean, with only one blob of white
std::pair<double, double> ObjectDetector::colorBlobDistanceCalibration(int videoCameraNumber)
{
    std::pair<double, double> widthPair;
    VideoCapture cap(videoCameraNumber); //capture the video from web cam
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720); 

    if ( !cap.isOpened() )  // if not success, exit program
    {
        cout << "Cannot open the web cam" << endl;
        widthPair.first = -1.0;
        widthPair.second = -1.0;
        return widthPair;
    }
    
    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

    int iLowH = 0;
    int iHighH = 179;

    int iLowS = 0; 
    int iHighS = 255;

    int iLowV = 0;
    int iHighV = 255;

    //Create trackbars in "Control" window
    cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Control", &iHighH, 179);

    cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &iHighS, 255);

    cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &iHighV, 255);
    
    cout << "Press the space bar to calculate vertical px distance of single object" << endl;
    cout << "Press esc to finalize" << endl;
    long double avgWidth = 0.0;
    long double widest = 0.0;
    long double avgDist = 0;
    iLowH = 38;
    iHighH = 83;
    iLowS = 50;
    iLowV = 35;
    double dist;
    
    while (true)
    {
        widest = 0.0;
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video

        if (!bSuccess) //if not success, break loop
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        Mat imgHSV;

        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

        Mat imgThresholded;

        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

        //morphological opening (remove small objects from the foreground)
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

        //morphological closing (fill small holes in the foreground)
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        UMat tempThreshold;
        //tempThreshold = imgThresholded.getUMat(ACCESS_RW);
        //blobDetect(tempThreshold);
        imshow("Thresholded Image", imgThresholded); //show the thresholded image
        imshow("Original", imgOriginal); //show the original image

        int key = waitKey(30);
        if (key == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            break; 
        }
        //else if (key == 32) // space key is pressed 
        //{
            // calculate
            
            // iterate through the image, row by row from the bottom
            // first row with more than 5 white-ish pixels contains the object
            // last row with more than 5 white-ish pixels contains the end of object...
            int startRow = 0, endRow = 0; 
            double startRowWidth = 0.0, endRowWidth = 0.0;
            avgWidth = 0.0;
            int rowCount = 0;
            
            int widthTotal = 0;
            bool objFound = false;
            int currentWidth = 0;
            int tempRow =0;
            int tempCol =0;
            int firstofrow =0;
            int firstofcol =0;
            int lastofrow =0;
            int lastofcol=0;
            int midofrow=0;
            for(int r = 0; r < imgThresholded.size().height; r++) {
                currentWidth = 0;
                for(int c = 0; c < imgThresholded.size().width; c++) {
                    RGB & color = imgThresholded.ptr<RGB>(r)[c];
                    if((color.r > 200) && (color.g > 200 )&& (color.b > 200)) {
                        // white-ish
                        if(!objFound){
                            objFound=true;
                            tempRow = r;
                            tempCol = c;

                        }
                        currentWidth++;
                    }                   
                }
                objFound=false;
                /*
                if(currentWidth > 5) {
                    //rowCount++;
                    if(objFound == false) {
                        objFound = true;
                        startRowWidth = currentWidth;
                    } else {
                        endRowWidth = currentWidth;
                    }
                    
                }*/
                if(currentWidth > widest) {
                    widest = currentWidth;
                    avgWidth = (avgWidth+widest)/2;
                    firstofrow = tempRow;
                    firstofcol = tempCol;
                    lastofrow = firstofrow + widest;
                    midofrow = firstofrow + (widest/2);
                }
                //cout << "widest: " << widest << endl;
                //cout << "Average width: " <<avgWidth <<endl;
                widthTotal += currentWidth;
            }
            
            //avgWidth = widthTotal / ((double)rowCount);
            cout << "startRow pixel: " << firstofrow << endl;
            cout << "middleRow pixel: " << midofrow << endl;
            cout << "endRow pixel: " << lastofrow << endl;
            cout << "startCol pixel: " << firstofcol << endl;
            dist = Util::disttodrone(20,487.68, 10.16 ,avgWidth);
            avgDist = (avgDist+dist)/2;
            cout << "Distace: " << avgDist << endl;
            widthPair.first = startRowWidth;
            widthPair.second = endRowWidth;
        //}
    }
    
    cout << "startRow width: " << widthPair.first << endl;
    cout << "endRow width: " << widthPair.second << endl;
    cout << "widest: " << widest << endl;
    
    return widthPair;
}
