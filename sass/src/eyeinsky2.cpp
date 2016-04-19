#include <iostream>
#include <cstdlib>
#include <signal.h>
#include "eyeinsky.h"
#include "backgroundSubtractor.h"
#include "human.h"
#include "objectdetector.h"
#include "matcher.h"
#include "util.h"
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/logger.h>
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/ocl.hpp> // opencl
#include <functional>
#include "communicationBox.h"
using namespace cv;
using namespace std; 

enum Processor { cl, gl, cpu };

bool protonect_shutdown = false; ///< Whether the running application should shut down.

UMat dst2;

int iLowH;
int iHighH;

int iLowS; 
int iHighS;

int iLowV;
int iHighV;

void sigint_handler(int s)
{
  protonect_shutdown = true;
}
void control(){
	//if(done1){

	//}
}
void getDronePostion(int droneid){

}

int eyeinsky::getDronePixelWidth(UMat & img){
	double avgWidth = 0.0;
    double widest = 0.0;
    int iLowH = 38;
    int iHighH = 83;

    int iLowS = 50; 
    int iHighS = 255;

    int iLowV = 35;
    int iHighV = 255;
    
	
    Mat imgOriginal = img.getMat(ACCESS_RW);
	Mat imgHSV;

    cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

    Mat imgThresholded;

    inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

    //morphological opening (remove small objects from the foreground)
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

    //morphological closing (fill small holes in the foreground)
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    imshow("Thresholded Image", imgThresholded); //show the thresholded image
    //imshow("Original", imgOriginal); //show the original image

    int startRow = 0, endRow = 0; 
    double startRowWidth = 0.0, endRowWidth = 0.0;
    avgWidth = 0.0;
    int rowCount = 0;
    
    int widthTotal = 0;
    bool objFound = false;
    int currentWidth = 0;
	for(int r = 0; r < imgThresholded.size().height; r++) {
        currentWidth = 0;
        for(int c = 0; c < imgThresholded.size().width; c++) {
            RGB & color = imgThresholded.ptr<RGB>(r)[c];
            if((color.r > 200) && (color.g > 200 )&& (color.b > 200)) {
                // white-ish
                currentWidth++;
                }                   
            }
            if(currentWidth > widest) {
                widest = currentWidth;
                 
            }
            //cout << "widest: " << widest << endl;
            widthTotal += currentWidth;
    }
    return (int)widest;
            
}
eyeinsky::eyeinsky(int method,CommunicationBox & commBox)
{
	
	Matcher matcher;
   	UMat templImgOriginal,drone1,drone2,grid;
   	Mat dst1;
   	bool initalWidth = false;
    
   	imread("template.jpg", CV_LOAD_IMAGE_COLOR).copyTo(templImgOriginal);
    imread("drone1.JPG", CV_LOAD_IMAGE_COLOR).copyTo(drone1);
    imread("drone2.JPG", CV_LOAD_IMAGE_COLOR).copyTo(drone2);
    imread("grid.jpg", CV_LOAD_IMAGE_COLOR).copyTo(grid);
    cout << "Type: " << drone1.type()<<endl;
    cout << "hello world from the sky" << endl;
	libfreenect2::Freenect2 freenect2;
	libfreenect2::Freenect2Device *dev = 0;
	libfreenect2::PacketPipeline *pipeline = 0;
	Mat rgbmat, depthmat, depthmatUndistorted, irmat, rgbd, rgbd2;
	UMat urgbmat;
	signal(SIGINT,sigint_handler);
	cout << "Before Pipline" << endl;
	int depthProcessor = Processor::gl;

	if(depthProcessor == Processor::cpu)
	{
		if(!pipeline)
		    //! [pipeline]
		    pipeline = new libfreenect2::CpuPacketPipeline();
		    //! [pipeline]
	} else if (depthProcessor == Processor::gl) {
#ifdef LIBFREENECT2_WITH_OPENGL_SUPPORT
	if(!pipeline)
		pipeline = new libfreenect2::OpenGLPacketPipeline();
#else
		cout << "OpenGL pipeline is not supported!" << endl;
#endif
	} else if (depthProcessor == Processor::cl) {
#ifdef LIBFREENECT2_WITH_OPENCL_SUPPORT
	if(!pipeline)
	    	pipeline = new libfreenect2::OpenCLPacketPipeline();
#else
		cout << "OpenCL pipeline is not supported!" << endl;
#endif
	}
	cout << "After Pipline" << endl;
	if(freenect2.enumerateDevices() == 0)
	{
		cout << "no device connected!" << endl;
		exit(0);
	}
	string serial = freenect2.getDefaultDeviceSerialNumber();
	dev = freenect2.openDevice(serial, pipeline);
	//libfreenect2::SyncMultiFrameListener listener(libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);
	libfreenect2::SyncMultiFrameListener listener(libfreenect2::Frame::Color );
	
	libfreenect2::FrameMap frames;
	
	dev->setColorFrameListener(&listener);
	//dev->setIrAndDepthFrameListener(&listener);

	if (!dev->start()){
		exit(0);
	}
	libfreenect2::Registration* registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
  	libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4), depth2rgb(1920, 1080 + 2, 4);
	
    
	//Viewer viewer;
	//viewer.initialize();
	while(!protonect_shutdown)
  	{
		listener.waitForNewFrame(frames);
		libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
		//libfreenect2::Frame *ir = frames[libfreenect2::Frame::Ir];
		//libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];
		libfreenect2::Frame *depth = NULL;
		cv::Mat(rgb->height, rgb->width, CV_8UC4, rgb->data).copyTo(rgbmat);
	    //cv::Mat(ir->height, ir->width, CV_32FC1, ir->data).copyTo(irmat);
	    //cv::Mat(depth->height, depth->width, CV_32FC1, depth->data).copyTo(depthmat);   
		registration->apply(rgb, depth, &undistorted, &registered, true, &depth2rgb); 

		Size size(640,480);//the dst image size,e.g.100x100
		//UMat dst;//dst image
		//Mat src;//src image
		//resize(rgbmat,dst,size);//resize image
		//rgbmat.create(1920, 1080, CV_8UC3);
		//resize(rgbmat,dst,size);
		//urgbmat = rgbmat.getUMat( ACCESS_RW );
		
		
	    //cv::imshow("rgb", rgbmat);
	    //cv::imshow("ir", irmat / 4096.0f); 
	    //cv::imshow("depth", depthmat / 4096.0f);
		
		
		

		//cv::Mat(undistorted.height, undistorted.width, CV_32FC1, undistorted.data).copyTo(depthmatUndistorted);
        //cv::Mat(registered.height, registered.width, CV_8UC4, registered.data).copyTo(rgbd);
        //cv::Mat(depth2rgb.height, depth2rgb.width, CV_32FC1, depth2rgb.data).copyTo(rgbd2);


        //cv::imshow("undistorted", depthmatUndistorted / 4096.0f);
        //cv::imshow("registered", rgbd);
        //cv::imshow("depth2RGB", rgbd2 / 4096.0f);

        //cout << "Before Type: " << urgbmat.type()<<endl;
        commBox.skyFrame = rgbmat;
        resize(rgbmat,dst1,size);
        cv::imshow("rgb", dst1);
		urgbmat = dst1.getUMat( ACCESS_RW );
		if(!initalWidth){
			//wait for inital width picture
			eyeinsky::startPixel = getDronePixelWidth(urgbmat);
			initalWidth = true;
		}
		else{
			//get distance to drone
			cout << "Have Width" << endl;
			eyeinsky::newPixel = getDronePixelWidth(urgbmat);
			eyeinsky::newDist = Util::disttodrone(eyeinsky::startPixel,eyeinsky::startDist, eyeinsky::startWidth ,eyeinsky::newPixel);
		}
		cvtColor(urgbmat,dst2,CV_BGRA2BGR);
		Point tempPoint;
		//cout << "After Type: " << dst.type()<<endl;
		//matcher.templateMatchingWithoutCallBack(std::ref(templImgOriginal), std::ref(dst2), 1);//0r 4
		tempPoint = matcher.templateMatchingWithoutCallBack(std::ref(drone1), std::ref(dst2), method);//0r 1
		
		//tempPoint = matcher.templateMatchingWithoutCallBack(std::ref(grid), std::ref(dst2), method);//0r 1

		//commBox.drone1Loc()->bottomLeft = tempPoint;
		//cv::imshow("Drone Matching", dst2);
        //cout << "Type: " << rgbmat.type()<<endl;
		//cout<<"x= "<<tempPoint.x<<" y= "<<tempPoint.y<<endl;
		//Testing backgroundsubtraction	
        //testBackgrndSubtractor(rgbmat);
		int key = cv::waitKey(1);
	    protonect_shutdown = protonect_shutdown || (key > 0 && ((key & 0xFF) == 27));
		listener.release(frames);
		
	}

	dev->stop();
  	dev->close();
	
	delete registration;
    
}


//use kinect camera to take picture 

