//Includes all the headers necessary to use the most common public pieces of the ROS system.
#include <ros/ros.h>
//Use image_transport for publishing and subscribing to images in ROS
#include <image_transport/image_transport.h>
//Use cv_bridge to convert between ROS and OpenCV Image formats
#include <cv_bridge/cv_bridge.h>
//Include some useful constants for image encoding. Refer to: http://www.ros.org/doc/api/sensor_msgs/html/namespacesensor__msgs_1_1image__encodings.html for more info.
#include <sensor_msgs/image_encodings.h>
//Include headers for OpenCV Image processing
#include <opencv2/imgproc/imgproc.hpp>
//Include headers for OpenCV GUI handling
#include <opencv2/highgui/highgui.hpp>
 
//Store all constants for image encodings in the enc namespace to be used later.
namespace enc = sensor_msgs::image_encodings;
 
//Declare a string with the name of the window that we will create using OpenCV where processed images will be displayed.
static const char WINDOW[] = "Image Processed";
 
//Use method of ImageTransport to create image publisher
image_transport::Publisher pub;

cv::Mat imgTmp;
cv_bridge::CvImagePtr cv_ptr;

int posX;
int posY; 
 
//This function is called everytime a new image is published
void imageCallback(const sensor_msgs::ImageConstPtr& original_image)
{
    //Convert from the ROS image message to a CvImage suitable for working with OpenCV for processing
    
    try
    {
        //Always copy, returning a mutable CvImage
        //OpenCV expects color images to use BGR channel order.
        cv_ptr = cv_bridge::toCvCopy(original_image, enc::RGB8);
    }
    catch (cv_bridge::Exception& e)
    {
        //if there is an error during conversion, display it
        ROS_ERROR("tutorialROSOpenCV::main.cpp::cv_bridge exception: %s", e.what());
        return;
    }
 /*
    //Invert Image
    //Go through all the rows
    for(int i=0; i<cv_ptr->image.rows; i++)
    {
        //Go through all the columns
        for(int j=0; j<cv_ptr->image.cols; j++)
        {
            //Go through all the channels (b, g, r)
            for(int k=0; k<cv_ptr->image.channels(); k++)
            {
                //Invert the image by subtracting image data from 255               
                cv_ptr->image.data[i*cv_ptr->image.rows*4+j*3 + k] = 255-cv_ptr->image.data[i*cv_ptr->image.rows*4+j*3 + k];
            }
        }
    }
     
 */
imgTmp = cv_ptr->image;

cv::VideoCapture capture( CV_CAP_OPENNI );
 //cv::VideoCapture cap ;
   // cap.open(CV_CAP_OPENNI); // open the video camera no. 0


/*
    if (!capture.isOpened())  // if not success, exit program
    {
        ROS_INFO( "Cannot open the video cam" );
        return ;
    }
  */  
cv::namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

int iLowH = 114;
    int iHighH = 179;

    int iLowS = 74; 
    int iHighS = 255;

    int iLowV = 45;
    int iHighV = 255;


   cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
 cvCreateTrackbar("HighH", "Control", &iHighH, 179);

  cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
 cvCreateTrackbar("HighS", "Control", &iHighS, 255);

  cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
 cvCreateTrackbar("HighV", "Control", &iHighV, 255);

  int iLastX = -1; 
 int iLastY = -1;




     
 //cap.retrieve( imgTmp, CV_CAP_OPENNI_BGR_IMAGE ) ;

cv::Mat imgLines = cv::Mat::zeros( imgTmp.size(), CV_8UC3 );


 //for(;;)
   // {

    cv::Mat depthMap;
    cv::Mat validDepthMap;
    cv::Mat disparityMap;
    cv::Mat bgrImage;
    cv::Mat grayImage;

/*
 if( !cap.grab() )
        {
            ROS_INFO( "Can not grab images." );
            return ;
        }

*/

//cap.retrieve( imgTmp, CV_CAP_OPENNI_BGR_IMAGE ) ;
 //cv::Mat imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );;




 if (cv::waitKey(10) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            ROS_INFO( "esc key is pressed by user" );
            return; 
       }
 

  cv::Mat imgHSV;
  cvtColor(bgrImage, imgHSV, cv::COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

  cv::Mat imgThresholded;
   inRange(imgHSV, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image


    erode(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );
     dilate( imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );


//morphological closing (fill small holes in the foreground)
       dilate( imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) ); 
       erode(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );




  //Calculate the moments of the thresholded image
     cv::Moments oMoments = cv::moments(imgThresholded);

      double dM01 = oMoments.m01;
      double dM10 = oMoments.m10;
      double dArea = oMoments.m00;





  if (dArea > 10000)
  {
   //calculate the position of the ball
    posX = dM10 / dArea;
    posY = dM01 / dArea;        
     // printf("x= %d,y=%d\n", posX,posY);  
   if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
   {
    //Draw a red line from the previous point to the current point
    line(imgLines, cv::Point(posX, posY), cv::Point(iLastX, iLastY), cv::Scalar(0,0,255), 2);
   }

    iLastX = posX;
   iLastY = posY;
  }




if ( posX > 54 && posX< 431 && posY > 40 && posY < 328 )
{

    float newX = (posX - 54) * .11 ;
    float newY = (posY-40) *.11 ;
printf("x= %f,y=%f\n", newX,newY); 
  
}


cv::imshow("Thresholded Image", imgThresholded); //show the thresholded image


     bgrImage = bgrImage + imgLines;
  //imshow("Original", bgrImage); //show the original image




     cv::imshow("Thresholded Image", imgThresholded); //show the thresholded image
        cv::imshow("Original",bgrImage ); //show the original image













    //int width = imgTmp.cols;
    //ROS_INFO("%d", width);
    //Display the image using OpenCV
    //cv::imshow(WINDOW, imgTmp);
    //Add some delay in miliseconds. The function only works if there is at least one HighGUI window created and the window is active. If there are several HighGUI windows, any of them can be active.
    //cv::waitKey(3);
    /**
    * The publish() function is how you send messages. The parameter
    * is the message object. The type of this object must agree with the type
    * given as a template parameter to the advertise<>() call, as was done
    * in the constructor in main().
    */
    //Convert the CvImage to a ROS image message and publish it on the "camera/image_processed" topic.
        //pub.publish(cv_ptr->toImageMsg());


}
 
/**
* This tutorial demonstrates simple image conversion between ROS image message and OpenCV formats and image processing
*/
int main(int argc, char **argv)
{
    /**
    * The ros::init() function needs to see argc and argv so that it can perform
    * any ROS arguments and name remapping that were provided at the command line. For programmatic
    * remappings you can use a different version of init() which takes remappings
    * directly, but for most command-line programs, passing argc and argv is the easiest
    * way to do it.  The third argument to init() is the name of the node. Node names must be unique in a running system.
    * The name used here must be a base name, ie. it cannot have a / in it.
    * You must call one of the versions of ros::init() before using any other
    * part of the ROS system.
    */
        ros::init(argc, argv, "image_processor");
    /**
    * NodeHandle is the main access point to communications with the ROS system.
    * The first NodeHandle constructed will fully initialize this node, and the last
    * NodeHandle destructed will close down the node.
    */
        ros::NodeHandle nh;
    //Create an ImageTransport instance, initializing it with our NodeHandle.
        image_transport::ImageTransport it(nh);
    //OpenCV HighGUI call to create a display window on start-up.
    cv::namedWindow(WINDOW, CV_WINDOW_AUTOSIZE);
    /**
    * Subscribe to the "camera/image_raw" base topic. The actual ROS topic subscribed to depends on which transport is used. 
    * In the default case, "raw" transport, the topic is in fact "camera/image_raw" with type sensor_msgs/Image. ROS will call 
    * the "imageCallback" function whenever a new image arrives. The 2nd argument is the queue size.
    * subscribe() returns an image_transport::Subscriber object, that you must hold on to until you want to unsubscribe. 
    * When the Subscriber object is destructed, it will automatically unsubscribe from the "camera/image_raw" base topic.
    */
        image_transport::Subscriber sub = it.subscribe("camera/rgb/image_raw", 1, imageCallback);
    //OpenCV HighGUI call to destroy a display window on shut-down.
    


    //cv::destroyWindow(WINDOW);
    /**
    * The advertise() function is how you tell ROS that you want to
    * publish on a given topic name. This invokes a call to the ROS
    * master node, which keeps a registry of who is publishing and who
    * is subscribing. After this advertise() call is made, the master
    * node will notify anyone who is trying to subscribe to this topic name,
    * and they will in turn negotiate a peer-to-peer connection with this
    * node.  advertise() returns a Publisher object which allows you to
    * publish messages on that topic through a call to publish().  Once
    * all copies of the returned Publisher object are destroyed, the topic
    * will be automatically unadvertised.
    *
    * The second parameter to advertise() is the size of the message queue
    * used for publishing messages.  If messages are published more quickly
    * than we can send them, the number here specifies how many messages to
    * buffer up before throwing some away.
    */
        //pub = it.advertise("camera/image_processed", 1);


        
  //  int height = imgTmp.rows;

      //  ROS_INFO("%d", height);
    /**
    * In this application all user callbacks will be called from within the ros::spin() call. 
    * ros::spin() will not return until the node has been shutdown, either through a call 
    * to ros::shutdown() or a Ctrl-C.
    */
        ros::spin();
    //ROS_INFO is the replacement for printf/cout.
    ROS_INFO("tutorialROSOpenCV::main.cpp::No error.");

  
 
}