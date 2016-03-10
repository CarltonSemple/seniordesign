#ifndef HUMAN_H
#define HUMAN_H

#include <string>
#include <tuple>
#include <vector>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

class Human
{
    std::string name;
    // coordinate values
    int x_left;
    int x_right;
    int y_top;
    int y_bottom;
    
    std::tuple<int,int,int> rgb_color; // color for identification
    
    // images of just the person
    std::vector<cv::Mat> images;
    
        
    public:
    static int idNumber;
    
    Human();
    Human(std::string id, std::pair<int,int> & topLeft, std::pair<int,int> & bottomRight, cv::Mat img);
    
    std::pair<int,int> getTopLeft(); // return top left coordinate
    std::pair<int,int> getTopRight(); // return top right coordinate
    std::pair<int,int> getBottomLeft(); // return bottom left coordinate
    std::pair<int,int> getBottomRight(); // return bottom right coordinate
    std::tuple<int,int,int> getColor();
    
    void setTopLeft(std::pair<int,int> p);
    void setBottomRight(std::pair<int,int> p);
    
    void saveImage(std::string dir) 
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