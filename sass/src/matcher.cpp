#include "matcher.h"
#include <string>
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <thread>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

Matcher::Matcher()
{
    ratio = 0.98f;
}

Matcher::Matcher(float AccuracyRatio)
{
    ratio = AccuracyRatio;
}

int totalMatches = 0;

void getMatches(RobustMatcher & rmatcher, cv::Mat & scannedImg, cv::Mat & potentialImage)
{
    std::vector<cv::DMatch> matches;
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    // get matches 
    rmatcher.match(scannedImg,potentialImage,matches, keypoints1, keypoints2, false);
    totalMatches += matches.size();
}

int Matcher::surfCount(Human & scannedHuman, cv::Mat & potentialImage)
{
    // Prepare the matcher
    int minHessian = 400; // 10;
	RobustMatcher rmatcher;
	rmatcher.setConfidenceLevel(0.98);
	rmatcher.setMinDistanceToEpipolar(1.0);
    // change the accuracy of the matches with the ratio
	rmatcher.setRatio(ratio);
	cv::Ptr<cv::FeatureDetector> detector = cv::xfeatures2d::SURF::create(minHessian);//new cv::SurfFeatureDetector(10); 
	rmatcher.setFeatureDetector(detector);
    
    //cv::namedWindow("Matches", 0);
    //cv::resizeWindow("Matches", 1199, 900);
    
    // Check matches between potentialImage and each of the human's images
    vector<cv::Mat> & scannedImages = scannedHuman.getImages();
    
    vector<thread> threads;
    
    // TODO
    //
    //
    // should probably limit the number of scanned images..
    int i = 0;
    for(Mat img : scannedHuman.getImages())
    {
        /*
        std::vector<cv::DMatch> matches;
        std::vector<cv::KeyPoint> keypoints1, keypoints2;
        // get matches 
        rmatcher.match(img,potentialImage,matches, keypoints1, keypoints2, false);
        totalMatches += matches.size();
        */
        
        getMatches(std::ref(rmatcher), std::ref(img), std::ref(potentialImage));
        
//        threads.push_back(thread(getMatches, std::ref(rmatcher), std::ref(img), std::ref(potentialImage)));
        
        // don't loop through
        //
        //
        // save time with only one image for now................
        if(i > 2)
            break;
        
        /*
        // draw the matches
        cv::Mat imageMatches;
        cv::drawMatches(img,keypoints1,  // 1st image and its keypoints
                        potentialImage,keypoints2,  // 2nd image and its keypoints
                        matches,			// the matches
                        imageMatches,		// the image produced
                        cv::Scalar(255,255,255)); // color of the lines
        */
        //cv::imshow("Matches",imageMatches);
        
        
        //cv::waitKey();
        // close the window
        //cvDestroyWindow("Matches");
        //cvReleaseImage(&imageMatches);
    }
    
    for(thread & tt : threads) {
        tt.join();
    }
    
    return totalMatches;
}

Mat mmimg; Mat templ; Mat result;
const char* image_window = "templateMatch - Source Image";
const char* result_window = "templateMatch - Result window";
int match_method;
int max_Trackbar = 5;

void MatchingMethodCallBack(int, void*);

void Matcher::templateMatchingWithoutCallBack(cv::UMat & templateImg, cv::UMat & sourceImg, int methodNum)
{
    UMat resultt2;
    namedWindow( image_window, WINDOW_AUTOSIZE );
    namedWindow( result_window, WINDOW_AUTOSIZE );
    UMat img_display;
    sourceImg.copyTo( img_display );
    int result_cols =  abs(sourceImg.cols - templateImg.cols + 1);
    int result_rows = abs(sourceImg.rows - templateImg.rows + 1);
    //cout << "r cols: " << result_cols << endl;
    //cout << "r rows: " << result_rows << endl;
    resultt2.create( result_rows, result_cols, CV_32FC1 );
    cout << "Type: " << templateImg.type()<<endl;
    //cout << "Size: " << templateImg.type()<<endl;
    matchTemplate( sourceImg, templateImg, resultt2, methodNum );
    //normalize( resultt2, resultt2, 0, 1, NORM_MINMAX, -1, Mat() );
    
    double minVal, maxVal; 
    Point minLoc, maxLoc, matchLoc;
    
    minMaxLoc( resultt2, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
    
    // view the max val and min val
    // make a threshold
    
    
    if( methodNum  == TM_SQDIFF || methodNum == TM_SQDIFF_NORMED )
    { 
        matchLoc = minLoc; 
        cout << "minLocation. min value: " << minVal << endl; 
        // if(minVal >= 0) {
            rectangle( img_display, matchLoc, Point( matchLoc.x + templateImg.cols , matchLoc.y + templateImg.rows ), Scalar::all(0), 2, 8, 0 );
            rectangle( resultt2, matchLoc, Point( matchLoc.x + templateImg.cols , matchLoc.y + templateImg.rows ), Scalar::all(0), 2, 8, 0 );
        //}
    }
    else
    { 
        matchLoc = maxLoc; 
        cout << "maxLocation. max value: " << maxVal << endl;
        double po = 100000000;
        cout << 4.5 * po << endl;
        if(maxVal >= 4.5 * po) { 
            rectangle( img_display, matchLoc, Point( matchLoc.x + templateImg.cols , matchLoc.y + templateImg.rows ), Scalar::all(0), 2, 8, 0 );
            rectangle( resultt2, matchLoc, Point( matchLoc.x + templateImg.cols , matchLoc.y + templateImg.rows ), Scalar::all(0), 2, 8, 0 );
        }
    }
    
    
    
    imshow( image_window, img_display );
    imshow( result_window, resultt2 );
    waitKey(1);
}

void Matcher::templateMatching(cv::Mat & templateImg, cv::Mat & sourceImg)
{
    templ = templateImg;
    mmimg = sourceImg;
    namedWindow( image_window, WINDOW_AUTOSIZE );
    namedWindow( result_window, WINDOW_AUTOSIZE );
    const char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    createTrackbar( trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethodCallBack );
    MatchingMethodCallBack(0, 0);
    waitKey(0);
}

void MatchingMethodCallBack(int, void*)
{
    //cout << "match method: " << match_method << endl;
    Mat img_display;
    mmimg.copyTo( img_display );
    
    int result_cols =  mmimg.cols - templ.cols + 1;
    int result_rows = mmimg.rows - templ.rows + 1;
    
    result.create( result_rows, result_cols, CV_32FC1 );
    
    matchTemplate( mmimg, templ, result, match_method );
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
    
    double minVal, maxVal; 
    Point minLoc, maxLoc, matchLoc;
    
    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
    
    if( match_method  == TM_SQDIFF || match_method == TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
    else
    { matchLoc = maxLoc; }
    
    rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
    rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
    
    imshow( image_window, img_display );
    imshow( result_window, result );
    return;
}