#include "cvRelating.h"
cvRelating::cvRelating()
{
	
}

cvRelating::~cvRelating()
{

}

void cvRelating::LoadImage()
{
	this->L_Mat.Image = cv::Mat(cv::imread("d://qtguiapplication1/Picture 1.jpg", 1));
	this->R_Mat.Image = cv::Mat(cv::imread("d://qtguiapplication1/Picture 3.jpg", 1));
	this->ImageTransform();
}
void cvRelating::ImageTransform()
{
	cvtColor(this->L_Mat.Image, this->L_Mat.Image, CV_BGR2RGB);
	cvtColor(this->R_Mat.Image, this->R_Mat.Image, CV_BGR2RGB);
}
void cvRelating::FeatureDection()
{
	this->L_Mat.Dector = xfeatures2d::SiftFeatureDetector::create();
	this->L_Mat.Dector->detect(this->L_Mat.Image, this->L_Mat.keypoints);
	this->L_Mat.Dector->clear();
	this->L_Mat.Dector = xfeatures2d::SiftDescriptorExtractor::create();
	this->L_Mat.Dector->compute(this->L_Mat.Image, this->L_Mat.keypoints, this->L_Mat.Descriptor);
	drawKeypoints(this->L_Mat.Image, this->L_Mat.keypoints, this->L_Mat.Image, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	cvtColor(this->L_Mat.Image, this->L_Mat.Image, CV_BGR2RGB);
	this->L_Mat.Dector->clear();

	this->R_Mat.Dector = xfeatures2d::SiftFeatureDetector::create();
	this->R_Mat.Dector->detect(this->R_Mat.Image, this->R_Mat.keypoints);
	this->R_Mat.Dector->clear();
	this->R_Mat.Dector = xfeatures2d::SiftDescriptorExtractor::create();
	this->R_Mat.Dector->compute(this->R_Mat.Image, this->R_Mat.keypoints, this->R_Mat.Descriptor);
	drawKeypoints(this->R_Mat.Image, this->R_Mat.keypoints, this->R_Mat.Image, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	cvtColor(this->R_Mat.Image, this->R_Mat.Image, CV_BGR2RGB);
	this->R_Mat.Dector->clear();
}
void cvRelating::Matching()
{
	this->match.Mathcer.match(this->L_Mat.Descriptor, this->R_Mat.Descriptor, this->match.Matches);
	double max_dist = 0; double min_dist = 100;
	for (int i = 0; i < this->L_Mat.Descriptor.rows; i++)
	{
		double dist = this->match.Matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	qDebug() << min_dist << endl;
	qDebug() << max_dist << endl;
	for (int i = 0; i < this->L_Mat.Descriptor.rows; i++)
	{
		if (this->match.Matches[i].distance < 2 * min_dist)
		{
			this->match.GoodMat.push_back(this->match.Matches[i]);
		}
	}
	cv::drawMatches(this->L_Mat.Image, this->L_Mat.keypoints,this->R_Mat.Image, this->R_Mat.keypoints,
		this->match.GoodMat,this->MatchMat.Image, Scalar::all(-1), Scalar::all(-1), vector<char>(), 2);
	imshow("Testing", this->MatchMat.Image);
	for (int i = 0; i < this->match.GoodMat.size(); i++)
	{
		this->L_Mat.goodKeypoints.push_back(this->L_Mat.keypoints[this->match.GoodMat[i].queryIdx].pt);
		this->R_Mat.goodKeypoints.push_back(this->R_Mat.keypoints[this->match.GoodMat[i].queryIdx].pt);
	}
	this->FMatrix=findFundamentalMat(this->L_Mat.goodKeypoints, this->R_Mat.goodKeypoints,cv::FM_8POINT);
	cv::stereoRectifyUncalibrated(this->L_Mat.goodKeypoints, this->R_Mat.goodKeypoints, this->FMatrix, cv::Size(this->L_Mat.Image.cols, this->L_Mat.Image.rows), this->L_Mat.H_Matrix, this->R_Mat.H_Matrix);
}
void cvRelating::Rectify()
{
	cv::Mat R_L, R_R;
	cv::stereoRectifyUncalibrated(this->L_Mat.goodKeypoints, this->R_Mat.goodKeypoints, this->FMatrix, cv::Size(this->L_Mat.Image.cols, this->L_Mat.Image.rows), this->L_Mat.H_Matrix, this->R_Mat.H_Matrix);
	R_L = this->L_Model.cameraMatrix.inv() * this->L_Mat.H_Matrix * this->L_Model.cameraMatrix.inv();
	R_R = this->R_Model.cameraMatrix.inv() * this->R_Mat.H_Matrix * this->R_Model.cameraMatrix.inv();

	cv::initUndistortRectifyMap(this->L_Model.cameraMatrix, this->L_Model.distCoeffs,R_L, this->L_Model.cameraMatrix,cv::Size(this->L_Mat.Image.cols,this->L_Mat.Image.rows), CV_16SC2, this->L_uMat.Map1,
		this->L_uMat.Map2);
	cv::initUndistortRectifyMap(this->R_Model.cameraMatrix, this->R_Model.distCoeffs, R_R, this->R_Model.cameraMatrix, cv::Size(this->R_Mat.Image.cols, this->R_Mat.Image.rows), CV_16SC2, this->R_uMat.Map1,
		this->R_uMat.Map2);
}
void cvRelating::Correspondence()
{

}
unsigned int cvRelating::getWidth(unsigned int P)
{
	return (P == 1) ? this->L_Mat.Image.cols : this->R_Mat.Image.cols;
}
unsigned int cvRelating::getHeight(unsigned int P)
{
	return (P == 1) ? this->L_Mat.Image.rows : this->R_Mat.Image.rows;
}
unsigned int cvRelating::getChannel(unsigned int P)
{
	return (P == 1) ? this->L_Mat.Image.channels() : this->R_Mat.Image.channels();
}
unsigned char* cvRelating::getData(unsigned int P)
{
	return (P == 1) ? this->L_Mat.Image.data : this->R_Mat.Image.data;
}
void cvRelating::CameraCalibration()
{
	this->L_Model.cameraMatrix = (Mat_<float>(3, 3) << 1, 2, 3, 4, 5, 7, 8, 9, 10, 11);
}



