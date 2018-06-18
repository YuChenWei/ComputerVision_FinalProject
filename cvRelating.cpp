#include "cvRelating.h"
cvRelating::cvRelating()
{
	this->L_Mat.P_Matrix.convertTo(this->L_Mat.P_Matrix, CV_32FC1);
	this->L_Mat.R_Matrix.convertTo(this->L_Mat.R_Matrix, CV_32FC1);
	this->R_Mat.P_Matrix.convertTo(this->R_Mat.R_Matrix, CV_32FC1);
	this->L_Mat.R_Matrix.convertTo(this->R_Mat.P_Matrix, CV_32FC1);

	this->L_Camera.cameraMatrix.convertTo(this->L_Camera.cameraMatrix, CV_32FC1);
	this->L_Camera.distCoeffs.convertTo(this->L_Camera.distCoeffs, CV_32FC1);
	this->R_Camera.cameraMatrix.convertTo(this->R_Camera.cameraMatrix, CV_32FC1);
	this->R_Camera.distCoeffs.convertTo(this->R_Camera.distCoeffs, CV_32FC1);

	this->stereoParameters.EnssentialMatrix.convertTo(this->stereoParameters.EnssentialMatrix, CV_32FC1);
	this->stereoParameters.FundamentalMatrix.convertTo(this->stereoParameters.FundamentalMatrix, CV_32FC1);
	this->stereoParameters.RotaionMatrix.convertTo(this->stereoParameters.RotaionMatrix, CV_32FC1);
	this->stereoParameters.TranslantionMatrix.convertTo(this->stereoParameters.TranslantionMatrix, CV_32FC1);

	

	stereoCaculator = cv::StereoSGBM::create(-32, 96, 5, 600, 2400, 48, 4, 10,100, 1, StereoSGBM::MODE_HH);
}

cvRelating::~cvRelating()
{

}

void cvRelating::LoadImage()
{
	this->L_Mat.Image = imread("D:/OpenCV_Testing/CameraCalibrationV0.0/L16.jpg");
	this->R_Mat.Image = imread("D:/OpenCV_Testing/CameraCalibrationV0.0/R16.jpg");
	this->ImageTransform();
}
void cvRelating::ImageTransform()
{
	cvtColor(this->L_Mat.Image, this->L_Mat.Image, CV_RGB2GRAY);
	cvtColor(this->R_Mat.Image, this->R_Mat.Image, CV_RGB2GRAY);
}
void cvRelating::Rectify()
{
	stereoRectify(this->L_Camera.cameraMatrix, this->L_Camera.distCoeffs, this->R_Camera.cameraMatrix, this->R_Camera.distCoeffs, this->L_Mat.Image.size(),
		this->stereoParameters.RotaionMatrix, this->stereoParameters.TranslantionMatrix, this->L_Mat.R_Matrix, this->R_Mat.R_Matrix, this->L_Mat.P_Matrix, this->R_Mat.P_Matrix,
		this->Q_Mtrix, 0, -1, this->L_Mat.Image.size() , 0, 0);
	initUndistortRectifyMap(this->L_Camera.cameraMatrix, this->L_Camera.distCoeffs, this->L_Mat.R_Matrix, this->L_Mat.P_Matrix,this->L_Mat.Image.size(), CV_32FC1, this->L_Mat.Map1, this->L_Mat.Map2);
	initUndistortRectifyMap(this->R_Camera.cameraMatrix, this->R_Camera.distCoeffs, this->R_Mat.R_Matrix, this->R_Mat.P_Matrix,this->L_Mat.Image.size(), CV_32FC1, this->R_Mat.Map1, this->R_Mat.Map2);
	remap(this->L_Mat.Image, this->L_Mat.Image_R, this->L_Mat.Map1, this->L_Mat.Map2, cv::INTER_LINEAR);
	remap(this->R_Mat.Image, this->R_Mat.Image_R, this->R_Mat.Map1, this->R_Mat.Map2, cv::INTER_LINEAR);
	//imshow("Testing", this->L_Mat.Image_R);
	//imshow("Testing2", this->R_Mat.Image_R);
}
void cvRelating::StereoCalibration()
{
	this->L_Camera.cameraMatrix = (cv::Mat_<double>(3, 3) << 530.3463, 0, 316.8934, 0, 528.7529, 234.5534, 0, 0, 1);
	this->L_Camera.distCoeffs = (cv::Mat_<double>(1, 5) << -0.3438, 0.14, 0, 0, 0);
	this->R_Camera.cameraMatrix = (cv::Mat_<double>(3, 3) << 529.4051, 0, 357.73, 0, 527.0524, 237.7642, 0, 0, 1);
	this->R_Camera.distCoeffs = (cv::Mat_<double>(1, 5) << -0.3597, 0.1647, 0, 0, 0);

	this->stereoParameters.RotaionMatrix = (cv::Mat_<double>(3, 3) << 1, 0.0027,- 0.0069,- 0.0027, 1, 0.0045,0.0069,- 0.0045, 1);
	this->stereoParameters.TranslantionMatrix = (cv::Mat_<double>( 3,1) << -121.511, 0.939, -0.3216);
	this->stereoParameters.FundamentalMatrix = (cv::Mat_<double>(3, 3) << 0, 0, 0, 0, 0,-0.2306, 1.775*0.0001, 0.2314, -0.2771);
	this->stereoParameters.EnssentialMatrix = (cv::Mat_<double>(3, 3) << 0, -1.154, -0.4263, 0.322, 0.5334, -121.5091, 0.0924, 121.5047, 0.5496);

	SaveFile("RM.txt", this->stereoParameters.RotaionMatrix);
	SaveFile("TM.txt", this->stereoParameters.TranslantionMatrix);


}
void cvRelating::Disparty()
{
	this->stereoCaculator->compute(this->L_Mat.Image_R, this->R_Mat.Image_R, this->disp);
	
	normalize(disp, dispV,0, 255, cv::NORM_MINMAX, CV_8U);
	cv::applyColorMap(this->dispV, this->dispV, COLORMAP_JET);
	cv::imshow("Disparty", dispV);
 //  cv::reprojectImageTo3D(this->dispV, this->_3D, this->Q_Mtrix, false, -1);
	//cv::imshow("3D", this->_3D);
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