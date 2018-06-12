#include "QtGuiApplication1.h"
#include <QtWidgets/QApplication>
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtGuiApplication1 w;
	w.show();
	
	return a.exec();
}
