#include "libfreenect.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <pthread.h>
#include <sstream>
#include <string>
#include <thread>
#include <limits>
#include "scanner.h"
#include "util.h"
//#include <cv.h>
//#include <cxcore.h>
//#include <highgui.h>


using namespace cv;
using namespace std;

#define DEFAULT_MIN_DISTANCE 400
#define DEFAULT_MAX_DISTANCE 870
#define CHANGE_RATE 10
#define FPS_RECORD 1 // frames per second saved when recording

// Global variables
int minDistance = DEFAULT_MIN_DISTANCE;
int maxDistance = DEFAULT_MAX_DISTANCE;

bool quit = false;
bool camera_saving = false;
int img_frame = 0;
int set_number = 0;

// filenames
string mediaFolder = "media/scans/";
string setCountFileName = "setnumber";

class myMutex {
	public:
		myMutex() {
			pthread_mutex_init( &m_mutex, NULL );
		}
		void lock() {
			pthread_mutex_lock( &m_mutex );
		}
		void unlock() {
			pthread_mutex_unlock( &m_mutex );
		}
	private:
		pthread_mutex_t m_mutex;
};

class MyFreenectDevice : public Freenect::FreenectDevice {
	public:
		MyFreenectDevice(freenect_context *_ctx, int _index)
	 		: Freenect::FreenectDevice(_ctx, _index), m_buffer_depth(FREENECT_DEPTH_11BIT),
			m_buffer_rgb(FREENECT_VIDEO_RGB), m_gamma(2048), m_new_rgb_frame(false),
			m_new_depth_frame(false), depthMat(Size(640,480),CV_16UC1),
			rgbMat(Size(640,480), CV_8UC3, Scalar(0)),
			ownMat(Size(640,480),CV_8UC3,Scalar(0)) {
			
			for( unsigned int i = 0 ; i < 2048 ; i++) {
				float v = i/2048.0;
				v = std::pow(v, 3)* 6;
				m_gamma[i] = v*6*256;
			}
		}
		
		// Do not call directly even in child
		void VideoCallback(void* _rgb, uint32_t timestamp) {
			//std::cout << "RGB callback" << std::endl;
			m_rgb_mutex.lock();
			uint8_t* rgb = static_cast<uint8_t*>(_rgb);
			rgbMat.data = rgb;
			m_new_rgb_frame = true;
			m_rgb_mutex.unlock();
		};
		
		// Do not call directly even in child
		void DepthCallback(void* _depth, uint32_t timestamp) {
			//std::cout << "Depth callback" << std::endl;
			m_depth_mutex.lock();
			uint16_t* depth = static_cast<uint16_t*>(_depth);
			depthMat.data = (uchar*) depth;
			//std::cout << "start" << std::endl;
            
            cvtColor(rgbMat, rgbMat, CV_BGR2RGB);
			for(int i = 0; i < 480; i++) {
				for(int c = 0; c < 640; c++) {
					//std::cout << depthMat.at<uint16_t>(i, c) << " ";
					uint16_t dist = depthMat.at<uint16_t>(i, c);
					
                    // fix color of RGB
                    //uint16_t & colorHere = rgbMat.ptr<uint16_t>(i)[c];
                    //colorHere = (colorHere & 0x000000ff) << 16 | (colorHere & 0x0000FF00) | (colorHere & 0x00FF0000) >> 16;
                    //uchar & temptation = colorHere.r;
                    //colorHere.r = colorHere.b;
                    //colorHere.b = temptation;
                   
					// change color to black if distance is outside of 
					// desired range
					if(dist < minDistance || dist > maxDistance) {
						RGB & color = rgbMat.ptr<RGB>(abs(i+20))[abs(c-10)];
						color.r = 0;
						color.g = 0;
						color.b = 0;
					}
				}
				//std::cout << std::endl;
			}
			//std::cout << "end" << std::endl;
			
			// show modified RGB image
            Mat cropped = Util::cropImage(rgbMat); 
            
			cropped = Util::removeSideBars(cropped, 98.0);
            cv::imshow("rgb", cropped);
            
            // save to file
            std::ostringstream imgname;
            if(camera_saving == true && img_frame < std::numeric_limits<int>::max())
            {
                
                imgname << mediaFolder << "img_set_" << set_number << "_" << img_frame++ << ".jpg";
                imwrite(imgname.str(), cropped); // A JPG FILE IS BEING SAVED
                
            }
			
			//std::cout << "rows: " << depthMat.rows << " cols: " << depthMat.cols << std::endl;
			//std::cout << depthMat.at<uint16_t>(0,0) << std::endl;
			m_new_depth_frame = true;
			m_depth_mutex.unlock();
		}
		
		bool getVideo(Mat& output) {
			m_rgb_mutex.lock();
			if(m_new_rgb_frame) {
				cv::cvtColor(rgbMat, output, CV_RGB2BGR);
				m_new_rgb_frame = false;
				m_rgb_mutex.unlock();
				return true;
			} else {
				m_rgb_mutex.unlock();
				return false;
			}
		}
		
		bool getDepth(Mat& output) {
				m_depth_mutex.lock();
				if(m_new_depth_frame) {
					depthMat.copyTo(output);
					m_new_depth_frame = false;
					m_depth_mutex.unlock();
					return true;
				} else {
					m_depth_mutex.unlock();
					return false;
				}
			}
	private:
		std::vector<uint8_t> m_buffer_depth;
		std::vector<uint8_t> m_buffer_rgb;
		std::vector<uint16_t> m_gamma;
		Mat depthMat;
		Mat rgbMat;
		Mat ownMat;
		myMutex m_rgb_mutex;
		myMutex m_depth_mutex;
		bool m_new_rgb_frame;
		bool m_new_depth_frame;
};

int Scanner::getLatestSetNumber()
{
    int num = 0;
    ifstream in;
    in.open(mediaFolder + setCountFileName);
    in >> num;
    return num;
}

/**
* Reads the latest set number from the media folder, returns the next number and sets the number to the next #.
**/
int Scanner::getNextSetNumber() 
{
    int num = 0;
    ifstream in;
    in.open(mediaFolder + setCountFileName);
    in >> num;
    num++;
    in.close();
    ofstream out;
    out.open(mediaFolder + setCountFileName);
    out << num;
    return num;
}

void Scanner::displayMinMax() {
    cout << "minimum distance: " << minDistance << endl;
    cout << "maximum distance: " << maxDistance << endl;
}

void Scanner::displayMenuOptions() {
    cout << "Scanner Application" << endl << endl;
    cout << "S: Start scan" << endl;
    cout << "D: Display Scan" << endl;
    cout << "E: End scan" << endl;
    cout << "q: Quit" << endl;
    cout << ": ";
}

void Scanner::startScan() {
    // start new set
    set_number = getNextSetNumber();
    cout << "recording set "<< set_number << endl << endl;
    camera_saving = true;
    img_frame = 0;
}

void Scanner::stopScan() {
    camera_saving = false;
}

char Scanner::menu() {
    displayMenuOptions();
    char c = 'z';
    cin >> c;
    switch(c)
    {
        case 's':
        case 'S': 
            startScan();
            break;
        case 'e':
        case 'E': 
            stopScan();
            break;
        case 'd':
        case 'D':
            cout << "Enter scan number: ";
            int scNu;
            cin >> scNu;
            displayScan(scNu);
            break;
        case 'q':
        case 'Q': quit = true; break;
    }
    return c;
}

Human & Scanner::loadScannedHuman(int setNum)
{
    Human * h = new Human(to_string(setNum));
    
    int picNum = 0;
    
    // Load all of the images matching this person's set #
    std::ostringstream imgname;
    while(true)
    {
        //try {            
            imgname << mediaFolder << "img_set_" << setNum << "_" << picNum++ << ".jpg";
            ifstream f(imgname.str());       
            if(f.good()){
                Mat newImage = imread(imgname.str(), CV_LOAD_IMAGE_COLOR);
                h->addImage(newImage);
            } else {
                //cout << imgname.str() << endl;
                //cout << "picNum: " << picNum << endl;
                //cout << "ending image load" << endl;
                break;
            }
            imgname.clear();
            imgname.str("");
        //} catch(...) {
        //    break;
        //}
    }
    return *h;
}

void runCamera()
{
	std::string filename("snapshot");
	std::string suffix(".png");
    int i_snap(0), iter(0);
	
    Mat depthMat(Size(640,480),CV_16UC1);
	Mat depthf (Size(640,480),CV_8UC1);
	Mat rgbMat(Size(640,480),CV_8UC3,Scalar(0));
	Mat ownMat(Size(640,480),CV_8UC3,Scalar(0));
	
	Freenect::Freenect freenect;
	MyFreenectDevice& device = freenect.createDevice<MyFreenectDevice>(0);
	    
	namedWindow("rgb",CV_WINDOW_AUTOSIZE);
	namedWindow("depth",CV_WINDOW_AUTOSIZE);
	device.startVideo();
	device.startDepth();
	while (!quit) {
		device.getVideo(rgbMat);
        
        
                
		device.getDepth(depthMat);
		//cv::imshow("rgb", rgbMat);
		depthMat.convertTo(depthf, CV_8UC1, 255.0/2048.0);
		cv::imshow("depth",depthf);
		char k = cvWaitKey(5);
        
        
        
        /*
		if( k == 27 ){
			cvDestroyWindow("rgb");
			cvDestroyWindow("depth");
			break;
		}*/
        /*
		if( k == 8 ) {
			std::ostringstream file;
			file << filename << i_snap << suffix;
			cv::imwrite(file.str(),rgbMat);
			i_snap++;
		}*/
		//if(iter >= 1000) break;
		//iter++;
	}
    
    cvDestroyWindow("rgb");
	cvDestroyWindow("depth");
	
	device.stopVideo();
	device.stopDepth();
}

void Scanner::createSliderWindow() 
{
    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
    cvCreateTrackbar("LowS", "Control", &minDistance, 1400); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &maxDistance, 1500);
}

void Scanner::runIndependently()
{
    createSliderWindow();
    thread camthread(&runCamera);
    
    while (true)
    {
        menu();
        if(quit)
            break;
    }
	
    camthread.join();
}