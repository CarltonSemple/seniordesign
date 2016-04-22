#ifndef HUMAN_H
#define HUMAN_H

#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "blob.h"

class Human : public Blob
{
    bool scanned;
    std::tuple<int,int,int> rgb_color; // color for identification
    // images of just the person
    std::vector<cv::Mat> images;    
    
    public:
       
    static int idNumber;
    
    Human();
    Human(std::string scannedSetId);
    Human(std::string id, std::pair<int,int> & topLeft, std::pair<int,int> & bottomRight, cv::Mat img);
    Human(std::string id, std::pair<int,int> & topLeft, std::pair<int,int> & bottomRight, cv::UMat img);
    
    std::tuple<int,int,int> getColor();
    
    std::vector<cv::Mat> & getImages();
    
    int getImageCount() { return images.size(); }
    
    void addImage(cv::Mat & newPic);
    
    void saveImage(std::string dir); 
    
    static void saveHumanImagesToFiles(std::vector<Human> & collection, std::string directoryName)
    {
        std::string dir = "media/livepeople/" + directoryName + "/";
        //std::string s = "mkdir" + dir;
        //system(s.c_str());
        for(Human h : collection) 
        {
            h.saveImage(dir);
        }
    }   
    
    // Open a new OpenCV window and display the human's images
    void displayImages(bool loop);
    
    // Display the human's images in an existing OpenCV window
    void displayImages(std::string windowName, bool loop);
};

#endif