#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <tuple>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"

using namespace std;

struct RGB {
    uchar r;
    uchar g;
    uchar b;  
};

struct Position {
    int x2d;
    int y2d;
    double radius2d;
    
    long double distanceDirect; // calculated distance to drone
    long double x3d; // distance to left or right of the middle of the station
    long double z3d; // distance from base of station
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

    //enter distances in centimeters and will return in centimeters
    static double disttodrone(long double startPixelWidth,long double knownDistance, long double knownWidth, long double newPixelWidth)
    {
        long double fl = (startPixelWidth*knownDistance)/knownWidth;
        long double dist = (knownWidth*fl)/newPixelWidth;
        return dist;
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
        double ySize = source.size().height;
        double xSize = source.size().width;
        int leftCol = 0, rightCol = xSize;
        bool leftSide = true;
        int column = 0;
        //for(column = 0; column < source.size().width / 2; column++)
        
        /*
        for(int x = 0; x < xSize; x++)
        {
            for(int y = 0; y < ySize; y++) {
                RGB & color = source.ptr<RGB>(y)[x];
                color.r = 255;
                color.g = 0;
                color.b = 0;
            }
        }*/
        
        // crop left side
        for(int x = 0; x < xSize; x++) {
            float blankCount = 0.0;
            for(int y = 0; y < ySize; y++) {
                RGB & color = source.ptr<RGB>(y)[x];
                //cout << "r: " << color.r << " g: " << color.g << " b: " << color.b << endl;
                if(color.r <5 && color.g < 5 && color.b < 5) {
                    blankCount+=1.0;
                }
            }
            // if most of the column is not blank
            // move on to right side
            if((blankCount/ySize)*100 < percentBlank) {
                cout << "end left" << endl;
                leftCol = x;
                /*
                for(int i = 0; i < x; i++) {
                    for(int y = 0; y < ySize; y++) {
                        RGB & color = source.ptr<RGB>(y)[i];
                        color.r = 255;
                        color.g = 0;
                        color.b = 0;
                    }
                }*/
                break; 
            }
        }
        
        rightCol = xSize - 1;
        
        // crop right side
        for(int x = xSize - 1; x >= 0; x--) {
            float blankCount = 0.0;
            for(int y = 0; y < ySize; y++) {
                RGB & color = source.ptr<RGB>(y)[x];
                if(color.r < 5 && color.g < 5 && color.b < 5) {
                    blankCount+=1.0;
                }
            }
            // if not enough of the column is blank, finish
            if((blankCount/ySize)*100 < percentBlank) {
                cout << "end right" << endl;
                rightCol = x;
                /*
                for(int i = xSize; i > x; i--) {
                    for(int y = 0; y < ySize; y++) {
                        RGB & color = source.ptr<RGB>(y)[i];
                        color.r = 0;
                        color.g = 0;
                        color.b = 255;
                    }
                }*/
                break; 
            }
        }
                

        cout << "leftCol: " << leftCol << endl;
        cout << "rightCol: " << rightCol << endl;
        cout << "source height: " << ySize << endl;
        cout << "source width: " << xSize << endl;
        if(rightCol <= leftCol) {
            return source;
        }
        //cv::Rect rectti(leftCol, 0, rightCol - leftCol, source.size().height);
        //cv::Rect rectti(leftCol, 0, rightCol, ySize);
        //cv::Rect rectti(leftCol, 0, xSize - leftCol, source.size().height);
        cv::Rect rectti(leftCol, 0, rightCol - leftCol, source.size().height);
        cv::Mat ree(source, rectti);
        
        return ree;
    }
    
    static void saveImage(std::string pathName, cv::Mat image)
    {
        cv::imwrite(pathName, image);
    }

    
        
};
#endif