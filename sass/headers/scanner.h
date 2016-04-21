#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <vector>
#include "human.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>

class Scanner 
{
private:
    int getLatestSetNumber();
    int getNextSetNumber();
    void displayMinMax();
    void displayMenuOptions();
    void startScan();
    void stopScan();
    void menu();
    //void runCamera();
    void createSliderWindow();
public:
    void runIndependently();
    Human & loadScannedHuman(int setNum);
    
    static void displayScan(int scanNumber)
    {
        Scanner s;
        Human scannedHuman = s.loadScannedHuman(scanNumber);
        std::cout << "loaded " << scannedHuman.getImageCount() << " pictures" << std::endl;
            
        std::vector<cv::Mat> & selfies = scannedHuman.getImages();
        for(int i = 0; i < selfies.size(); i++)
        {
            cv::imshow("video capture", selfies[i]);//img);
            cv::waitKey(10);
        }
    }
};

#endif