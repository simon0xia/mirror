#include <QtCore\QtCore>


void testDistribute(const QString &inFile)
{
	qDebug() << __FUNCTION__ << inFile;

	QFile file(inFile);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	quint32 id, monster_id_start, monster_id_stop, boss_id_start, boss_id_stop;

	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> id >> monster_id_start >> monster_id_stop >> boss_id_start >> boss_id_stop;

		qDebug() << id << monster_id_start << monster_id_stop << boss_id_start << boss_id_stop;
	}

	file.close();
}

void distribute(const QString &inFile, const QString &outFile)
{
	qDebug() << __FUNCTION__ << inFile << outFile;

	QFile Rfile(inFile);
	if (!Rfile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "open " << inFile << "error.";
		return;
	}

	QFile Wfile(outFile);
	if (!Wfile.open(QIODevice::WriteOnly))
	{
		qDebug() << "open " << outFile << "error.";
		return;
	}

	quint32 mapID, monster_id_start, monster_id_stop, boss_id_start, boss_id_stop;
	QString strTmp, monster, boss;
	QStringList list, list1,list2;

	QDataStream iData(&Wfile);

	Rfile.readLine(1000);		//第一行是标题
	while (!Rfile.atEnd())
	{
		strTmp = Rfile.readLine(1000);
		list = strTmp.split("\t");

		mapID = list.at(0).toUInt();

		monster = list.at(1);
		list1 = monster.split("-");
		monster_id_start = list1.at(0).toUInt();
		monster_id_stop = list1.at(list1.size() - 1).toUInt();

		boss = list.at(2);
		list2 = boss.split("-");
		boss_id_start = list2.at(0).toUInt();
		boss_id_stop = list2.at(list2.size() - 1).toUInt();

		iData << mapID << monster_id_start << monster_id_stop << boss_id_start << boss_id_stop;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";


	testDistribute(outFile);
}
