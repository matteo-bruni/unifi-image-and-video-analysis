//============================================================================
// Name        : optical_flow_opencv.cpp
// Author      : Vincenzo Varano
// Version     :
// Copyright   :
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <sstream>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/video/tracking.hpp>
#include "support_methods.cpp"

#define PI 3.14159

using namespace std;
using namespace cv;

/*************** metodi di calcolo del flusso ottico ***************/
void calcOpticalFlowHS(IplImage* prev, IplImage* curr, IplImage* flow, int type=1, double lambda=0.01, int max_iter=100);
void calcOpticalFlowPyrLK(IplImage* prev, IplImage* curr, IplImage* flow, int window_size=10,int pyramid_levels=4, float min_corner_distance=5.0, float cornerQualityLevel=0.01);

/*********************************************************************/

/*************** metodi di rappresentazione del flusso ottico ***************/
void draw_arrows(IplImage** flow, int x, int y, float px, float py,
		float min_threshold, float max_threshold,
		int thickness, CvScalar colorLine, CvScalar colorHead);
void colorMapping(float dx, float dy, int j, int h, int frame_width,
		int frame_height, IplImage* output);
/****************************************************************************/

int main(int argc, char *argv[])
{
	IplImage *imgA;
	IplImage *imgB;
	CvCapture *capture;
	/* supply the AVI file to play */
	if(argc<2){
		/* load from WebCam */
		capture = cvCaptureFromCAM( 0 );
	}
	else{
		/* load the AVI file */
		capture = cvCaptureFromAVI( argv[1] );
	}

	/* always check */
	if( !capture ) return 1;

	/* display video */
	IplImage* dst1;
	IplImage* dst2;
	IplImage* imgA_grey;
	IplImage* imgB_grey;

	imgA = cvQueryFrame( capture );
	CvSize img_sz = cvGetSize(imgA);

	cvNamedWindow("OpticalFlow1", CV_WINDOW_NORMAL);
	cvMoveWindow("OpticalFlow1",0,0);
	cvResizeWindow("OpticalFlow1",640,480);

	//Initialize dst# images
	dst1 = cvCreateImage(img_sz, imgA->depth, imgA->nChannels);
	cvCopy(imgA, dst1, NULL);
	dst2 = cvCreateImage(img_sz, imgA->depth, imgA->nChannels);
	cvCopy(imgA, dst2, NULL);

	imgA_grey = cvCreateImage(img_sz, IPL_DEPTH_8U, 1);
	cvConvertImage(imgA, imgA_grey);


	cvNamedWindow("OpticalFlow2", CV_WINDOW_NORMAL);
	cvMoveWindow("OpticalFlow2",660,0);
	cvResizeWindow("OpticalFlow2",640,480);

	/**********************************************************/

	while((char)cvWaitKey(5)!='q'){

		imgA_grey = cvCreateImage(img_sz, IPL_DEPTH_8U, 1);
		cvConvertImage(imgA, imgA_grey);

		imgB = cvQueryFrame( capture );
		cvWaitKey(5);
		if( !imgA || !imgB){
			exit(1);
		}
		imgB_grey = cvCreateImage(img_sz, IPL_DEPTH_8U, 1);
		cvConvertImage(imgB, imgB_grey);
		cvConvertImage(imgB, dst1);
		cvConvertImage(imgB, dst2);
		/*******************************************************************/

		calcOpticalFlowHS(imgA_grey, imgB_grey, dst2, 1, 0.2);

		/*******************************************************************/
		cvShowImage("OpticalFlow1", dst1);
		cvShowImage("OpticalFlow2", dst2);

	}
	cvReleaseImage(&dst1);
	cvReleaseImage(&dst2);
	cvReleaseImage(&imgA_grey);
	cvReleaseImage(&imgB_grey);
	cvReleaseImage(&imgA);
	cvReleaseImage(&imgB);
	cvDestroyAllWindows();

	/* free memory */
	cvReleaseCapture( &capture );
	return 0;
}
