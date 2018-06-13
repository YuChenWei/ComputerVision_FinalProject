#include "cvRelating.h"
cvRelating::cvRelating()
{
	this->L_Mat.H_Matrix.convertTo(this->L_Mat.H_Matrix, CV_32FC1);
	this->R_Mat.H_Matrix.convertTo(this->R_Mat.H_Matrix, CV_32FC1);
	this->CMatrix.convertTo(this->CMatrix, CV_32FC1);
	this->FMatrix.convertTo(this->FMatrix, CV_32FC1);
	this->dMatrix.convertTo(this->dMatrix, CV_32FC1);
}

cvRelating::~cvRelating()
{

}

void cvRelating::LoadImage()
{
	cv::Mat TMP;
	this->L_Mat.Image = cv::Mat(cv::imread("d://qtguiapplication1/LeftImage.png", 1));
	this->R_Mat.Image = cv::Mat(cv::imread("d://qtguiapplication1/RightImage.png", 1));
	//cv::undistort(this->L_Mat.Image,TMP, this->CMatrix, this->dMatrix, noArray());
	//this->L_Mat.Image = TMP;
	//cv::undistort(this->R_Mat.Image, TMP, this->CMatrix, this->dMatrix, noArray());
	//this->R_Mat.Image = TMP;
	this->ImageTransform();
	//imshow("Testing", this->L_Mat.Image);
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
	for (int i = 0; i < this->L_Mat.Descriptor.rows; i++)
	{
		if (this->match.Matches[i].distance < 2 * min_dist)
		{
			this->match.GoodMat.push_back(this->match.Matches[i]);
		}
	}
	cv::drawMatches(this->L_Mat.Image, this->L_Mat.keypoints, this->R_Mat.Image, this->R_Mat.keypoints,
		this->match.GoodMat, this->MatchMat.Image, Scalar::all(-1), Scalar::all(-1), vector<char>(), 2);
	imshow("testing", this->MatchMat.Image);
	for (int i = 0; i < this->match.GoodMat.size(); i++)
	{
		this->L_Mat.goodKeypoints.push_back(this->L_Mat.keypoints[this->match.GoodMat[i].queryIdx].pt);
		this->R_Mat.goodKeypoints.push_back(this->R_Mat.keypoints[this->match.GoodMat[i].queryIdx].pt);
	}
	this->FMatrix = findFundamentalMat(this->L_Mat.goodKeypoints, this->R_Mat.goodKeypoints,FM_RANSAC,1.0,0.9);
	SaveFile("F.txt", this->FMatrix);
}
void cvRelating::Rectify()
{
	cv::Mat R_L, R_R,TMP,TMP2,TMP3;
	R_L.convertTo(R_L, CV_32FC1);
	R_R.convertTo(R_R, CV_32FC1);
	TMP.convertTo(TMP, CV_32FC1);

	cv::stereoRectifyUncalibrated(this->L_Mat.goodKeypoints, this->R_Mat.goodKeypoints, this->FMatrix, this->L_Mat.Image.size(), this->L_Mat.H_Matrix, this->R_Mat.H_Matrix,3);
	R_L = this->CMatrix.inv() * this->L_Mat.H_Matrix * this->CMatrix;
	R_R = this->CMatrix.inv() * this->R_Mat.H_Matrix * this->CMatrix;
	SaveFile("R_L.txt", R_L);
	SaveFile("R_R.txt", R_R);
	SaveFile("C.txt", this->CMatrix);
	TMP = this->CMatrix.inv();
	SaveFile("CI.txt", TMP);

	
	cv::initUndistortRectifyMap(this->CMatrix, this->dMatrix, R_L, TMP3, this->L_Mat.Image.size() , CV_16SC2, this->L_uMat.Map1,
		this->L_uMat.Map2);
	cv::initUndistortRectifyMap(this->CMatrix, this->dMatrix,R_R,TMP2, this->R_Mat.Image.size(), CV_16SC2, this->R_uMat.Map1,
		this->R_uMat.Map2);
	cv::remap(this->L_Mat.Image, TMP3, this->L_uMat.Map1, this->L_uMat.Map2, cv::INTER_LINEAR);
	SaveFile("F1.txt", this->FMatrix);
	imshow("testing2", TMP3);
	cv::remap(this->R_Mat.Image, TMP2, this->R_uMat.Map1, this->R_uMat.Map2, cv::INTER_LINEAR);
	imshow("TESTING3", TMP2);
	//TMP = FMatrix.inv();
	//SaveFile("FI.txt", TMP);
	//SaveFile("D.txt", this->dMatrix);
	
}
void cvRelating::CameraCalibration()
{
	this->CMatrix = (cv::Mat_<double>(3, 3) << 1383.1, 0, 618.9, 0, 1383.7, 504.3504, 0, 0, 1);
	SaveFile("F1.txt", this->CMatrix);
	this->dMatrix = (cv::Mat_<double>(1, 5) << 0.1228, -0.0399, 0, 0, 0);
	SaveFile("D.txt", this->dMatrix);
}
void cvRelating::SaveFile(string fileName, cv::Mat& mat)
{
	ofstream outstream(fileName);
	if (outstream)
	{
		uint16_t rows = mat.rows;
		uint16_t columns = mat.cols;
		cout << rows << endl;
		cout << columns << endl;
		for (int r = 0; r < rows; r++)
		{
			for (int c = 0; c < columns; c++)
			{
				double value = mat.at<double>(r, c);
				outstream << value << " ";
			}
			outstream << endl;
		}
		outstream.close();
	}
}