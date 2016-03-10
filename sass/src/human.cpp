#include "human.h"

/* Getters */

Human::Human(std::string id, std::pair<int,int> & topLeft, std::pair<int,int> & bottomRight, cv::Mat img)
{
    name = id;
    
    //
    setTopLeft(topLeft);
    setBottomRight(bottomRight);
    images.push_back(img);
    //
    //
}

std::pair<int,int> Human::getTopLeft()
{
    std::pair<int,int> c(x_left,y_top);
    return c;
}

std::pair<int,int> Human::getTopRight()
{
    std::pair<int,int> c(x_right,y_top);
    return c;
}

std::pair<int,int> Human::getBottomLeft()
{
    std::pair<int,int> c(x_left, y_bottom);
    return c;
}

std::pair<int,int> Human::getBottomRight()
{
    std::pair<int,int> c(x_right, y_bottom);
    return c;
}

std::tuple<int,int,int> Human::getColor()
{
    std::tuple<int,int,int> c(rgb_color);
    return c;
}

void Human::setTopLeft(std::pair<int,int> p)
{
    x_left = p.first;
    y_top = p.second;
}

void Human::setBottomRight(std::pair<int,int> p)
{
    x_right = p.first;
    y_bottom = p.second;
}