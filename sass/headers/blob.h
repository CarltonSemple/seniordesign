#ifndef BLOB_H
#define BLOB_H

#include <string>
#include <tuple>
#include <vector>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

/**
* Represents 2-dimensional rectangular objects on an OpenCV Mat image.
* It offers access to the coordinate points of the object's 4 corners,
* as well as several additional & related functions for convenience.  
**/
class Blob
{
    protected:
    std::string name;
    
    // coordinate values
    int x_left;
    int x_right;
    int y_top;
    int y_bottom;
    
    public:
    static int idNumber;
    
    Blob();
    
    /*
    * Returns the top left point as an std::pair,
    * with the first member being the x value & the second member being 
    * the y value.
    */
    std::pair<int,int> getTopLeft(); 
    
    /*
    * Returns the top left point in the form of an OpenCV Point object 
    */
    cv::Point getTopLeftPoint();
    
    /*
    * Returns the top right point as an std::pair,
    * with the first member being the x value & the second member being 
    * the y value.
    */
    std::pair<int,int> getTopRight(); 
    
    /*
    * Returns the top right point in the form of an OpenCV Point object 
    */
    cv::Point getTopRightPoint();
    
    /*
    * Returns the bottom left point as an std::pair,
    * with the first member being the x value & the second member being 
    * the y value.
    */
    std::pair<int,int> getBottomLeft(); 
    
    /*
    * Returns the bottom left point in the form of an OpenCV Point object 
    */
    cv::Point getBottomLeftPoint();
    
    /*
    * Returns the bottom right point as an std::pair,
    * with the first member being the x value & the second member being 
    * the y value.
    */
    std::pair<int,int> getBottomRight(); 
    
    /*
    * Returns the bottom right point in the form of an OpenCV Point object 
    */
    cv::Point getBottomRightPoint();
    
    /*
    * Calculates and returns the center x value along the bottom X axis
    * TODO - rename to getBottomCenterX
    */
    double getCenterX();
        
    /*
    * Set the top left point using an std::pair, with the first
    * member being the x value, and the second member being the y value
    */
    void setTopLeft(std::pair<int,int> p);
    
    /*
    * Set the bottom right point using an std::pair, with the first
    * member being the x value, and the second member being the y value
    */
    void setBottomRight(std::pair<int,int> p);
};

#endif