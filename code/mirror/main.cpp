#include "mirror.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QMessageBox>
#include "login_main.h"
#include "myapplication.h"

int main(int argc, char *argv[])
{
	MyApplication a(argc, argv);
	if (a.isRunning())
	{
		exit(0);
	}

	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

	login_main *lm = new login_main;
	if (QDialog::Accepted != lm->exec())
	{
		exit(0);
	}
	delete lm;

	mirror *w = new mirror;
	a.w = w;
	w->show();

	return a.exec();
}
