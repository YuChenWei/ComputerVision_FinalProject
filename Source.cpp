#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\calib3d.hpp>
#include <iostream>
#include <stdio.h>
#include "cvRelating.h"
using namespace cv;
using namespace std;
cvRelating CR;
int main(int argv, char** argc)
{
	CR.CameraCalibration();
	//CR.LoadImages();
	//CR.FeatureDection();
	//CR.FeatureMatching();
	//CR.FundamentalEpipolar();
	CR.LoadImage();
	CR.FeatureDection();
	CR.Matching();
	CR.Rectify();

	cv::waitKey(0);
	return 0;
	
}