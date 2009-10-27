#include "contactfu_qt.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	ContactFU_QT main; main.setFixedSize(573,455);
	main.show();
	return app.exec();
}
