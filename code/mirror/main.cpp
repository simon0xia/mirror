#include "mirror.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	mirror w;
	w.show();
	return a.exec();
}
