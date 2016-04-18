#include <iostream>
#include <vector>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

#define COLOR_RANGE_SIZE 20

struct HueRange
{
    int low;
    int high;
    int count;
};

vector<HueRange> ranges;

void initializeColorRanges() 
{
    int i;
    for(i = 0; (i + COLOR_RANGE_SIZE) <= 179;) {
        HueRange range = {i, i + COLOR_RANGE_SIZE, 0};
        ranges.push_back(range);
        i = i + COLOR_RANGE_SIZE;
    }
    if(i < 179) {
        HueRange range = {i, 179, 0};
        ranges.push_back(range);
    }
}

/*
    Get pixel intensities of all the pixels in the image.  In the future,
    should change this to just the center of the picture.  Have on-screen markers
    on the sides that define the region in which the person should stand, and
    from which the pixels will be measured.  This way, just the person is measured.
*/
int getActivePixelCount(Mat & image, HueRange range) 
{    
    int count = 0;
    int rows = 0;
    int cols = 0;
    cv::Size s = image.size();
    rows = s.height;
    cols = s.width;
    
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            // columns should be limited.....
            
            // must double check what type of value is being returned
            unsigned char val = image.at<uchar>(r, c); // y, x
            if(val >= 200) // white-ish 
            {
                count++;
            }            
            //vec->push_back(image.at<uchar>(r, c));
        }
    }
    
    return count;
}

int iLowH = 0;
int iHighH = 179;

int iLowS = 0; 
int iHighS = 255;

int iLowV = 0;
int iHighV = 255;

void setHueRangeCounts(Mat & imageHSV) 
{
    for(int i = 0; i < ranges.size(); i++) 
    {
        Mat imgThresholded;
        inRange(imageHSV, Scalar(ranges[i].low, iLowS, iLowV), Scalar(ranges[i].high, iHighS, iHighV), imgThresholded); //Threshold the image
        
        //morphological opening (remove small objects from the foreground)
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

        //morphological closing (fill small holes in the foreground)
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
                
        imshow("Thresholded Image", imgThresholded); //show the thresholded image

        ranges[i].count = getActivePixelCount(imgThresholded, ranges[i]);
    }
}

 int main( int argc, char** argv )
 {
    VideoCapture cap(0); //capture the video from web cam

    if ( !cap.isOpened() )  // if not success, exit program
    {
        cout << "Cannot open the web cam" << endl;
        return -1;
    }

    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

    initializeColorRanges();

    //Create trackbars in "Control" window
    cvCreateTrackbar("LowHue", "Control", &iLowH, 179); //Hue (0 - 179)
    cvCreateTrackbar("HighHue", "Control", &iHighH, 179);

    cvCreateTrackbar("LowSat", "Control", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighSat", "Control", &iHighS, 255);

    // not as necessary
    //cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    //cvCreateTrackbar("HighV", "Control", &iHighV, 255);
    
    namedWindow("Thresholded Image");

    while (true)
    {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video

        if (!bSuccess) //if not success, break loop
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }

        Mat imgHSV;

        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

        setHueRangeCounts(imgHSV);
        
        /*
        Mat imgThresholded;
        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
        //morphological opening (remove small objects from the foreground)
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
        //morphological closing (fill small holes in the foreground)
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        //setHueRangeCounts(imgThresholded);
        */
        
        cout << "size: " << ranges.size() << endl;
        for(int i = 0; i < ranges.size(); i++) {
            cout << ranges[i].count << ", ";
        }
        cout << endl;

        //imshow("Thresholded Image", imgThresholded); //show the thresholded image
        imshow("Original", imgOriginal); //show the original image
        imshow("HSV", imgHSV);
        
        

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            break; 
        }
    }

   return 0;

}