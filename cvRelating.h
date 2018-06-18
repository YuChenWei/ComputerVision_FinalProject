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
	void ImageTransform();
	void StereoCalibration();
	void LoadImage();
	void Rectify();
	void Disparty();
	void SaveFile(string, cv::Mat&);
	typedef struct matRelating
	{
		cv::Mat Image;
		cv::Mat R_Matrix;
		cv::Mat P_Matrix;
		cv::Mat Map1;
		cv::Mat Map2;
		cv::Mat Image_R;
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
	typedef struct recifyParameter
	{
		cv::Mat RotaionMatrix;
		cv::Mat TranslantionMatrix;
		cv::Mat EnssentialMatrix;
		cv::Mat FundamentalMatrix;
	}RectifyParameter;
private:

	MatRelating L_Mat;
	MatRelating R_Mat;
	CameraModel L_Camera;
	CameraModel R_Camera;
	RectifyParameter stereoParameters;
	cv::Mat Q_Mtrix;
	cv::Mat _3D;
	cv::Ptr<cv::StereoSGBM> stereoCaculator;
	cv::Mat disp;
	cv::Mat dispV;
};