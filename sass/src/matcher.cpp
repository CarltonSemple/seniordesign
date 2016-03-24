#include "matcher.h"
#include <string>
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

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

int Matcher::surfCount(Human & scannedHuman, cv::Mat & potentialImage)
{
    int totalMatches = 0;
    
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
    //for(Mat img : scannedHuman.getImages())
    for(int i = 0; i < 1; i++) //i < scannedImages.size(); i++)
    {
        Mat & img = scannedImages[i];
        std::vector<cv::DMatch> matches;
        std::vector<cv::KeyPoint> keypoints1, keypoints2;
        // get matches 
        rmatcher.match(img,potentialImage,matches, keypoints1, keypoints2, true);
        totalMatches += matches.size();
        
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
    
    return totalMatches;
}