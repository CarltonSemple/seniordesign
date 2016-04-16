#ifndef UTIL_H
#define UTIL_H

#include <tuple>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"

struct RGB {
    uchar r;
    uchar g;
    uchar b;  
};

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
    
    /**
    ** For scanning, crop out the black sides of the image so that the image is just the person
    ** percentBlank: percentage of a column that can be blank and the column will still be considered a side bar
    **/
    static cv::Mat removeSideBars(cv::Mat source, double percentBlank) 
    {
        int leftCol = 0, rightCol = source.size().width;
        bool leftSide = true;
        int column = 0;
        //for(column = 0; column < source.size().width / 2; column++)
        while(true)
        {
            // go from the left to the middle, and then from the right to the middle
            if(leftSide) {
                if(column >= source.size().width / 2) {
                    leftSide = false; // finished with the left, move to the right
                    column = source.size().width;
                } else {
                    column++;
                }
            } else {
                // right side bar
                if(column <= source.size().width / 2) {
                    break;
                } else {
                    column++;
                }
            }
            int row = 0;
            double blankCount = 0;
            for(row = 0; row < source.size().height; row++)
            {
                RGB & color = source.ptr<RGB>(column)[row];
                if(color.r == 0 && color.g == 0 && color.b == 0) {
                    blankCount += 1.0;
                }
            }
            // if most of the column is blank, move the marker over
            if((blankCount/source.size().height)*100 >= percentBlank) {
                leftCol += 1;
            }                        
        }
        return source;
    }
    
    static void saveImage(std::string pathName, cv::Mat image)
    {
        cv::imwrite(pathName, image);
    }
    
    
};
#endif