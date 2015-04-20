
#include <QtCore/QCoreApplication>
#include <QRegExp>
#include <QDebug>

void lvExp(const QString &fileName);
void monster(const QString &inFile, const QString &outFile);
void distribute(const QString &inFile, const QString &outFile);
void item(const QString &inFile, const QString &outFile);
void jobSet(const QStringList &jobSetFiles, const QString &outFile);

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

//	lvExp("./lvExpSet.db");

//	monster("./settings/Monster_normal.txt", "./Monster_normal1.db");
//	monster("./settings/Monster_boss.txt", "./Monster_boss1.db");


//	distribute("./settings/distribute.txt", "./distribute.db");

//	item("./settings/item_item.txt", "./item_item.db");

	QStringList list;
	list.append("./settings/job_1.txt");
	list.append("./settings/job_2.txt");
	list.append("./settings/job_3.txt");
//	jobSet(list, "./jobSet.db");

	return a.exec();
}
