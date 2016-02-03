
#include <QtCore/QCoreApplication>
#include <QRegExp>
#include <QDebug>

void monster(const QString &inFile, const QString &outFile);
void distribute(const QString &inFile, const QString &outFile);
void item(const QString &inFile, const QString &outFile);
void Equip(const QString &inFile, const QString &outFile);
void StateEquip(const QString &inFile, const QString &outFile);
void Drop(const QString &inFile, const QString &outFile);
void jobSet(const QStringList &jobSetFiles, const QString &outFile);

void Skill_basic(const QString &inFile, const QString &outFile);
void skill_damage(const QString &inFile, const QString &outFile);
void skill_buff(const QString &inFile, const QString &outFile);
void skill_summon(const QString &inFile, const QString &outFile);
void skill_treat(const QString &inFile, const QString &outFile);

void Task(const QString &inFile, const QString &outFile);
void Formula(const QString &inFile, const QString &outFile);

void TowerDistribute(const QString &inFile, const QString &outFile);
void TowerDrop(const QString &inFile, const QString &outFile);

void dat_item(const QString &outFile, qint32 count);
void dat_UI(const QString &outFile, qint32 count);

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
//	dat_UI("../mirror/dat/ui.dat", 10);
//	dat_item("../mirror/dat/item.dat", 1500);

//	monster("./settings/Monster.txt",  "../mirror/db/Monster.db");
//	distribute("./settings/distribute.txt",  "../mirror/db/distribute.db");

//	item("./settings/item_item.txt",  "../mirror/db/item_item.db");
//	Equip("./settings/item_equip.txt",  "../mirror/db/item_equip.db");
//	StateEquip("./settings/StateEquip.txt", "../mirror/db/StateEquip.db");
//	Drop("./settings/drop.txt",  "../mirror/db/drop.db");

	QStringList list;
	list.append("./settings/job_1.txt");
	list.append("./settings/job_2.txt");
	list.append("./settings/job_3.txt");
//	jobSet(list,  "../mirror/db/jobSet.db");

//	Skill_basic("./settings/skill_basic.txt", "../mirror/db/skill_basic.db");
//	skill_damage("./settings/skill_damage.txt", "../mirror/db/skill_damage.db");
//	skill_buff("./settings/skill_buff.txt", "../mirror/db/skill_buff.db");
//	skill_summon("./settings/skill_summon.txt", "../mirror/db/skill_summon.db");
//	skill_treat("./settings/skill_treat.txt", "../mirror/db/skill_treat.db");

//	Task("./settings/task.txt", "../mirror/db/task.db");

//	Formula("./settings/formula.txt", "../mirror/db/formula.db");

//	TowerDrop("./settings/TowerDrop.txt", "../mirror/db/TowerDrop.db");
//	TowerDistribute("./settings/TowerDistribute.txt", "../mirror/db/TowerDistribute.db");

	qDebug() << "Finish!";
	return a.exec();
}
