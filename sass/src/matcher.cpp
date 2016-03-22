#include "matcher.h"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

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
    
    // Check matches between potentialImage and each of the human's images
    for(Mat img : scannedHuman.getImages())
    {
        std::vector<cv::DMatch> matches;
        std::vector<cv::KeyPoint> keypoints1, keypoints2;
        // get matches 
        rmatcher.match(img,potentialImage,matches, keypoints1, keypoints2);
        totalMatches += matches.size();
    }
    
    return totalMatches;
}