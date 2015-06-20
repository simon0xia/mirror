#include "mirror.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <time.h>
#include "login_main.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	  
	qsrand(clock());

	login_main *lm = new login_main;
	if (QDialog::Accepted != lm->exec())
	{
		exit(0);
	}
	delete lm;


	mirror *w = new mirror;
	w->show();

	return a.exec();
}
