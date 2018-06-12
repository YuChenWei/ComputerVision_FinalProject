#include "QtGuiApplication1.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

QtGuiApplication1::QtGuiApplication1(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	/*this->scene = new QGraphicsScene;*/
	this->L_Scene.scene = new QGraphicsScene;
	this->R_Scene.scene = new QGraphicsScene;

	this->cvR = new cvRelating;

	
	connect(this->ui.pushButton, SIGNAL(clicked()), this, SLOT(CoverCVImageToQImage()));
	connect(this->ui.pushButton_2, SIGNAL(clicked()), this, SLOT(SIFT()));
	connect(this->ui.pushButton_3, SIGNAL(clicked()), this, SLOT(GetEpipolar()));
	connect(this->ui.pushButton_4, SIGNAL(clicked()), this, SLOT(CameraCalibration()));
	//this->CoverCVImageToQImage();
	

}
QtGuiApplication1::~QtGuiApplication1()
{
	delete cvR;
}
void QtGuiApplication1::CoverCVImageToQImage()
{
	this->cvR->LoadImage();


	this->L_Scene.Img=new QImage((const unsigned char*)(this->cvR->getData(1)),
		this->cvR->getWidth(1),this->cvR->getHeight(1),
		this->cvR->getWidth(1)*this->cvR->getChannel(1),
		QImage::Format_RGB888);
	this->L_Scene.scene->addPixmap(QPixmap::fromImage(*this->L_Scene.Img));
	this->ui.graphicsView->setScene(this->L_Scene.scene);
	this->ui.graphicsView->show();

	this->R_Scene.Img = new QImage((const unsigned char*)(this->cvR->getData(2)),
		this->cvR->getWidth(2), this->cvR->getHeight(2),
		this->cvR->getWidth(2)*this->cvR->getChannel(2),
		QImage::Format_RGB888);
	this->R_Scene.scene->addPixmap(QPixmap::fromImage(*this->R_Scene.Img));
	this->ui.graphicsView_3->setScene(this->R_Scene.scene);
	this->ui.graphicsView_3->show();
}
void QtGuiApplication1::SIFT()
{
	this->cvR->FeatureDection();


	this->L_Scene.Img =new QImage((const unsigned char*)(this->cvR->getData(1)),
		this->cvR->getWidth(1), this->cvR->getHeight(1),
		this->cvR->getWidth(1)*this->cvR->getChannel(1),
		QImage::Format_RGB888);
	this->L_Scene.scene->addPixmap(QPixmap::fromImage(*this->L_Scene.Img));
	this->ui.graphicsView->setScene(this->L_Scene.scene);
	this->ui.graphicsView->show();

	this->R_Scene.Img = new QImage((const unsigned char*)(this->cvR->getData(2)),
		this->cvR->getWidth(2), this->cvR->getHeight(2),
		this->cvR->getWidth(2)*this->cvR->getChannel(2),
		QImage::Format_RGB888);
	this->R_Scene.scene->addPixmap(QPixmap::fromImage(*this->R_Scene.Img));
	this->ui.graphicsView->setScene(this->R_Scene.scene);
	this->ui.graphicsView->show();

}
void QtGuiApplication1::GetEpipolar()
{
	this->cvR->Matching();
}
void QtGuiApplication1::CameraCalibration()
{
	this->cvR->CameraCalibration();
}
