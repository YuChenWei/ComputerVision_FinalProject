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
	CR.StereoCalibration();
	CR.LoadImage();
	CR.Rectify();
	CR.Disparty();
	cv::waitKey(0);
	return 0;
	
}