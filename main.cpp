#include "contactfu_qt.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	ContactFU_QT main; main.resize(640,480);
	main.show(); 
	return app.exec();
}
