#ifndef SCANNER_H
#define SCANNER_H

class scanner 
{
private:
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
};

#endif