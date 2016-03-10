#include "blob.h"

Blob::Blob()
{
    
}

std::pair<int,int> Blob::getTopLeft()
{
    std::pair<int,int> c(x_left,y_top);
    return c;
}

std::pair<int,int> Blob::getTopRight()
{
    std::pair<int,int> c(x_right,y_top);
    return c;
}

std::pair<int,int> Blob::getBottomLeft()
{
    std::pair<int,int> c(x_left, y_bottom);
    return c;
}

std::pair<int,int> Blob::getBottomRight()
{
    std::pair<int,int> c(x_right, y_bottom);
    return c;
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