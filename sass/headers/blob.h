#ifndef BLOB_H
#define BLOB_H

#include <string>
#include <tuple>
#include <vector>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

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
    
    std::pair<int,int> getTopLeft(); // return top left coordinate
    std::pair<int,int> getTopRight(); // return top right coordinate
    std::pair<int,int> getBottomLeft(); // return bottom left coordinate
    std::pair<int,int> getBottomRight(); // return bottom right coordinate
    
    void setTopLeft(std::pair<int,int> p);
    void setBottomRight(std::pair<int,int> p);
};

#endif