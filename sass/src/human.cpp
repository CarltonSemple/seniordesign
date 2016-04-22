#include "human.h"

using namespace cv;

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

Human::Human(std::string id, std::pair<int,int> & topLeft, std::pair<int,int> & bottomRight, cv::UMat img)
{
    scanned = false;
    name = id;
    
    //
    setTopLeft(topLeft);
    setBottomRight(bottomRight);
    Mat m;
    img.copyTo(m);
    images.push_back(m);
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

void Human::displayImages(bool loop)
{
    cv::namedWindow(name, CV_WINDOW_NORMAL); //CV_WINDOW_AUTOSIZE);
    cv::resizeWindow(name, 800, 800);
    displayImages("Potential Target", loop);
}

void Human::displayImages(std::string windowName, bool loop)
{
    // display loaded images
    if(loop)
    {
        while(true)
        {
            for(cv::Mat imggg : images)
            {
                cv::imshow(windowName, imggg);
                //waitKey(30);
                if (waitKey(30) == 27) { // wait for escape button press
                    //std::cout << "escape" << std::endl;
                    return;
                } 
            }
            
        }
    } else {
        for(cv::Mat imggg : images)
        {
            cv::imshow(windowName, imggg);
            waitKey(30);
        }
    }
    
}