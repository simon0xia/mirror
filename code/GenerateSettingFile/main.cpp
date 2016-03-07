
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

void Chenhao(const QString &inFile, const QString &outFile);

void Task(const QString &inFile, const QString &outFile);
void Formula(const QString &inFile, const QString &outFile);

void TowerDistribute(const QString &inFile, const QString &outFile);
void TowerDrop(const QString &inFile, const QString &outFile);

void dat_monster(const QString &outFile, qint32 count);
void dat_item(const QString &outFile, qint32 count);
void dat_dat(const QString resPath, const QString &outFile, qint32 count);

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
//	dat_item("../mirror/dat/item.dat", 1500);
//	dat_monster("../mirror/dat/monster.dat", 200);
//	dat_dat("./Resources/icon/", "../mirror/dat/icon.dat", 250);
//	dat_dat("./Resources/map/", "../mirror/dat/map.dat", 31);

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

//	Chenhao("./settings/chenhao.txt", "../mirror/db/chenhao.db");
//	Task("./settings/task.txt", "../mirror/db/task.db");

//	Formula("./settings/formula.txt", "../mirror/db/formula.db");

//	TowerDrop("./settings/TowerDrop.txt", "../mirror/db/TowerDrop.db");
//	TowerDistribute("./settings/TowerDistribute.txt", "../mirror/db/TowerDistribute.db");

	qDebug() << "Finish!";
	return a.exec();
}
