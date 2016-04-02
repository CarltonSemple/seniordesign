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
    for(Mat img : scannedHuman.getImages())
    {
        /*
        std::vector<cv::DMatch> matches;
        std::vector<cv::KeyPoint> keypoints1, keypoints2;
        // get matches 
        rmatcher.match(img,potentialImage,matches, keypoints1, keypoints2, false);
        totalMatches += matches.size();
        */
        //thread mt(getMatches, std::ref(rmatcher), std::ref(img), std::ref(potentialImage));
        threads.push_back(thread(getMatches, std::ref(rmatcher), std::ref(img), std::ref(potentialImage)));
        
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