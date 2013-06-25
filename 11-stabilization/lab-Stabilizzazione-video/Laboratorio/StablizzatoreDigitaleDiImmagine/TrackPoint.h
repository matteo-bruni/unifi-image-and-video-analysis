/*
 * TrackPoint.h
 *
 *  Created on: 14/mag/2013
 *      Author: andrea
 */



#ifndef TRACKPOINT_H_
#define TRACKPOINT_H_

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/ml/ml.hpp"
#include <opencv2/opencv.hpp>
#include "opencv2/flann/flann_base.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/flann/flann.hpp"

using namespace cv;
using namespace std;

class TrackPoint {
public:
	TrackPoint(KeyPoint keypoint, Mat descriptor);
	virtual ~TrackPoint();

	KeyPoint keypoint;
	Mat descriptor;
	float err_norm_cum;		//errore cumulato calcolato sulla norm2 tra keypoint matchati (matched) e aspettati (expected)
	float err_rot_cum;		//errore cumulato calcolato sulla rotazione rispetto al centro del frame tra keypoint matchati (matched) e attesi (expected)

	int matchIdx;			//indirizzo del descrittore matchato del frame precedente
};

#endif /* TRACKPOINT_H_ */
