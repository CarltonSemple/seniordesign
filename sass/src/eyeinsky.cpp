#include <iostream>
#include <cstdlib>
#include <signal.h>
#include "eyeinsky.h"
#include "backgroundSubtractor.h"
#include "human.h"
#include "persondetector.h"
#include "matcher.h"
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
using namespace cv;
using namespace std; 

enum Processor { cl, gl, cpu };

bool protonect_shutdown = false; ///< Whether the running application should shut down.



void sigint_handler(int s)
{
  protonect_shutdown = true;
}

eyeinsky::eyeinsky()
{
	cout << "hello world from the sky" << endl;
	libfreenect2::Freenect2 freenect2;
	libfreenect2::Freenect2Device *dev = 0;
	libfreenect2::PacketPipeline *pipeline = 0;
	Mat rgbmat, depthmat, depthmatUndistorted, irmat, rgbd, rgbd2;
	UMat templImgOriginal,urgbmat,dst;
    imread("template.jpg", CV_LOAD_IMAGE_COLOR).copyTo(templImgOriginal);
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
	libfreenect2::SyncMultiFrameListener listener(libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth);
	libfreenect2::FrameMap frames;
	
	dev->setColorFrameListener(&listener);
	dev->setIrAndDepthFrameListener(&listener);

	if (!dev->start()){
		exit(0);
	}
	libfreenect2::Registration* registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
  	libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4), depth2rgb(1920, 1080 + 2, 4);
	BackgrndSubtractor bs;
    
	//Viewer viewer;
	//viewer.initialize();
	while(!protonect_shutdown)
  	{
		listener.waitForNewFrame(frames);
		libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
		libfreenect2::Frame *ir = frames[libfreenect2::Frame::Ir];
		libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];

		cv::Mat(rgb->height, rgb->width, CV_8UC4, rgb->data).copyTo(rgbmat);
	    cv::Mat(ir->height, ir->width, CV_32FC1, ir->data).copyTo(irmat);
	    cv::Mat(depth->height, depth->width, CV_32FC1, depth->data).copyTo(depthmat);   
		registration->apply(rgb, depth, &undistorted, &registered, true, &depth2rgb); 

		Size size(640,480);//the dst image size,e.g.100x100
		UMat dst;//dst image
		//Mat src;//src image
		//resize(rgbmat,dst,size);//resize image
		//rgbmat.create(1920, 1080, CV_8UC3);
		//resize(rgbmat,dst,size);
		//urgbmat = rgbmat.getUMat( ACCESS_RW );
		
		// do hog
        //PersonDetector pd;
    	//pd.detectHumans(urgbmat, true);
        
        //Matcher matcher;
        //matcher.templateMatchingWithoutCallBack(std::ref(templImgOriginal), std::ref(urgbmat), 4);//0r 1
		
	    cv::imshow("rgb", rgbmat);
	    cv::imshow("ir", irmat / 4096.0f); 
	    cv::imshow("depth", depthmat / 4096.0f);
		
		
		

		cv::Mat(undistorted.height, undistorted.width, CV_32FC1, undistorted.data).copyTo(depthmatUndistorted);
        cv::Mat(registered.height, registered.width, CV_8UC4, registered.data).copyTo(rgbd);
        cv::Mat(depth2rgb.height, depth2rgb.width, CV_32FC1, depth2rgb.data).copyTo(rgbd2);


        cv::imshow("undistorted", depthmatUndistorted / 4096.0f);
        cv::imshow("registered", rgbd);
        cv::imshow("depth2RGB", rgbd2 / 4096.0f);
        //cout << "Before Type: " << urgbmat.type()<<endl;
        resize(rgbmat,dst,size);
		urgbmat = rgbmat.getUMat( ACCESS_RW );
		cvtColor(urgbmat,dst,CV_BGRA2BGR);
		//cout << "After Type: " << dst.type()<<endl;
		Matcher matcher;
        matcher.templateMatchingWithoutCallBack(std::ref(templImgOriginal), std::ref(dst), 4);//0r 1
        
        //cout << "Type: " << rgbmat.type()<<endl;
		int key = cv::waitKey(1);
	    protonect_shutdown = protonect_shutdown || (key > 0 && ((key & 0xFF) == 27));
		listener.release(frames);
		
	}

	dev->stop();
  	dev->close();
	
	delete registration;
}
/*void testTemplateMatchingVideo(UMat img)
{
    //cv::setNumThreads(4);
    //VideoCapture cap(cameraNumber);//CV_CAP_ANY);
    //cap.set(CV_CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH);
    //cap.set(CV_CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT);    
    //if (!cap.isOpened())
      //  return;
        
    //UMat img;
    UMat templImgOriginal;
    imread("template.jpg", CV_LOAD_IMAGE_COLOR).copyTo(templImgOriginal);
    //UMat templImg;
    //templImgOriginal.copyTo(templImg);
    
    //Matcher matcher;
    
    int i = 0;
    
    while(true)
    {
        //cap >> img;
        //if (!img.data)
          //  continue;
            
        namedWindow("hog", WINDOW_AUTOSIZE);
        if(i == 2) {
            i = 0;
            //imshow("hog", img);
            //continue;
        }
        i++;
            
        //UMat original;
        //img.copyTo(original);
        
        // do hog
        PersonDetector pd;
    	pd.detectHumans(img, true);
        
        Matcher matcher;
        matcher.templateMatchingWithoutCallBack(std::ref(templImgOriginal), std::ref(img), 4);//0r 1
        
        //////////hh.join();
        //imshow("hog", img);
        waitKey(1);
    }
}
*/