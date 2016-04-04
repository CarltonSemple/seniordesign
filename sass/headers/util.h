#ifndef UTIL_H
#define UTIL_H

#include <tuple>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"

class Util
{
    public:
    static double distance(std::pair<double,double> p1, std::pair<double,double> p2)
    {
        double m = pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2);
        double s = sqrt(m);
        return s;
    }  
    
    static cv::Mat cropImage(cv::Mat source)
    {
        int wDivisor = 3;
        float wsRatio = 1.0/wDivisor, weRatio = 1.8/wDivisor;
        int hDivisor = 10;
        float hsRatio = 0.5/hDivisor, heRatio = 8.8/hDivisor;
        int width = source.size().width, height = source.size().height;
        int startX = width * wsRatio, startY = height * hsRatio;
        int endX = (width * weRatio), endY = (height * heRatio);
        
        cv::Rect rectti(startX, startY, endX, endY); //10, 10, 310, 100);
        cv::Mat ree(source, rectti);
        return ree;
    }
    
    static void saveImage(std::string pathName, cv::Mat image)
    {
        cv::imwrite(pathName, image);
    }
};

struct RGB {
    uchar r;
    uchar g;
    uchar b;  
};

#endif