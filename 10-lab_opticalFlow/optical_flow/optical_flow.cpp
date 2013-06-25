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
		/*printf("Usage: main <video-file-name>.avi\n\7");
		exit(0);*/
	}
	else{
		/* load the AVI file */
		capture = cvCaptureFromAVI( argv[1] );
	}

	/* always check */
	if( !capture ) return 1;
	/* get fps, needed to set the delay */
	int fps = ( int )cvGetCaptureProperty( capture, CV_CAP_PROP_FPS );


	/* display video */
	//cvNamedWindow( "video", 0 );
	IplImage* dst1;
	IplImage* dst2;
	IplImage* imgA_grey;
	IplImage* imgB_grey;

	imgA = cvQueryFrame( capture );
	CvSize img_sz = cvGetSize(imgA);

	cvNamedWindow("OpticalFlow1", CV_WINDOW_NORMAL);
	cvMoveWindow("OpticalFlow1",0,0);
	cvResizeWindow("OpticalFlow1",640,480);
	//cvShowImage("OpticalFlow1", imgA);

	//Initialize dst image
	dst1 = cvCreateImage(img_sz, imgA->depth, imgA->nChannels);
	cvCopy(imgA, dst1, NULL);
	dst2 = cvCreateImage(img_sz, imgA->depth, imgA->nChannels);
	cvCopy(imgA, dst2, NULL);

	imgA_grey = cvCreateImage(img_sz, IPL_DEPTH_8U, 1);
	cvConvertImage(imgA, imgA_grey);


	cvNamedWindow("OpticalFlow2", CV_WINDOW_NORMAL);
	cvMoveWindow("OpticalFlow2",660,0);
	cvResizeWindow("OpticalFlow2",640,480);

	/*** OBJECTS TO RECORD VIDEOS ***/

	// Change fps value to reproduce different time per frames

	VideoWriter filmatoRGB("OpticalFlowRecording.avi",CV_FOURCC('D','V','I','X'),fps,img_sz,true);
	VideoWriter filmatoRGB2("OpticalFlowRecording2.avi",CV_FOURCC('D','V','I','X'),fps,img_sz,true);

	/*********************************************************************/

	/** PARAMETERS FOR HORN-SCHUNCK **/

	double lambda1=0.01, lambda2=0.2;

	int max_iter1=50, max_iter2=100;

	/*******************************************************************/

	/** PARAMETERS FOR LUCAS-KANADE **/

	//Window dimension Lukas - Kanade
	// Tested with dim= 10,20,30, 50, 100
	int dim1=20,dim2=20;    // If window dimension is too small we have bad features
							 // If window dimension is too big   we haven't many features
							 // 	but the feature tracked are very reliable
							 // NOTE: Higer is the dimension lower is the speed of computation
	//Pyramid levels Lukas - Kanade
	// Tested with levels= 1,4, 6 ,8
	int levelsPyr1=1,levelsPyr2=1; // The result with a video without or with few noise
								   // are't so dissimilar using the KLT and manyLevelsPyramid
								   // In a noisy video
	//Minimal Corner Distance
	// Tested with distances=5.0, 6.0, 7.0 ,14.0
	float min_corner_distance1=6.0,min_corner_distance2=6.0;
								   // This parameter eliminates corner too close
								   // Higher is this parameter, Lower are the corner considered good
	//Minimal Corner Quality
	// Tested with distances=0.01, 0.02, 0.1 ,0.2
	float cornerQualityLevel1=0.01,cornerQualityLevel2=0.2;
								   // This parameter eliminates corner with
								   // Lambda<LambdaMAX*cornerQualityLevel
								   // So max value of cornerQualityLevel is 1
								   // and min value is 0 (tipically 0.02)


	/**********************************************************/

	int frequency = 20, i = 0;
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
			// scegliere due delle seguenti quattro righe per il calcolo in parallelo dei vari metodi

			//calcOpticalFlowPyrLK(imgA_grey, imgB_grey, dst1);
			/*calcOpticalFlowPyrLK(imgA_grey, imgB_grey, dst2);

			//if(i%frequency==frequency-1){
				calcOpticalFlowHS(imgA_grey, imgB_grey, dst2, 1);
			//}*/
			calcOpticalFlowHS(imgA_grey, imgB_grey, dst2, 1, 0.2);

			/*******************************************************************/
			filmatoRGB<<dst1;
			filmatoRGB2<<dst2;
			cvShowImage("OpticalFlow1", dst1);
			//if(i%frequency==frequency-1){
				cvShowImage("OpticalFlow2", dst2);
			//}
		//}
		i++;

	}
	filmatoRGB.release();
	filmatoRGB2.release();
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



	void calcOpticalFlowPyrLK(IplImage* prev, IplImage* curr, IplImage *flow, int window_size,int pyramid_levels,float min_corner_distance,float cornerQualityLevel) {

		int number_of_max_features = 200; //numero di features
		CvSize win_size = cvSize(window_size, window_size); //dimensione finestra quadrata usata da lucas kanade
		int level = pyramid_levels; //profondità piramidi
		char features_found[number_of_max_features]; //per ogni feature indica se è stato trovato il flusso ottico
		float feature_errors[number_of_max_features]; //
		CvTermCriteria term_criteria = cvTermCriteria(
				CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, .3); //criterio d'arresto
		int flags = 0;

		CvSize pyr_size = cvSize(prev->width + 8, prev->height / 3);
		CvSize img_size = cvSize(prev->width, prev->height);

		float diag_image = sqrt(
				flow->width * flow->width + flow->height * flow->height);

		IplImage* pyrA = cvCreateImage(pyr_size, IPL_DEPTH_32F, 1);
		IplImage* pyrB = cvCreateImage(pyr_size, IPL_DEPTH_32F, 1);

		IplImage* eig_image = cvCreateImage(img_size, IPL_DEPTH_32F, 1);
		IplImage* tmp_image = cvCreateImage(img_size, IPL_DEPTH_32F, 1);

		//features nel frame 1

		CvPoint2D32f frame1_features[number_of_max_features];

		//features da utilizzare nel tracking
		//float min_corner_distance=10.0;

		//float cornerQualityLevel=0.01;// It stands for the minimal Lambda of the corner
									  // considered good.
									  // If it's Lambda is bigger than LambdaMax*cornerQualityLevel
									  // than it's a good quality corner
									  // It varies between 0 and 1
									  // Higher than 0.05 cut some value

		cvGoodFeaturesToTrack(prev, eig_image, tmp_image, frame1_features,
				&number_of_max_features,cornerQualityLevel , min_corner_distance, 0, 3);

		CvPoint2D32f frame2_features[number_of_max_features];

		cvCalcOpticalFlowPyrLK(prev, curr, pyrA, pyrB, frame1_features,
				frame2_features, number_of_max_features, win_size, level,
				features_found, feature_errors, term_criteria, flags);

		float sum_dx = 0;
		float sum_dy = 0;
		int num_found = 0;
		//calcoliamo il flusso ottico
		for (int i = 0; i < number_of_max_features; i++) {

			//se non viene trovata una corrispondenza di features si salta l'iterazione
			if (features_found[i] == 0)
				continue;

			num_found = num_found + 1;

			CvPoint p0 = cvPoint(cvRound(frame1_features[i].x),
					cvRound(frame1_features[i].y));
			CvPoint p1 = cvPoint(cvRound(frame2_features[i].x),
					cvRound(frame2_features[i].y));

			sum_dx = sum_dx + (float) (p1.x - p0.x);
			sum_dy = sum_dy + (float) (p1.y - p0.y);

			draw_arrows(&flow, p0.x, p0.y, (float) (p1.x - p0.x),
					(float) (p1.y - p0.y), 0.01, diag_image, 1, CV_RGB(255,0,0),
					CV_RGB(0,255,0));

		}
		//flusso ottico medio centrato al centro del frame

	//	cout <<"flusso ottico medio: ("<<sum_dx/float(num_found)<<"),("<< sum_dy/float(num_found)<<")"<<endl;
		draw_arrows(&flow, (int) flow->width / 2, (int) flow->height / 2,
				sum_dx / float(num_found), sum_dy / float(num_found), 0.01,
				diag_image, 2, CV_RGB(0,0,255), CV_RGB(255,255,0));

		cvReleaseImage(&pyrA);
		cvReleaseImage(&pyrB);
		cvReleaseImage(&eig_image);
		cvReleaseImage(&tmp_image);

	}

	void calcOpticalFlowHS(IplImage* prev, IplImage* curr, IplImage* img_out, int type, double lambda, int max_iter) {

		int usePrevious = 0;
		//double lambda = 0.01; //lambda per pesare il vincolo di regolarità 0.002
		CvTermCriteria criteria;
		criteria.type = CV_TERMCRIT_ITER;
		criteria.max_iter = max_iter;

		int height = prev->height;
		int width = prev->width;

		IplImage *velx = cvCreateImage(cvSize(width, height), IPL_DEPTH_32F, 1);
		IplImage* vely = cvCreateImage(cvSize(width, height), IPL_DEPTH_32F, 1);

		IplImage* cvvelx = velx;
		IplImage* cvvely = vely;
		IplImage* cvprev = prev;
		IplImage* cvcurr = curr;
		cvCalcOpticalFlowHS(cvprev, cvcurr, usePrevious, cvvelx, cvvely, lambda,
				criteria);

		float min_threshold = 0;
		float max_threshold = sqrt(
				img_out->width * img_out->width
						+ img_out->height * img_out->height);
		int step = 8;

		if (type == 0) {

			for (int j = 0; j < img_out->height; j++) {
				float *px = (float*) (velx->imageData + j * velx->widthStep);
				float *py = (float*) (vely->imageData + j * vely->widthStep);
				for (int h = 0; h < img_out->width; h++) {

					colorMapping(px[h], py[h], j, h, img_out->width,
							img_out->height, img_out);

				}
			}
		} else {

			for (int y = 0; y < (img_out)->height; y += step) {
				float *px = (float*) (velx->imageData + y * velx->widthStep);
				float *py = (float*) (vely->imageData + y * vely->widthStep);
				for (int x = 0; x < (img_out)->width; x += step) {

					draw_arrows(&img_out, x, y, px[x], py[x], min_threshold,
							max_threshold, 1, CV_RGB(255,0,0), CV_RGB(0,255,0));

				}
			}

		}
		cvReleaseImage(&velx);
		cvReleaseImage(&vely);

	}

	// visualizza il campo del flusso ottico a video tramite una rappresentazione vettoriale
	void draw_arrows(IplImage** flow, int x, int y, float px, float py,
			float min_threshold, float max_threshold, int thickness,
			CvScalar colorLine, CvScalar colorHead) {

		float ip;
		float m1;
		float ang;
		int signx;
		int signy;
		float delta;
		float epsilon;
		CvPoint p0;
		CvPoint pA;
		CvPoint pB;

		if (sqrt(px * px + py * py) > min_threshold
				&& max_threshold > sqrt(px * px + py * py)) {

			if (px != 0 && py != 0) {
				ip = sqrt(px * px + py * py);
				//cout <<"px="<<px<<"py="<<py<<endl;
				m1 = abs(py) / abs(px);
				ang = atan(m1) * 180 / PI;
				signx = (px > 0) ? 1 : ((px < 0) ? -1 : 0);
				signy = (py > 0) ? 1 : ((py < 0) ? -1 : 0);

				p0 = cvPoint(x + signx * (4.0 / 5.0 * ip * cos(ang * PI / 180)),
						y + signy * (4.0 / 5.0 * ip * sin(ang * PI / 180)));

				delta = sqrt((ip / 10) * (ip / 10) * ((m1 * m1) / (1 + m1 * m1)));
				epsilon = delta / m1;

				pA = cvPoint(p0.x + signx * (delta), p0.y - signy * epsilon);
				pB = cvPoint(p0.x - signx * delta, p0.y + signy * epsilon);
			} else if (py == 0) {

				p0 = cvPoint(x + px * (4.0 / 5.0), y);
				delta = 0;
				epsilon = abs(px) / 10;
				pA = cvPoint(p0.x, p0.y + epsilon);
				pB = cvPoint(p0.x, p0.y - epsilon);
			} else if (px == 0) {

				p0 = cvPoint(x, y + py * (4.0 / 5.0));
				delta = abs(py) / 10;
				epsilon = 0;
				pA = cvPoint(p0.x - delta, p0.y);
				pB = cvPoint(p0.x + delta, p0.y);

			}

			cvLine(*flow, cvPoint(x, y), p0, colorLine, thickness, CV_AA);

			cvLine(*flow, pA, pB, colorHead, thickness, CV_AA);
			cvLine(*flow, pA, cvPoint(x + px, y + py), colorHead, thickness, CV_AA);
			cvLine(*flow, pB, cvPoint(x + px, y + py), colorHead, thickness, CV_AA);

		}

	}

	// attribuisce ad ogni pixel un colore differente a seconda della direzione e della intensità del flusso ottico
	void colorMapping(float dx, float dy, int j, int h, int frame_width,
			int frame_height, IplImage* output) {

		int threshold = 4;

		uchar* data = (uchar *) output->imageData;

		if (dx <= 0 && dy <= 0 && sqrt(dx * dx + dy * dy) > threshold) { //red
	//		data[j * output->widthStep + h * output->nChannels + 0] = (int) (sqrt(
	//				dx * dx + dy * dy) * 4.5);
			data[j * output->widthStep + h * output->nChannels + 0] = 255;
			data[j * output->widthStep + h * output->nChannels + 1] = 0;
			data[j * output->widthStep + h * output->nChannels + 2] = 0;
		} else if (dx >= 0 && dy <= 0 && sqrt(dx * dx + dy * dy) > threshold) { //blue
			data[j * output->widthStep + h * output->nChannels + 0] = 0;
	//		data[j * output->widthStep + h * output->nChannels + 1] = (int) (sqrt(
	//				dx * dx + dy * dy) * 4.5);
			data[j * output->widthStep + h * output->nChannels + 1] = 255;
			data[j * output->widthStep + h * output->nChannels + 2] = 0;

		} else if (dx >= 0 && dy >= 0 && sqrt(dx * dx + dy * dy) > threshold) { //green

			data[j * output->widthStep + h * output->nChannels + 0] = 0;
			data[j * output->widthStep + h * output->nChannels + 1] = 0;
	//		data[j * output->widthStep + h * output->nChannels + 2] = (int) (sqrt(
	//				dx * dx + dy * dy) * 4.5);
			data[j * output->widthStep + h * output->nChannels + 2] = 255;

		} else if (dx <= 0 && dy >= 0 && sqrt(dx * dx + dy * dy) > threshold) { //yellow
			data[j * output->widthStep + h * output->nChannels + 0] = 0;
	//		data[j * output->widthStep + h * output->nChannels + 1] = (int) (sqrt(
	//				dx * dx + dy * dy) * 4.5);
			data[j * output->widthStep + h * output->nChannels + 1] = 255;
			data[j * output->widthStep + h * output->nChannels + 2] = 255;

		} else { //white
			data[j * output->widthStep + h * output->nChannels + 0] = 255;
			data[j * output->widthStep + h * output->nChannels + 1] = 255;
			data[j * output->widthStep + h * output->nChannels + 2] = 255;

		}

	}
