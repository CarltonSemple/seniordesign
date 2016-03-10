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

std::tuple<int,int,int> Human::getColor()
{
    std::tuple<int,int,int> c(rgb_color);
    return c;
}