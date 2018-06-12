#pragma once
#include "cvRelating.h"
#include <QtWidgets/QMainWindow>
#include "ui_QtGuiApplication1.h"
#include <qdebug.h>

class QtGuiApplication1 : public QMainWindow
{
	Q_OBJECT

public:
	QtGuiApplication1(QWidget *parent = Q_NULLPTR);
	~QtGuiApplication1();
	

private slots:
	void CoverCVImageToQImage();
	void SIFT();
	void GetEpipolar();
	void CameraCalibration();
private:
	typedef struct scene_Cont
	{
		QGraphicsScene *scene=nullptr;
		QImage* Img=nullptr;
	}Scene_cont;

	Scene_cont L_Scene;
	Scene_cont R_Scene;
	Scene_cont LE_Scene;
	Scene_cont RE_Scene;
	Ui::QtGuiApplication1Class ui;

	/*QGraphicsScene *scene = nullptr;*/
	cvRelating* cvR;
	//QPushButton *m_button;
};


