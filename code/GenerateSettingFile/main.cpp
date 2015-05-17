
#include <QtCore/QCoreApplication>
#include <QRegExp>
#include <QDebug>

void lvExp(const QString &fileName);
void monster(const QString &inFile, const QString &outFile);
void distribute(const QString &inFile, const QString &outFile);
void item(const QString &inFile, const QString &outFile);
void Equip(const QString &inFile, const QString &outFile);
void Drop(const QString &inFile, const QString &outFile);
void jobSet(const QStringList &jobSetFiles, const QString &outFile);
void Skill(const QString &inFile, const QString &outFile);
void Buff(const QString &inFile, const QString &outFile);
void Task(const QString &inFile, const QString &outFile);

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

//	lvExp ("../mirror/db/lvExpSet.db");

//	monster("./settings/Monster_normal.txt",  "../mirror/db/Monster_normal1.db");
//	monster("./settings/Monster_boss.txt",  "../mirror/db/Monster_boss1.db");
//	distribute("./settings/distribute.txt",  "../mirror/db/distribute.db");

//	item("./settings/item_item.txt",  "../mirror/db/item_item.db");
//	Equip("./settings/item_equip.txt",  "../mirror/db/item_equip.db");
//	Drop("./settings/drop.txt",  "../mirror/db/drop.db");

	QStringList list;
	list.append("./settings/job_1.txt");
	list.append("./settings/job_2.txt");
	list.append("./settings/job_3.txt");
//	jobSet(list,  "../mirror/db/jobSet.db");

//	Skill("./settings/skill.txt", "../mirror/db/skill.db");
//	Buff("./settings/buff.txt", "../mirror/db/buff.db");

//	Task("./settings/task.txt", "../mirror/db/task.db");

	qDebug() << "Finish!";
	return a.exec();
}
