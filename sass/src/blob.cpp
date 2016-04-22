#include "blob.h"

Blob::Blob()
{
    
}

std::pair<int,int> Blob::getTopLeft()
{
    std::pair<int,int> c(x_left,y_top);
    return c;
}

cv::Point Blob::getTopLeftPoint()
{
    auto pa = getTopLeft();
    return cv::Point(pa.first, pa.second);
}

std::pair<int,int> Blob::getTopRight()
{
    std::pair<int,int> c(x_right,y_top);
    return c;
}

cv::Point Blob::getTopRightPoint()
{
    auto pa = getTopRight();
    return cv::Point(pa.first, pa.second);
}

std::pair<int,int> Blob::getBottomLeft()
{
    std::pair<int,int> c(x_left, y_bottom);
    return c;
}

cv::Point Blob::getBottomLeftPoint()
{
    auto pa = getBottomLeft();
    return cv::Point(pa.first, pa.second);
}

std::pair<int,int> Blob::getBottomRight()
{
    std::pair<int,int> c(x_right, y_bottom);
    return c;
}

cv::Point Blob::getBottomRightPoint()
{
    auto pa = getBottomRight();
    return cv::Point(pa.first, pa.second);
}

double Blob::getCenterX()
{
    auto br = getBottomRightPoint();
    auto bl = getBottomLeftPoint();
    
    return (br.x + bl.x) / 2.0;
}

void Blob::setTopLeft(std::pair<int,int> p)
{
    x_left = p.first;
    y_top = p.second;
}

void Blob::setBottomRight(std::pair<int,int> p)
{
    x_right = p.first;
    y_bottom = p.second;
}