
#include <QtCore/QCoreApplication>

void lvExp(const QString &fileName);
void monster(const QString &inFile, const QString &outFile);
void distribute(const QString &inFile, const QString &outFile);
void item(const QString &inFile, const QString &outFile);

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	lvExp("./expSetting.db");

//	monster("./settings/monster1.txt", "./monster1.db");
//	monster("./settings/boss.txt", "./boss.db");

//	distribute("./settings/distribute.txt", "./distribute.db");

//	item("./settings/item_item.txt", "./item_item.db");

	return a.exec();
}
