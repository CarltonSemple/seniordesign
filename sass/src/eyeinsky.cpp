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

Position eyeinsky::colorBlobDistanceCalibration(int iLowH, int iHighH, int iLowS, int iHighS, int iLowV, int iHighV
							, Mat imgOriginal, int droneId, CommunicationBox & commBox)
{
    
    long double avgWidth = 0.0;
    long double widest = 0.0;
    long double avgDist = 0;
    long double dist = 0;


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
   	if(droneId == 1){
    	//imshow("Thresholded Image1", imgThresholded); //show the thresholded image
    }
    if(droneId == 2){
    	//imshow("Thresholded Image2", imgThresholded); //show the thresholded image
    }
    //imshow("Original", imgOriginal); //show the original image

    
    //else if (key == 32) // space key is pressed 
    //{
    // calculate
    
    // iterate through the image, row by row from the bottom
    // first row with more than 5 white-ish pixels contains the object
    // last row with more than 5 white-ish pixels contains the end of object...
    int startRow = 0, endRow = 0; 
    double startRowWidth = 0.0, endRowWidth = 0.0;
    avgWidth = 0.0;
    int rowCount = 0;
    
    int widthTotal = 0;
    bool objFound = false;
    int currentWidth = 0;
    int tempRow =0;
    int tempCol =0;
    int firstofrow =0;
    int firstofcol =0;
    int lastofrow =0;
    int lastofcol=0;
    int midofrow=0;
    
    int startColumn = 0, endColumn = 0, largestStartCol = 0, largestEndCol = 0;
    int largestRow = 0;
    bool prevWhite = false;
    
    for(int r = 0; r < imgThresholded.size().height-1; r++) {
        currentWidth = 0;
        // look through all of the columns in this row
        for(int c = 0; c < imgThresholded.size().width-1; c++) {
            RGB & color = imgThresholded.ptr<RGB>(r)[c];
            if((color.r > 200) && (color.g > 200 )&& (color.b > 200)) {
                // white-ish
                if(prevWhite == false) {
                    if(c != 0) {
                        startColumn = c;
                    } // startColumn starts at 0
                }
                prevWhite = true;
                currentWidth++;
            } else {
                // make sure that the spot is the largest...
                if(prevWhite == true) {
                    endColumn = c-1;
                }
                
                avgWidth = (avgWidth+currentWidth)/2;
                if(currentWidth > widest) {
                	widest = currentWidth;
                    largestStartCol = startColumn;
                    largestEndCol = endColumn;
                    largestRow = r;
                }
                prevWhite = false;
                currentWidth = 0;
            }                   
        }
        
        //avgWidth = (avgWidth+currentWidth)/2;
        /*if(currentWidth > widest) {
            widest = currentWidth;
            
            firstofrow = tempRow;
            firstofcol = tempCol;
            lastofrow = firstofrow + widest;
            midofrow = firstofrow + (widest/2);
        }
        //cout << "widest: " << widest << endl;
        //cout << "Average width: " <<avgWidth <<endl;
        widthTotal += currentWidth;*/
    }
    
    /*cout << "startRow pixel: " << firstofrow << endl;
    cout << "middleRow pixel: " << midofrow << endl;
    cout << "endRow pixel: " << lastofrow << endl;
    cout << "startCol pixel: " << firstofcol << endl;*/
    //dist = Util::disttodrone(20, 487.68, 10.16 ,avgWidth);
    
    Position returnedPos;
    returnedPos.y2d = largestRow;
    returnedPos.x2d = imgThresholded.size().width - ((largestStartCol + largestEndCol) / 2);
    returnedPos.radius2d = abs((largestEndCol - largestStartCol));
    returnedPos.distanceDirect = dist;
    if(droneId == 1){
	    //cout << "Drone 1 X axis pixel: " << returnedPos.x2d << endl;
	    //cout << "Drone 1 Y axis pixel: " << returnedPos.y2d << endl;
	    if(commBox.desired_drone == 1){
		    if(returnedPos.y2d > commBox.bMark.y2d){
		    	//In my zone give me control
		    	cout << "Drone 1 is in control"<<endl;
		    	commBox.desired_drone = 1;
		    }
		    else if (returnedPos.y2d < commBox.bMark.y2d){
		    	//Not in my zone give up control and go home 
		    	cout << "Drone 2 take control"<<endl;
		    	commBox.desired_drone = 2;
		    }
		}
		else{
			cout <<"Drone 2 Has full control" << endl;
			if (returnedPos.y2d < commBox.bMark.y2d){
		    	// go home 
		    	cout << "Drone 1 Backing Off"<<endl;
		    	
		    }
		}
    }
    if(droneId == 2){
    	//cout << "Drone 2 X axis pixel: " << returnedPos.x2d << endl;
	    //cout << "Drone 2 Y axis pixel: " << returnedPos.y2d << endl;
	    if(commBox.desired_drone == 2){
		    if(returnedPos.y2d < commBox.bMark.y2d){
		    	//In my zone give me control
		    	cout << "Drone 2 is in control"<<endl;
		    	commBox.desired_drone = 2;

		    }
		    else if (returnedPos.y2d > commBox.bMark.y2d){
		    	//Not in my zone give up control and go home 
		    	cout << "Drone 1 take control"<<endl;
		    	commBox.desired_drone = 1;
		    }
		}
		else{
			cout <<"Drone 1 Has full control" << endl;
			if (returnedPos.y2d > commBox.bMark.y2d){
		    	// go home 
		    	cout << "Drone 2 Backing Off"<<endl;
		    	
		    }
		}
    }
	
	
    return returnedPos;
}

Position eyeinsky::getDronePosition(int droneId, Mat & img,CommunicationBox & commBox)
{
    Position dposition;
    
	/*Ranges
	iLowH = 0;
    iHighH = 179;

    iLowS = 0; 
    iHighS = 255;

    iLowV = 0;
    iHighV = 255;*/
    long double drone1dist;
    long double drone2dist;
	if(droneId == 1){
		//Blue Drone
		iLowH = 60;
    	iHighH = 178;
    	iLowS = 100;
    	iHighS = 255;
   		iLowV = 85;
   		iHighV = 255;
   		
	}
	else if(droneId == 2){
		//Green Drone
		iLowH = 46;
    	iHighH = 80;
    	iLowS = 11;
    	iHighS = 255;
   		iLowV = 186;
   		iHighV = 255;
	}
    dposition = colorBlobDistanceCalibration(iLowH, iHighH, iLowS, iHighS, iLowV, iHighV, img,droneId,commBox);

    return dposition;
}



eyeinsky::eyeinsky(int method,CommunicationBox & commBox)
{
	
	Matcher matcher;
	ObjectDetector od;
   	UMat j1,j2,j3,j4;
   	Mat dst1,flippeddst,tempdis,tempdis2,tempdis3;
   	bool initalWidth = false;
    Position bludposition;
    Position grndposition;
    bool testColor = true;
    int count =0;
    iLowH = 0;
	iHighH = 179;

	iLowS = 0; 
	iHighS = 255;

	iLowV = 0;
	iHighV = 255;
   	
    imread("img_set_1_0.jpg", CV_LOAD_IMAGE_COLOR).copyTo(j1);
    imread("img_set_1_1.jpg", CV_LOAD_IMAGE_COLOR).copyTo(j2);
    imread("img_set_1_2.jpg", CV_LOAD_IMAGE_COLOR).copyTo(j3);
    imread("img_set_1_3.jpg", CV_LOAD_IMAGE_COLOR).copyTo(j4);
    //cout << "Type: " << drone1.type()<<endl;
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
	/*
	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

    

    //Create trackbars in "Control" window
    createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
    createTrackbar("HighH", "Control", &iHighH, 179);

    createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    createTrackbar("HighS", "Control", &iHighS, 255);

    createTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    createTrackbar("HighV", "Control", &iHighV, 255);*/
	cout << "HERE" << endl;
	string serial = freenect2.getDefaultDeviceSerialNumber();
	dev = freenect2.openDevice(serial, pipeline);
	cout << "HERE" << endl;
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
		resize(rgbmat,dst1,size);
        //cout << "Before Type: " << urgbmat.type()<<endl;
        flip(rgbmat, flippeddst, 1);
        
        bludposition = getDronePosition(1,flippeddst,commBox);
        grndposition = getDronePosition(2,flippeddst,commBox);
        commBox.d1Mark = bludposition;
		commBox.d2Mark = grndposition;
        tempdis = Util::drawRectangleAroundDrone(flippeddst,Point(commBox.d1Mark.x2d,commBox.d1Mark.y2d),20,1);
        tempdis2 = Util::drawRectangleAroundDrone(tempdis,Point(commBox.d2Mark.x2d,commBox.d2Mark.y2d),20,2);
        tempdis3 = Util::drawLineAtBarrier(tempdis2,Point(commBox.bMark.x2d,commBox.bMark.y2d));
        
        urgbmat = tempdis3.getUMat( ACCESS_RW );
		cvtColor(urgbmat,dst2,CV_BGRA2BGR);
		//Hog detection
		//od.detectHumans(dst2, false, true, commBox);

		//Template Matching 
		//matcher.templateMatchingWithoutCallBack(std::ref(j1), std::ref(dst2), 1);//0r 4
		//matcher.templateMatchingWithoutCallBack(std::ref(j2), std::ref(dst2), 1);//0r 1
		//matcher.templateMatchingWithoutCallBack(std::ref(j3), std::ref(dst2), 1);//0r 1
		//matcher.templateMatchingWithoutCallBack(std::ref(j4), std::ref(dst2), 1);//0r 1
        commBox.skyFrame = dst1;
		//commBox.skyFrame = dst2.getMat(ACCESS_RW);
        //imshow("Both squares and Barrier Line",dst2);
        
        //cv::imshow("rgb", flippeddst);
		
    	//od.detectHumans(dst2, false, true);
		if(!initalWidth){
			//wait for inital width picture
			//eyeinsky::startPixel = getDronePixelWidth(urgbmat);
			initalWidth = true;
		}
		else{
			//get distance to drone
			//cout << "Have Width" << endl;
			//eyeinsky::newPixel = getDronePixelWidth(urgbmat);
			//eyeinsky::newDist = Util::disttodrone(eyeinsky::startPixel,eyeinsky::startDist, eyeinsky::startWidth ,eyeinsky::newPixel);
		}
		

		Point tempPoint;
		
		//cout << "After Type: " << dst.type()<<endl;
		//matcher.templateMatchingWithoutCallBack(std::ref(templImgOriginal), std::ref(dst2), 1);//0r 4
		//tempPoint = matcher.templateMatchingWithoutCallBack(std::ref(drone1), std::ref(dst2), method);//0r 1
		
		//tempPoint = matcher.templateMatchingWithoutCallBack(std::ref(grid), std::ref(dst2), method);//0r 1

		//commBox.drone1Loc()->bottomLeft = tempPoint;
		//cv::imshow("Img in CommBox", urgbmat);
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


//Color detection for drone

