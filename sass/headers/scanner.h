#ifndef SCANNER_H
#define SCANNER_H

#include "human.h"

class scanner 
{
private:
    int getLatestSetNumber();
    int getNextSetNumber();
    void displayMinMax();
    void displayMenuOptions();
    void decreaseMinimum();
    void increaseMinimum();
    void decreaseMaximum();
    void increaseMaximum();
    void startScan();
    void stopScan();
    char menu();
    //void runCamera();
    void createSliderWindow();
public:
    void runIndependently();
    Human & loadScannedHuman(int setNum);
};

#endif