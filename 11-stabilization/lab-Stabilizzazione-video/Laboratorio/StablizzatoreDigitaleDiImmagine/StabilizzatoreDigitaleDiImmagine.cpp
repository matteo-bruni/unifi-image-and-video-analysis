
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include <fstream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <opencv2/opencv.hpp>
#include "opencv2/flann/flann.hpp"

#include "TrackPoint.h"

using namespace cv;
using namespace std;

#define PI 3.14159
#define sgn(x)  ( ((x) < 0) ?  -1 : ( ((x) == 0 ) ? 0 : 1) )

#define THRESHOLD 1000
#define WEIGHT_ERR 0.35 //alpha
#define TYPE_OF_DETECTOR "GFTT"

#define theta_threshold 3
#define CROP 0 // Crop the borders of the output video. We have crop with positive values (25 is a good value)

#define OUTPUT_COMPARE false //If true, compares the input video with the stabilized video.
#define IMSHOW false //If true, shows the output video during processing. If OUTPUT_COMPARE is true, it shows the input video next to the stabilized one
#define SAVE_OUTPUT false //If true, saves the stabilized videos. If OUTPUT_COMPARE is true, it saves the input video next to the stabilized one
#define SHOW_LOCAL_MOTION_VECTOR false//If true then show local motion vector on the current frame
#define STEP_BY_STEP false //If true, it shows the execution step by step
#define WAITms 0

#define auto_DELTA true //if true, dynamic deltas are used
float DELTAX=0.5;
float DELTAY=0.5;

#define not_threshold1 false
#define not_threshold2 false

void drawLocalMotionVectors(vector <vector <KeyPoint> >VVK, vector< DMatch > good_matches, Mat frame);
vector <Point2f> computeExpectedPoints(vector <TrackPoint> tp, vector <DMatch> good_matches, Mat out);

void computeErrors(vector<TrackPoint> &TP_new,vector<TrackPoint> &TP_old, vector<Point2f> expectedPoints, vector<DMatch> good_matches);
vector <TrackPoint> chooseBestPointsToTrack(vector <TrackPoint> TP_new, vector <DMatch> good_matches);

double distance2Points (int x1,int y1, int x2, int y2);
double distance2Points (int x1,int y1, float x2, float y2);

double angleError(int x1,int y1, float x2, float y2);

bool operator<(const TrackPoint & a, const TrackPoint & b);

Mat localMotionVectors;
int x_c=0;
int y_c=0;
bool order_by_norm=true;
double theta_glob=0;

string nameVideoInput="shaky";
Mat GMV_Prec=(Mat_<double>(4,1));


int main() {
	vector<KeyPoint> keypoints;
	vector<vector<KeyPoint> > VVK; 	//-- Used to display keypoints

	vector <TrackPoint> TP_old;
	vector <TrackPoint> TP_new;
	vector < DMatch > matches;
	Mat descriptorsPrec;
	Mat descriptors;
	Mat frame;

	Ptr<FeatureDetector> detector = FeatureDetector::create(TYPE_OF_DETECTOR);
	Ptr<DescriptorExtractor> extractor(Ptr<DescriptorExtractor>(new SiftDescriptorExtractor()));
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");

	stringstream pathVideoInput;
	pathVideoInput<<"../videos/"<<nameVideoInput<<".mp4";

	VideoCapture cap(pathVideoInput.str());
	VideoWriter outputVideo;

	Size S;
	if (OUTPUT_COMPARE==true)
		S = Size((int) cap.get(CV_CAP_PROP_FRAME_WIDTH)*2,
				(int) cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	else
		S = Size((int) cap.get(CV_CAP_PROP_FRAME_WIDTH),
				(int) cap.get(CV_CAP_PROP_FRAME_HEIGHT));

	stringstream nameVideoOutput;
    string c="noCrop";
    if (CROP>0)c="crop";
    Scalar value;

    string comp="noComp";
    stringstream d;

    if (OUTPUT_COMPARE==true)comp="Comp";
    if (auto_DELTA==false){d<<"DX"<<DELTAX<<"_DY"<<DELTAY;}else d<<"autoDelta";
    nameVideoOutput<<""<<nameVideoInput<<"_"<<TYPE_OF_DETECTOR<<THRESHOLD<<"_"<<d.str()<<"_"<<c<<"_"<<comp<<"_.avi";
    cout<<nameVideoOutput.str()<<endl;
    outputVideo.open(nameVideoOutput.str(),  CV_FOURCC('M','J','P','G'), cap.get(CV_CAP_PROP_FPS),S, true);

	if(!cap.isOpened())return -1;   //-- Check if we succeeded

//	for(int x=0; x<30; x++)cap >> frame; //-- Skip some frames

	//-- Elaborate the first frame
	cap >> frame;
	x_c=frame.cols/2;
	y_c=frame.rows/2;

	if(frame.empty())return -1;

	detector->detect( frame, keypoints );
	if(keypoints.size()>THRESHOLD) keypoints.resize(THRESHOLD);

	VVK.push_back(keypoints);

	extractor->compute( frame, keypoints, descriptors);

	for(int i=0; i<descriptors.rows; i++){
		TrackPoint tp=TrackPoint(keypoints[i],descriptors.row(i));
		TP_new.push_back(tp);
	}

	/*-- IMV:= Integrated Motion Vector is used to filter the cumulative motion curve
	 * 		   See below lines 338-340
	 */
	Mat IMV =(Mat_<double>(4,1));
	IMV.at<double>(0,0)=1;
	IMV.at<double>(1,0)=0;
	IMV.at<double>(2,0)=0;
	IMV.at<double>(3,0)=0;

	for(int x=0;;x++){
		cout<<"\t\t\tframe "<<x+1<<endl; //The current frame.
		descriptorsPrec=descriptors.clone();

		TP_old=TP_new;
		TP_new=vector<TrackPoint>();

		cap >> frame; //-- Get a new frame from video

		if(frame.empty())return -1;

		detector->detect( frame, keypoints );
		if(keypoints.size()>THRESHOLD) keypoints.resize(THRESHOLD);

		VVK.push_back(keypoints);

		extractor->compute( frame, keypoints, descriptors);

		for(int i=0; i<descriptors.rows; i++){
			TrackPoint tp=TrackPoint(keypoints[i],descriptors.row(i));
			TP_new.push_back(tp);
		}

		matcher->match(descriptors, descriptorsPrec,matches );

		if(STEP_BY_STEP==true){
			drawLocalMotionVectors(VVK, matches, frame);
			waitKey(WAITms);
		}

		double max_dist = 0; double min_dist = 50;
		double max_dist_eu = 0; double min_dist_eu = 100;

		//-- Quick calculation of max and min distances euclidean and of descriptors between keypoints
		for( int i = 0; i < descriptors.rows; i++ ){
			Point2f pT=TP_old[matches[i].trainIdx].keypoint.pt;
			Point2f pQ=TP_new[matches[i].queryIdx].keypoint.pt;
			double dist = matches[i].distance;
			if( dist < min_dist && dist>20)
				min_dist = dist;
			if( dist > max_dist )
				max_dist = dist;
			dist=distance2Points(pT.x,pT.y,pQ.x,pQ.y);
			if( dist < min_dist_eu && dist!=0)
				min_dist_eu = dist;
			if( dist > max_dist_eu )
				max_dist_eu = dist;
		}
		std::vector< DMatch > good_matches;

		//-- Consider only good matches for computation
		for( int i = 0; i < descriptors.rows; i++ ){
			Point2f pT=TP_old[matches[i].trainIdx].keypoint.pt;
			Point2f pQ=TP_new[matches[i].queryIdx].keypoint.pt;

			//-- Threshold to choose good matches
			if(not_threshold1!=true){
				if(matches[i].distance <= 5*min_dist && distance2Points(pT.x,pT.y,pQ.x,pQ.y)<20*min_dist_eu){
					good_matches.push_back( matches[i]);
					TP_new[i].matchIdx=matches[i].trainIdx;
				}
			}
			else{
				good_matches.push_back( matches[i]);
				TP_new[i].matchIdx=matches[i].trainIdx;
			}

		}

		if(SHOW_LOCAL_MOTION_VECTOR==true || STEP_BY_STEP==true){
			drawLocalMotionVectors(VVK, good_matches, frame);
			waitKey(WAITms);
		}

		Mat A=(Mat_<double>(2*good_matches.size(),4));
		Mat b=(Mat_<double>(2*good_matches.size(),1));

		int j=0;
		for(unsigned int i=0;i<2*good_matches.size();i++){
			//-- Fill the matrixes for LSM

			Point2f pT=TP_old[good_matches[j].trainIdx].keypoint.pt;
			Point2f pQ=TP_new[good_matches[j].queryIdx].keypoint.pt;
			A.at<double>(i,0)=pT.x;
			A.at<double>(i,1)=-pT.y;
			A.at<double>(i,2)=1;
			A.at<double>(i,3)=0;

			b.at<double>(i,0)=pQ.x;

			i++;

			A.at<double>(i,0)=pT.y;
			A.at<double>(i,1)=pT.x;
			A.at<double>(i,2)=0;
			A.at<double>(i,3)=1;

			b.at<double>(i,0)=pQ.y;

			j++;
		}

		if(A.rows>=4){

			Mat GMVTemp=(Mat_<double>(4,1));
			solve(A,b,GMVTemp,DECOMP_QR);

			vector <Point2f> expectedPoints = computeExpectedPoints(TP_old,good_matches,GMVTemp);

			if(STEP_BY_STEP==true){
				for(unsigned int i=0; i<expectedPoints.size();i++) {
					circle(localMotionVectors,expectedPoints[i],3,CV_RGB(0,255,0),1,8,0);
				}
				imshow( "localMotionVectors", localMotionVectors );
				waitKey(WAITms);
			}

			vector <TrackPoint> bestPointsToTrack;
			computeErrors(TP_new,TP_old,expectedPoints,good_matches);
			bestPointsToTrack=chooseBestPointsToTrack(TP_new,good_matches);
			computeErrors(TP_new,TP_old,expectedPoints,good_matches);

			bestPointsToTrack=chooseBestPointsToTrack(TP_new,good_matches);

			if(STEP_BY_STEP==true){
				//-- Draw all keypoints selected to compute the final GMV
				for(unsigned int i=0; i<bestPointsToTrack.size();i++) {
					circle(localMotionVectors,bestPointsToTrack[i].keypoint.pt,3,CV_RGB(255,255,255),1,8,0);
				}
				imshow( "localMotionVectors", localMotionVectors );
				waitKey(WAITms);
			}

			Mat A_final=(Mat_<double>(2*bestPointsToTrack.size(),4));//Mat A=(Mat_<double>(2*num_match,4));
			Mat b_final=(Mat_<double>(2*bestPointsToTrack.size(),1));

			int j=0;
			for(unsigned int i=0;i<2*bestPointsToTrack.size();i++){

				Point2f pT=TP_old[bestPointsToTrack[j].matchIdx].keypoint.pt;
				Point2f pQ=bestPointsToTrack[j].keypoint.pt;

				A_final.at<double>(i,0)=pT.x;
				A_final.at<double>(i,1)=-pT.y;
				A_final.at<double>(i,2)=1;
				A_final.at<double>(i,3)=0;

				b_final.at<double>(i,0)=pQ.x;

				i++;

				A_final.at<double>(i,0)=pT.y;
				A_final.at<double>(i,1)=pT.x;
				A_final.at<double>(i,2)=0;
				A_final.at<double>(i,3)=1;

				b_final.at<double>(i,0)=pQ.y;

				j++;
			}

			if(A_final.rows>=4){

				Mat GMV=(Mat_<double>(4,1));
				solve(A_final,b_final,GMV,DECOMP_QR);

				double lambda=sqrt(GMV.at<double>(0,0)*GMV.at<double>(0,0)+GMV.at<double>(1,0)*GMV.at<double>(1,0));

//				cout<<"GMV values: "<<endl;
//				cout<<"lambda\t"<<lambda<<endl;
//				cout<<"theta \t"<< asin(GMV.at<double>(1,0)/lambda)<<endl;
//				cout<<"Tx   \t"<<GMV.at<double>(2,0)<<endl;
//				cout<<"Ty   \t"<<GMV.at<double>(3,0)<<endl;

				Mat IMV_Prec=IMV.clone();
				lambda = sqrt(GMV.at<double>(0,0)*GMV.at<double>(0,0)+GMV.at<double>(1,0)*GMV.at<double>(1,0));
				double arg=GMV.at<double>(1,0)/lambda;
				if(arg>1)arg=1; if(arg<-1) arg=-1;
				double theta = asin(arg);
				/*--Deltas are weights for horizontal and vertical translations (Tx,Ty).
				 *  Their values depend on Global Motion Vector history.
				 *  If the sum of GMV and the previous one is low then we set a high delta, otherwise we set it lower.
				 */
				//-- Sum of previous angles of the scene.
				Mat GMV_sum=GMV+GMV_Prec;
//				cout<<" GMV SUM: "<<GMV_sum<<endl;
				if(auto_DELTA==true){
					DELTAX=1;
					DELTAY=1;
					if(abs(GMV_sum.at<double>(0,2))>5) DELTAX=0.75;
					if(abs(GMV_sum.at<double>(0,3))>5) DELTAY=0.75;
					if(abs(GMV_sum.at<double>(0,2))>10) DELTAX=0.45;
					if(abs(GMV_sum.at<double>(0,3))>10) DELTAY=0.45;
				}
				theta_glob+=theta;

				double theta_gr=theta_glob*180/PI;

				if (abs(theta_gr)>theta_threshold) //Threshold on theta.
					theta_gr=sgn(theta_gr)*theta_threshold;

				IMV.at<double>(0,2)=IMV.at<double>(0,2)*DELTAX;
				IMV.at<double>(0,3)=IMV.at<double>(0,3)*DELTAY;
				IMV=IMV+GMV;
				GMV_Prec=GMV;

				cout<<"IMV("<<x+1<<")"<<endl;
				cout<<"\tlambda\t"<<lambda<<endl;
				cout<<"\ttetha \t"<< theta_gr<<endl;
				cout<<"\tTx   \t"<<IMV.at<double>(0,2)<<endl;
				cout<<"\tTy   \t"<<IMV.at<double>(0,3)<<endl<<"________________________________"<<endl;

				//-- Apply the rotation to the frame to compensate the angle

				//Mat rot_mat=getRotationMatrix2D(Point2f(x_c,y_c),theta_glob*180/PI,1.0);

				Mat rot_mat;
				double a=20; double b=70;

//				cout<<"accumulated theta : "<<theta_gr<<endl;
				//For resize the frame on video roll.
				if (abs(theta_gr)<a||(theta_gr<a+180 && theta_gr>-a+180)||(theta_gr<a-180 && theta_gr>-a-180)){
					//cout<<"intervallo non scalato"<<endl;
					rot_mat= getRotationMatrix2D(Point2f(x_c,y_c),theta_gr,1.0);
				}
				else{
					if(abs(theta_gr)<b||(theta_gr<b+180 && theta_gr>-b+180)||(theta_gr<b-180 && theta_gr>-b-180)){
						double s;
						if (theta_gr>-90){
							s=(-(theta_gr+b)/(a-b)*(1-cap.get(CV_CAP_PROP_FRAME_HEIGHT)/cap.get(CV_CAP_PROP_FRAME_WIDTH))+cap.get(CV_CAP_PROP_FRAME_HEIGHT)/cap.get(CV_CAP_PROP_FRAME_WIDTH));
						}
						else {
							s=((180+theta_gr-b)/(a-b)*(1-cap.get(CV_CAP_PROP_FRAME_HEIGHT)/cap.get(CV_CAP_PROP_FRAME_WIDTH))+cap.get(CV_CAP_PROP_FRAME_HEIGHT)/cap.get(CV_CAP_PROP_FRAME_WIDTH));							//cout<<"<"<<-90-a<<" - theta="<<theta_gr<<" - s:"<<s<<endl;
						}
						cout<<"intervallo di scalatura: scala "<<s<<endl;
						rot_mat= getRotationMatrix2D(Point2f(x_c,y_c),theta_gr,abs(s));
					}
					else{
						cout<<"scala H/W"<<endl;
						rot_mat= getRotationMatrix2D(Point2f(x_c,y_c),theta_gr,cap.get(CV_CAP_PROP_FRAME_HEIGHT)/cap.get(CV_CAP_PROP_FRAME_WIDTH));
					}
				}

				Mat frameC=Mat::zeros( frame.rows, frame.cols, frame.type() );

				Point2f srcTri[3];
				Point2f dstTri[3];

				Mat warp_mat( 2, 3, CV_32FC1 );

				//-- Apply the translation to the frame to compensate
				srcTri[0] = Point2f( 0,0 );
				srcTri[1] = Point2f( frameC.cols, 0 );
				srcTri[2] = Point2f( 0, frameC.rows );

				double Tx=IMV.at<double>(2,0);
				double Ty=IMV.at<double>(3,0);
				float coeff= 2.f;
				Tx=round((float)Tx*coeff)/coeff;
				Ty=round((float)Ty*coeff)/coeff;

				dstTri[0] = Point2f( -Tx, -Ty );
				dstTri[1] = Point2f( frameC.cols-Tx, -Ty );
				dstTri[2] = Point2f( -Tx, +frameC.rows-Ty );

				warp_mat = getAffineTransform( srcTri, dstTri );

				warpAffine(frame, frameC, rot_mat, frameC.size());

				warpAffine( frameC, frameC, warp_mat, frameC.size() );

				//copyMakeBorder( frameC, frameC, top, bottom, left, right, BORDER_WRAP, Scalar( 0, 0, 0 ));
				line(frameC, Point(0,0), Point(cap.get(CV_CAP_PROP_FRAME_WIDTH),0), Scalar(0,0,0), CROP*2, 8, 0);
				line(frameC, Point(0,0), Point(0,cap.get(CV_CAP_PROP_FRAME_HEIGHT)), Scalar(0,0,0), CROP*2, 8, 0);
				line(frameC, Point(cap.get(CV_CAP_PROP_FRAME_WIDTH),0), Point(cap.get(CV_CAP_PROP_FRAME_WIDTH),cap.get(CV_CAP_PROP_FRAME_HEIGHT)), Scalar(0,0,0), CROP*2, 8, 0);
				line(frameC, Point(0,cap.get(CV_CAP_PROP_FRAME_HEIGHT)), Point(cap.get(CV_CAP_PROP_FRAME_WIDTH),cap.get(CV_CAP_PROP_FRAME_HEIGHT)), Scalar(0,0,0), CROP*2, 8, 0);

//				stringstream ss;
//				ss<<"#"<<TYPE_OF_DETECTOR<<":"<<THRESHOLD<<" - delta:"<<DELTA<<" - weight_err:"<<WEIGHT_ERR;
//				putText(frameC, ss.str(), cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,0,0), 1, CV_AA);

				if (OUTPUT_COMPARE==true){
					Mat frameCompare;
					vector <DMatch> dm;
					vector <KeyPoint> kp1, kp2;
					drawMatches(frame, kp1, frameC, kp2,
							dm, frameCompare, Scalar::all(-1), Scalar::all(-1),
							vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

					if (SAVE_OUTPUT==true)
						outputVideo<<frameCompare;
					if (IMSHOW==true){
						imshow( "Adjusted", frameCompare );
						waitKey(WAITms);
					}
				}
				else{
					if (SAVE_OUTPUT==true)
						outputVideo<<frameC;
					if (IMSHOW==true){
						imshow( "Adjusted", frameC );
						waitKey(WAITms);
					}
				}
			}
			else{
				IMV.at<double>(0,0)=1;
				IMV.at<double>(1,0)=0;
				IMV.at<double>(2,0)=0;
				IMV.at<double>(3,0)=0;

				GMV_Prec.at<double>(0,0)=0;
				GMV_Prec.at<double>(1,0)=0;
				GMV_Prec.at<double>(2,0)=0;
				GMV_Prec.at<double>(3,0)=0;
				theta_glob=0;
			}
		}
		VVK.pop_back();VVK.pop_back();
		VVK.push_back(keypoints);
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////
void drawLocalMotionVectors(vector <vector <KeyPoint> >VVK, vector< DMatch > good_matches, Mat frame){
	localMotionVectors=frame.clone();
	for(unsigned int i=0; i<good_matches.size();i++) {
		Point p0=cvPoint(VVK[0][good_matches[i].trainIdx].pt.x,VVK[0][good_matches[i].trainIdx].pt.y);
		Point p1=cvPoint(VVK[1][good_matches[i].queryIdx].pt.x,VVK[1][good_matches[i].queryIdx].pt.y);

		circle(localMotionVectors,p0,3,CV_RGB(255,0,0),1,8,0);
		circle(localMotionVectors,p1,3,CV_RGB(0,0,255),1,8,0);

		line(localMotionVectors, p0, p1, CV_RGB(255,255,255), 1, 1);
	}
	imshow( "localMotionVectors", localMotionVectors );
	waitKey(WAITms);
}
///////////////////////////////////////////////////////////////////////////////////////
vector <Point2f> computeExpectedPoints(vector <TrackPoint> tp, vector <DMatch> good_matches, Mat out){
	//-- Compute the expected points using the parameters got from LSM solution.
	vector <Point2f> expectedPoints;
	for(unsigned int i=0; i<good_matches.size();i++){
		Point2f pT=tp[good_matches[i].trainIdx].keypoint.pt;
		Point2f pE;
		pE.x=pT.x*out.at<double>(0,0)-pT.y*out.at<double>(1,0)+out.at<double>(2,0);
		pE.y=pT.x*out.at<double>(1,0)+pT.y*out.at<double>(0,0)+out.at<double>(3,0);
		expectedPoints.push_back(pE);
	}
	return expectedPoints;
}
///////////////////////////////////////////////////////////////////////////////////////
void computeErrors(vector<TrackPoint> &TP_new, vector<TrackPoint> &TP_old, vector<Point2f> expectedPoints, vector<DMatch> good_matches){
	// -- Compute the Distance Error and the Rotation Error between the real points and the expected points.
	for(unsigned int i=0;i<good_matches.size();i++){
		TP_new[good_matches[i].queryIdx].err_norm_cum=(1-WEIGHT_ERR)*TP_old[good_matches[i].trainIdx].err_norm_cum  +  WEIGHT_ERR*distance2Points(
				TP_new[good_matches[i].queryIdx].keypoint.pt.x,
				TP_new[good_matches[i].queryIdx].keypoint.pt.y,
				expectedPoints[i].x,expectedPoints[i].y);

		TP_new[good_matches[i].queryIdx].err_rot_cum=(1-WEIGHT_ERR)*TP_old[good_matches[i].trainIdx].err_rot_cum  +  WEIGHT_ERR*angleError(
						TP_new[good_matches[i].queryIdx].keypoint.pt.x-x_c,
						TP_new[good_matches[i].queryIdx].keypoint.pt.y-y_c,
						expectedPoints[i].x-x_c,expectedPoints[i].y-y_c);
	}
}
//////////////////////////////////////////////////////////////////////////////////////
vector <TrackPoint> chooseBestPointsToTrack(vector <TrackPoint> TP_new, vector <DMatch> good_matches){
	//-- Select the best points to track with the lowest errors
	vector <TrackPoint> bptt;

	vector <TrackPoint> TP_new_onlymatch;
	for (unsigned int i=0; i<TP_new.size();i++)if(TP_new[i].matchIdx!=-1)TP_new_onlymatch.push_back(TP_new[i]);
	if(not_threshold2==true) return TP_new_onlymatch;
	vector <TrackPoint> TP_new_orderbynorm=TP_new_onlymatch;
	vector <TrackPoint> TP_new_orderbyangle=TP_new_onlymatch;
	order_by_norm=true;
	std::sort(TP_new_orderbynorm.begin(),TP_new_orderbynorm.end());

	order_by_norm=false;
	std::sort(TP_new_orderbyangle.begin(),TP_new_orderbyangle.end());

	for (unsigned int i=0; i<TP_new_orderbynorm.size()/2;i++){
		for(unsigned int j=0; j<TP_new_orderbynorm.size()/2; j++){
			if(TP_new_orderbynorm[i].keypoint.pt==TP_new_orderbyangle[j].keypoint.pt){
				bptt.push_back(TP_new_orderbynorm[i]);
				break;
			}
		}
	}

	return bptt;
}
//////////////////////////////////////////////////////////////////////////////////////
double distance2Points (int x1,int y1, int x2, int y2){
	return 	sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}
//////////////////////////////////////////////////////////////////////////////////////
double distance2Points (int x1,int y1, float x2, float y2){
	return 	sqrt((x2-(float)x1)*(x2-(float)x1) + (y2-(float)y1)*(y2-(float)y1));
}
//////////////////////////////////////////////////////////////////////////////////////
double angleError(int x1,int y1, float x2, float y2){
	double t1=PI/2;
	double t2=PI/2;
	if ( x1 == 0 ){
		if ( y1 < 0 )
			t1 = -PI/2;
	}
	else t1=atan(y1/x1);
	if ( x2 == 0 ){
		if ( y2 < 0 )
			t2 = -PI/2;
	}
	else t2=atan(y2/x2);
	return abs(t1-t2);
}
//////////////////////////////////////////////////////////////////////////////////////
bool operator<(const TrackPoint & a, const TrackPoint & b) {

	if (order_by_norm==true && a.err_norm_cum != b.err_norm_cum)
		return a.err_norm_cum < b.err_norm_cum;
	if (order_by_norm==false && a.err_rot_cum != b.err_rot_cum)
		return a.err_rot_cum < b.err_rot_cum;


	return false;
}

