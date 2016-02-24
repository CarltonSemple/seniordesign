#include "libfreenect.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
//#include <cv.h>
//#include <cxcore.h>
//#include <highgui.h>


using namespace cv;
using namespace std;

#define DEFAULT_MIN_DISTANCE 400
#define DEFAULT_MAX_DISTANCE 1000
#define CHANGE_RATE 10

// Global variables
int minDistance = DEFAULT_MIN_DISTANCE;
int maxDistance = DEFAULT_MAX_DISTANCE;

struct RGB {
    uchar r;
    uchar g;
    uchar b;  
};


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
			std::cout << "RGB callback" << std::endl;
			m_rgb_mutex.lock();
			uint8_t* rgb = static_cast<uint8_t*>(_rgb);
			rgbMat.data = rgb;
			m_new_rgb_frame = true;
			m_rgb_mutex.unlock();
		};
		
		// Do not call directly even in child
		void DepthCallback(void* _depth, uint32_t timestamp) {
			std::cout << "Depth callback" << std::endl;
			m_depth_mutex.lock();
			uint16_t* depth = static_cast<uint16_t*>(_depth);
			depthMat.data = (uchar*) depth;
			std::cout << "start" << std::endl;
			for(int i = 0; i < 480; i++) {
				for(int c = 0; c < 640; c++) {
					//std::cout << depthMat.at<uint16_t>(i, c) << " ";
					uint16_t dist = depthMat.at<uint16_t>(i, c);
					
					// change color to black if distance is outside of 
					// desired range
					if(dist < 400 || dist > 1000) {
						RGB & color = rgbMat.ptr<RGB>(i)[c];
						color.r = 0;
						color.g = 0;
						color.b = 0;
					}
				}
				//std::cout << std::endl;
			}
			//std::cout << "end" << std::endl;
			
			// show modified RGB image 
			cv::imshow("rgb", rgbMat);
			
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

void displayMinMax() {
    cout << "minimum distance: " << minDistance << endl;
    cout << "maximum distance: " << maxDistance << endl;
}

void displayMenuOptions() {
    cout << "Scanner Application" << endl;
    cout << "S: Start scan" << endl;
    cout << "E: End scan" << endl;
    cout << "n: Decrease minimum distance" << endl;
    cout << "N: Increase minimum distance" << endl;
    cout << "m: Decrease maximum distance" << endl;
    cout << "M: Increase maximum distance" << endl;
}

void decreaseMinimum() {
    if(minDistance - CHANGE_RATE >= 0) {
        minDistance -= CHANGE_RATE;
    }
    displayMinMax();
}

void increaseMinimum() {
    if(minDistance + CHANGE_RATE <= maxDistance) {
        minDistance += CHANGE_RATE;
    }
    displayMinMax();
}

void decreaseMaximum() {
    if(maxDistance - CHANGE_RATE >= minDistance) {
        maxDistance -= CHANGE_RATE;
    }
    displayMinMax();
}

void increaseMaximum() {
    maxDistance += CHANGE_RATE;
    displayMinMax();
}

void startScan() {
    
}

char menu() {
    displayMenuOptions();
    char c = 'z';
    cin >> c;
    switch(c)
    {
        case 's':
        case 'S':
            break;
        case 'e':
        case 'E': 
            break;
        case 'n': decreaseMinimum(); break;
        case 'N': increaseMinimum(); break;
        case 'm': decreaseMaximum(); break;
        case 'M': increaseMaximum(); break;
    }
    return c;
}

int main(int argc, char **argv) {
    
    menu();
        
	bool die(false);
	string filename("snapshot");
	string suffix(".png");
    int i_snap(0), iter(0);
	
	Mat depthMat(Size(640,480),CV_16UC1);
	Mat depthf (Size(640,480),CV_8UC1);
	Mat rgbMat(Size(640,480),CV_8UC3,Scalar(0));
	Mat ownMat(Size(640,480),CV_8UC3,Scalar(0));
	
	// The next two lines must be changed as Freenect::Freenect
	// isn't a template but the method createDevice:
	// Freenect::Freenect<MyFreenectDevice> freenect;
	// MyFreenectDevice& device = freenect.createDevice(0);
	// by these two lines:
	
	Freenect::Freenect freenect;
	MyFreenectDevice& device = freenect.createDevice<MyFreenectDevice>(0);
	
	namedWindow("rgb",CV_WINDOW_AUTOSIZE);
	namedWindow("depth",CV_WINDOW_AUTOSIZE);
	device.startVideo();
	device.startDepth();
	while (!die) {
		device.getVideo(rgbMat);
        
        // save to file
        imwrite("test.jpg", save_img); // A JPG FILE IS BEING SAVED
        
		device.getDepth(depthMat);
		//cv::imshow("rgb", rgbMat);
		depthMat.convertTo(depthf, CV_8UC1, 255.0/2048.0);
		cv::imshow("depth",depthf);
		char k = cvWaitKey(5);
		if( k == 27 ){
			cvDestroyWindow("rgb");
			cvDestroyWindow("depth");
			break;
		}
		if( k == 8 ) {
			std::ostringstream file;
			file << filename << i_snap << suffix;
			cv::imwrite(file.str(),rgbMat);
			i_snap++;
		}
		//if(iter >= 1000) break;
		//iter++;
	}
	
	device.stopVideo();
	device.stopDepth();
	return 0;
}
