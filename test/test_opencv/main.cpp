
/*
 * load_camera_opencv.cpp
 */
#include <cv.h>
#include <highgui.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>



int main(int argc, char *argv[])
{
	CvCapture *capture = 0;
	IplImage *frame = 0;
	/* initialize camera */
	capture = cvCaptureFromCAM( 0 );

	/* always check */
	if ( !capture ) {
		printf("Cannot open initialize webcam!\n" );
		exit(0);
	}

	/* create a window for the video */
	cvNamedWindow( "result", CV_WINDOW_AUTOSIZE );

	/* exit if user press 'q' */
	while( (char)cvWaitKey(6) != 'q' ) {
		/* get a frame */
		frame = cvQueryFrame( capture );
		/* always check */
		if( !frame ) break;
		/* display current frame */
		cvShowImage( "result", frame );
	}

	/* free memory */
	cvDestroyWindow( "result" );
	cvReleaseCapture( &capture );
	return 0;
}
