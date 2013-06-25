/*
 * TrackPoint.cpp
 *
 *  Created on: 14/mag/2013
 *      Author: andrea
 */

#include "TrackPoint.h"

TrackPoint::TrackPoint(KeyPoint keypoint, Mat descriptor) {
	this->keypoint=keypoint;
	this->descriptor=descriptor;
	err_norm_cum=0;
	err_rot_cum=0;
	matchIdx=-1;
}

TrackPoint::~TrackPoint() {
	// TODO Auto-generated destructor stub
}

