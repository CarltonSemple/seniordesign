#ifndef HUMAN_H
#define HUMAN_H

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
    
    std::tuple<int,int,int> getColor();
    
    std::vector<cv::Mat> & getImages();
    
    void addImage(cv::Mat & newPic);
    
    void saveImage(std::string dir); 
    
    static void saveHumanImagesToFiles(std::vector<Human> & collection, std::string directoryName)
    {
        std::string dir = "media/" + directoryName + "/";
        //std::string s = "mkdir" + dir;
        //system(s.c_str());
        for(Human h : collection) 
        {
            h.saveImage(dir);
        }
    }   
};

#endif