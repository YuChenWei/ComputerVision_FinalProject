#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2\xfeatures2d\nonfree.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;
class cvRelating
{
public:
	cvRelating();
	~cvRelating();
	void CameraCalibration();
	void LoadImage();
	void ImageTransform();
	void FeatureDection();
	void Matching();
	void Rectify();
	void SaveFile(string, cv::Mat&);
	typedef struct matRelating
	{
		cv::Mat Image;
		cv::Mat Descriptor;
		cv::Mat H_Matrix;
		Ptr<Feature2D> Dector;
		std::vector<cv::KeyPoint> keypoints;
		std::vector<cv::Point2f> goodKeypoints;
	}MatRelating;
	typedef struct flann_matching
	{
		cv::FlannBasedMatcher Mathcer;
		std::vector< DMatch > Matches;
		std::vector< DMatch > GoodMat;
	}FlannMatch;
	typedef struct undistortionMat
	{
		cv::Mat Map1;
		cv::Mat Map2;
	}UndistortionMat;
	typedef struct cameraModel
	{
		cv::Mat cameraMatrix;
		cv::Mat distCoeffs;
	}CameraModel;
private:

	MatRelating L_Mat;
	MatRelating R_Mat;
	UndistortionMat L_uMat;
	UndistortionMat R_uMat;
	MatRelating MatchMat;
	cv::Mat CMatrix;
	cv::Mat FMatrix;
	cv::Mat dMatrix;
	FlannMatch match;
};