//#include <vld.h>
#include "mirror.h"
#include <QtWidgets/QApplication>
#include "login_main.h"
#include "myapplication.h"

#define singleton

int main(int argc, char *argv[])
{
#ifdef singleton
	MyApplication a(argc, argv);
	if (a.isRunning())
	{
		exit(0);
	}
#else
	QApplication a(argc, argv);	
#endif

	QFont font = a.font();
	font.setPointSize(9);
	a.setFont(font);

	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

	login_main *lm = new login_main;
	if (QDialog::Accepted != lm->exec())
	{
		delete lm;
		exit(0);
	}
	delete lm;

	mirror w;
#ifdef singleton
	a.w = &w;
#endif
	
	w.show();

	return a.exec();
}
