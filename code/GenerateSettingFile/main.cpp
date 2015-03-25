
#include <QtCore/QCoreApplication>

void lvExp(QString fileName);

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	lvExp("./expSetting.db");

	return a.exec();
}
