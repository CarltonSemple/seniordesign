#include "human.h"

/* Getters */

Human::Human()
{
    
}

Human::Human(std::string scannedSetId)
{
    scanned = true;
}

Human::Human(std::string id, std::pair<int,int> & topLeft, std::pair<int,int> & bottomRight, cv::Mat img)
{
    scanned = false;
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

std::vector<cv::Mat> & Human::getImages()
{
    return images;
}

void Human::addImage(cv::Mat & newPic)
{
    images.push_back(newPic);
}

void Human::saveImage(std::string dir)
{
    std::string fn = dir + name;
    if(images.size() > 0)
    {
        for(int i = 0; i < images.size(); i++) 
        {
            std::string fn_s = fn + "_" + std::to_string(i) + ".jpg";
            cv::imwrite(fn_s, images[i]);
        }
    } 
} 