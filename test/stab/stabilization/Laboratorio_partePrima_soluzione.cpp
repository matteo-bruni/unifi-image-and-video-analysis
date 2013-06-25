//============================================================================
// Name        : LaboratorioStabilizzatoreDigitale.cpp
// Author      : Andrea Salvi e Alessandro Venturi
//============================================================================

#include <iostream>

#include "opencv2/highgui/highgui.hpp"		/*definisce una serie di funzioni per l’I/O e per visualizzare su
											finestre le immagini o un video; si occupa in pratica della parte
											user-interface.*/
#include "opencv2/imgproc/imgproc.hpp" 		/*come dice il nome stesso, libreria con le funzioni di Image Processing;
											ciò significa che se dovete applicare un filtro ad una immagine o una
											trasformazione geometrica, questa è la libreria che dovete linkare.*/
#include "opencv2/features2d/features2d.hpp"/*contiene tutte le funzioni per estrarre descrittori dalle immagini
											per effettuare confronti tra queste a seconda delle caratteristiche
											selezionate.*/
//#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/flann/flann.hpp"			/*contiene le funzioni per la ricerca approssimata*/
#include "opencv2/core/core.hpp"			/* libreria principale di OpenCV; contiene tutte le
											strutture dati e le funzioni di base per lavorare sulle immagini.*/

//#include <opencv2/nonfree/features2d.hpp>
//#include <opencv2/legacy/legacy.hpp>


using namespace std;
using namespace cv;

#define NUM_KEYPOINTS 200
#define WAIT_MS 0

int main() {

	//struttura per caricare il video
	VideoCapture capture("aula.mp4");

	Ptr<FeatureDetector> detector = FeatureDetector::create("GFTT");
	Ptr<cv::DescriptorExtractor> extractor(Ptr<DescriptorExtractor>(new SiftDescriptorExtractor()));
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");

	Mat frame;
	Mat framePrec;

	vector <KeyPoint> keypoints;
	vector <KeyPoint> keypointsPrec;

	Mat descriptors;
	Mat descriptorsPrec;

	vector<DMatch> matches;

	capture >> frame;

	//passo base sul primo frame
	//trova i keypoint tramite GFTT
	detector->detect( frame, keypoints );
	if(keypoints.size()>NUM_KEYPOINTS) keypoints.resize(NUM_KEYPOINTS);
	//estrae i descrittori SIFT dei keypoint trovati al passo precedente
	extractor->compute( frame, keypoints, descriptors);

	for (;;) {

		//le strutture dell'iterazione precedente diventano istanze Prec
		framePrec=frame.clone();
		descriptorsPrec=descriptors.clone();
		keypointsPrec=keypoints;

		//grab del frame
		capture >> frame;
		if (frame.empty()) break;

		//troviamo i key point col GFTT
		detector->detect( frame, keypoints );
		if(keypoints.size()>NUM_KEYPOINTS) keypoints.resize(NUM_KEYPOINTS);

		//disegnamo i keypoint del frame corrente
		for (int i = 0; i < keypoints.size(); i++){
			circle(frame,keypoints[i].pt,3,Scalar(0,0,255),1,8,0); //BGR
		}
		imshow("KeyPoint",frame);
		waitKey(WAIT_MS);

		//estraiamo i descrittori SIFT dai keypoint
		extractor->compute( frame, keypoints, descriptors);

		//fare i match con i descrittori del frame precedente con quello attuale
		matcher->match(descriptorsPrec, descriptors ,matches );

		//disenga i match
		Mat out;
		drawMatches(framePrec,keypointsPrec,frame,keypoints,matches,out,Scalar::all(-1), Scalar::all(-1),Mat(),0);
		imshow("Matches",out);

		waitKey(WAIT_MS);
	}


	return 0;
}
