#include <string>
#include <vector>
#include "test.h"
#include "backgroundSubtractor.h"
#include "drone.h"
#include "human.h"
#include "persondetector.h"
#include "matcher.h"
#include "scanner.h"
#include "util.h"
#include "eyeinsky.h"
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/ocl.hpp> // opencl
#include <cstdlib>
#include <unistd.h>
#include <thread>
#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/logger.h>
#include <signal.h>
using namespace std;
using namespace cv;

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480

int cameraNumber = 0;
int templatematchingmethod = 4; // 4 and 1 are best

enum Processor { cl, gl, cpu };

bool protonect_shutdown1 = false;

test::test()
{
	cout << "hello world" << endl;
}

void printHelp()
{
    cout << "enter the camera number when running" << endl;
}

// this is a test of cloud 9
void sigint_handler1(int s)
{
  protonect_shutdown1 = true;
}
int findAndSaveHumans(bool save, bool drawBoxes)
{
    PersonDetector detector;
    
    VideoCapture cap(cameraNumber);//CV_CAP_ANY);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT);    
    if (!cap.isOpened())
        return -1;
    
    vector<Human> cumulativeHumans;
    int size = cumulativeHumans.size();
    
    namedWindow("video capture", CV_WINDOW_NORMAL);
    Mat img;
    while (true)
    {
        cap >> img;
        if (!img.data)
            continue;
        // look for humans
        
        
        
        // FIX THIS LATER
        vector<Human> humans;// = detector.detectHumans(img, drawBoxes);
        
        
        
        
        cumulativeHumans.reserve(cumulativeHumans.size() + humans.size()); 
        cumulativeHumans.insert(cumulativeHumans.end(),humans.begin(),humans.end());
        if(cumulativeHumans.size() > size) {
            size = cumulativeHumans.size();
            cout << cumulativeHumans.size() << " humans so far" << endl;
        }        
        
        imshow("video capture", img);        
        char key = waitKey(20);
        /*
        if(key == 's') 
        {
            if(humans.size() > 0) {
                Human::saveHumanImagesToFiles(humans, "t");
                break;
            }
        }*/
        
        // Save the humans to files if this is desired
        if(save)
        {
            if(cumulativeHumans.size() > 0) {
                Human::saveHumanImagesToFiles(humans, "t");
            }
        }
    }
    
    return 0;
}

int Human::idNumber = 0;

int backgroundSubtractionTest()
{
    BackgrndSubtractor bs;
    bs.runIndependently();
    return 0;
}

/* Run video and use the person detector (HOG) to show humans and
   print how many surf similarities each "human" has to the scanned person */
void countSimilaritiesToScannedTarget(Human & target)
{
    PersonDetector detector;
    VideoCapture cap(cameraNumber);//CV_CAP_ANY);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT);    
    if (!cap.isOpened())
        return;
    
    UMat img;
    while(true)
    {
        cap >> img;
        //if (!img.data)
          //  continue;
            
        namedWindow("video capture", CV_WINDOW_NORMAL);
                
        vector<Human> detectedHumans = detector.detectHumans(img, false);
        // look at the humans and see the similarities
        for(Human hu : detectedHumans)
        {
            Matcher matcher;
            std::vector<cv::Mat> & hImages = hu.getImages();
            int a = 0;
            for(int p = 0; p < hu.getImageCount(); p++) 
            {
                //cout << "p = " << p << endl;
                a += matcher.surfCount(target, hImages[p]);
            }
            
            //a = a / target.getImageCount(); //hu.getImageCount();
            cout << "a = " << a << endl;
            
            // create rectangle around the person, with the color based
            // off of the # of matches
            
            int colorIntensity = 20 * a;
            
            cv::rectangle(img, hu.getTopLeftPoint(), hu.getBottomRightPoint(), cv::Scalar(colorIntensity,colorIntensity,colorIntensity), 2);
        }
        
        imshow("video capture", img);
        /*
        // display loaded images
        vector<cv::Mat> & selfies = target.getImages();
        //cout << selfies.size() << " selfies" << endl;
        for(int i = 0; i < selfies.size(); i++)
        {
            //cout << "image: " << i << endl;
            imshow("video capture", selfies[i]);//img);
            waitKey(10);
        }*/
        char key = waitKey(1);
    }
}

void printInitialMenu()
{
    cout << "s: scan new human" << endl;
    cout << "l: load previous scan" << endl;
}

void testCrop()
{
    VideoCapture cap(cameraNumber);//CV_CAP_ANY);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT);    
    if (!cap.isOpened())
        return;
    Mat img;
    while(true)
    {
        cap >> img;
        if (!img.data)
            continue;
            
        namedWindow("original", CV_WINDOW_NORMAL);
        namedWindow("crop test", CV_WINDOW_NORMAL);
        Mat cropped = Util::cropImage(img);
        imshow("original", img);
        imshow("crop test", cropped);
        char key = waitKey(1);
    }
}

void tmm(UMat & templImg, UMat & img)
{
    Matcher matcher;
    //matcher.templateMatchingWithoutCallBack(templImg, img, 1);
}

void hogg(UMat & img)
{
    PersonDetector pd;
    pd.detectHumans(img, true);
}

void testTemplateMatchingVideo()
{
    //cv::setNumThreads(4);
    VideoCapture cap(cameraNumber);//CV_CAP_ANY);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT);    
    if (!cap.isOpened())
        return;
        
    namedWindow("hog", WINDOW_AUTOSIZE);
        
    UMat img;
    UMat templImgOriginal;
    vector<cv::UMat> templateImages;
    // load the template image set
    
    std::ostringstream imgname;
    for(int i = 1; i <= 4; i++) {
        UMat tttt;
        imgname << "template" << i << ".jpg";
        UMat n;
        imread(imgname.str(), CV_LOAD_IMAGE_COLOR).copyTo(n);
        tttt = n.clone();
        imgname.clear();
        imgname.str("");
        templateImages.push_back(tttt);
    }
    //imread("template3.jpg", CV_LOAD_IMAGE_COLOR).copyTo(templImgOriginal);
    //templateImages.push_back(templImgOriginal);
    
    //UMat templImg;
    //templImgOriginal.copyTo(templImg);
    
    //Matcher matcher;
    
    int i = 0;
    
    while(true)
    {
        cap >> img;
        //if (!img.data)
          //  continue;
            
        
        if(i == 2) {
            i = 0;
            //imshow("hog", img);
            //continue;
        }
        i++;
            
        //UMat original;
        //img.copyTo(original);
        
        // do hog
        //hogg(std::ref(img));
                       
        Matcher matcher;
        //for(int i = 0; i < 4; i++)
        for(UMat temImg : templateImages) 
        {
            //UMat temImg = templateImages[i];
            // change color of squares for visualization purposes
            matcher.templateMatchingWithoutCallBack(std::ref(temImg), std::ref(img), templatematchingmethod);
        }
        
        //////////hh.join();
        imshow("hog", img);
        waitKey(1);
    }
}

void displayScan()
{
    Scanner s;
    int scanNum = 0;
    cout << "Enter scan number: ";
    cin >> scanNum;
    Human scannedHuman = s.loadScannedHuman(scanNum);
    scannedHuman.displayImages(true);
}


Mat publicImage;
bool run = true;

void publicImageLoop()
{
    VideoCapture cap(cameraNumber);//CV_CAP_ANY);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT);    
    if (!cap.isOpened())
        return;
    
    namedWindow("preview", CV_WINDOW_NORMAL);
    cv::resizeWindow("preview", 800, 800);
    while(true)
    {
        if(!run)
            break;
        cap >> publicImage;
        if(!publicImage.data)
            continue;
        imshow("preview", publicImage);
        waitKey(1);
    }
}
void kinectImageLoop()
{
    cout << "HERE" << endl;
    libfreenect2::Freenect2 freenect2;
    libfreenect2::Freenect2Device *dev = 0;
    libfreenect2::PacketPipeline *pipeline = 0;
    Mat rgbmat;
    
    namedWindow("preview", CV_WINDOW_NORMAL);
    cv::resizeWindow("preview", 800, 800);
    signal(SIGINT,sigint_handler1);
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
    libfreenect2::SyncMultiFrameListener listener(libfreenect2::Frame::Color );
    libfreenect2::FrameMap frames;
    
    dev->setColorFrameListener(&listener);

    if (!dev->start()){
        exit(0);
    }
    libfreenect2::Registration* registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());
    libfreenect2::Frame undistorted(512, 424, 4), registered(512, 424, 4), depth2rgb(1920, 1080 + 2, 4);
    
    while(!protonect_shutdown1)
    {
        listener.waitForNewFrame(frames);
        libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
        libfreenect2::Frame *depth = NULL;
        cv::Mat(rgb->height, rgb->width, CV_8UC4, rgb->data).copyTo(rgbmat);  
        registration->apply(rgb, depth, &undistorted, &registered, true, &depth2rgb); 

        Size size(640,480);
        publicImage = rgbmat;
        //commBox.skyFrame = rgbmat;
        //resize(rgbmat,dst1,size);
        cv::imshow("preview", rgbmat);
        
        int key = cv::waitKey(1);
        protonect_shutdown1 = protonect_shutdown1 || (key > 0 && ((key & 0xFF) == 27));
        listener.release(frames);
        
    }

    dev->stop();
    dev->close();
    
    delete registration;
}
void cameraApp(string filename)
{
    thread pp(kinectImageLoop);
    cout << "Enter any character to take a picture" << endl;
    char c;
    cin >> c;
    
    int seconds = 4;
    int microseconds = 1000000;
    usleep(microseconds);
    for(int i = seconds; i > 0; i--)
    {
        cout << i << "... ";
    }
    cout << endl << "snap!" << endl;
    Util::saveImage(filename, publicImage);
    run = false;
}


int main(int argc, char *argv[]) 
{
    //cv::ocl::setUseOpenCL(true); // enable OpenCL in the processing of UMat
    string fiii;
    Mat rgbmat;
    CommunicationBox commBox;
    //if(argc == 3) {
    //  fiii = argv[2];
    //} else 
    if(argc == 3) {
        cameraNumber = atoi(argv[1]);
        // meant for templatematching program - choose the method
        templatematchingmethod = atoi(argv[2]);
        eyeinsky eye(templatematchingmethod,commBox);
    } else if(argc == 2) {
        //cameraNumber = atoi(argv[1]);
        cout << "HERE0" << endl;
        fiii = argv[1];
        cout << "HERE1" << endl;
        cameraApp(fiii);
    } else {
        printHelp();
        //cout << "Enter the filename of the picture as the second argument" << endl;
        //cout << "Enter the method number (0-5) for the template matching" << endl;
        return -1;
    }
    
    //cameraApp(fiii);
    
    // Run Scanner
    //Scanner s;
    //s.runIndependently();
      
    //findAndSaveHumans(true, false);
    
    // Background Subtraction test
    //backgroundSubtractionTest();
    
    // Matching person test
    //Human scannedHuman = s.loadScannedHuman(5);
    //cout << "loaded " << scannedHuman.getImageCount() << " pictures" << endl;
    //countSimilaritiesToScannedTarget(scannedHuman);
    
    // Test matching with template matching
    //Mat srcImg = imread("s2.jpg", CV_LOAD_IMAGE_COLOR);
    //Mat templImg = imread("t2.jpg", CV_LOAD_IMAGE_COLOR);
    //Matcher matcher;
    //matcher.templateMatching(templImg, srcImg);
    
    // Test matching in video with template matching
    // and HOG
    //testTemplateMatchingVideo();    
    
    // Test cropping of image
    //testCrop();

    // Display Loaded Human
    //displayScan();
    
	return 0;
}
