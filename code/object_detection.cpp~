#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>


using namespace cv;
using namespace std;

int posX;
int posY; 

int main(int argc, char* argv[])
{
    VideoCapture cap ;
		cap.open(CV_CAP_OPENNI); // open the video camera no. 0

    if (!cap.isOpened())  // if not success, exit program
    {
        cout << "Cannot open the video cam" << endl;
        return -1;
    }
	
	    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
	int iLowH = 114;
    int iHighH = 179;

    int iLowS = 74; 
    int iHighS = 255;

    int iLowV = 45;
    int iHighV = 255;

	 //Create trackbars in "Control" window
 cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
 cvCreateTrackbar("HighH", "Control", &iHighH, 179);

  cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
 cvCreateTrackbar("HighS", "Control", &iHighS, 255);

  cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
 cvCreateTrackbar("HighV", "Control", &iHighV, 255);

  int iLastX = -1; 
 int iLastY = -1;

    //double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    //double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    //cout << "Frame size : " << dWidth << " x " << dHeight << endl;

	//Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));

   // namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
	
 
 
     Mat imgTmp;
 cap.retrieve( imgTmp, CV_CAP_OPENNI_BGR_IMAGE ) ;
 Mat imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );;

 for(;;)
    {
Mat depthMap;
        Mat validDepthMap;
        Mat disparityMap;
      Mat bgrImage;
        Mat grayImage;


      
		 if( !cap.grab() )
        {
            cout << "Can not grab images." << endl;
            return -1;
        }



		 
     Mat imgTmp;
 cap.retrieve( imgTmp, CV_CAP_OPENNI_BGR_IMAGE ) ;
 Mat imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );;



		 cap.retrieve( bgrImage, CV_CAP_OPENNI_BGR_IMAGE ) ;
             

		

		  if (waitKey(10) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break; 
       }
 

 


	Mat imgHSV;
	cvtColor(bgrImage, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

	 Mat imgThresholded;
	 inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
 
	 //morphological opening (remove small objects from the foreground)
	 erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
     dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );



	   //morphological closing (fill small holes in the foreground)
       dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
       erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );


	     //Calculate the moments of the thresholded image
      Moments oMoments = moments(imgThresholded);

      double dM01 = oMoments.m01;
      double dM10 = oMoments.m10;
      double dArea = oMoments.m00;


 // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
  if (dArea > 10000)
  {
   //calculate the position of the ball
    posX = dM10 / dArea;
    posY = dM01 / dArea;        
     // printf("x= %d,y=%d\n", posX,posY);  
   if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
   {
    //Draw a red line from the previous point to the current point
    line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0,0,255), 2);
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
   imshow("Thresholded Image", imgThresholded); //show the thresholded image


     bgrImage = bgrImage + imgLines;
  //imshow("Original", bgrImage); //show the original image



	    imshow("Thresholded Image", imgThresholded); //show the thresholded image
        imshow("Original",bgrImage ); //show the original image


		     if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
           // break; 
       }
 }

   return 0;

}
