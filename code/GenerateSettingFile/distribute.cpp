#include <QtCore\QtCore>


void testDistribute(const QString &inFile)
{
	qDebug() << __FUNCTION__ << inFile;

	QFile file(inFile);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	quint32 id;
	QVector<quint32> vec_normal, vec_boss;

	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> id >> vec_normal >> vec_boss;

		qDebug() << id << vec_normal << vec_boss;
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

	quint32 mapID, nStart, nStop;
	QString strTmp, monster, boss;
	QStringList list, list1, list11;
	QVector<quint32> vec_normal, vec_boss;

	QDataStream iData(&Wfile);

	Rfile.readLine(1000);		//第一行是标题
	while (!Rfile.atEnd())
	{
		strTmp = Rfile.readLine(1000);
		list = strTmp.split("\t");

		mapID = list.at(0).toUInt();

		vec_normal.clear();
		monster = list.at(1);
		list1 = monster.split(",");
		for (quint32 i = 0; i < list1.size(); i++)
		{
			list11 = list1.at(i).split("-");
			nStart = list11.at(0).toUInt();
			nStop = list11.at(list11.size() - 1).toUInt();
			for (quint32 j = nStart; j <= nStop; j++)
			{
				vec_normal << j;
			}
		}

		vec_boss.clear();
		boss = list.at(2);
		list1 = boss.split(",");
		for (quint32 i = 0; i < list1.size(); i++)
		{
			list11 = list1.at(i).split("-");
			nStart = list11.at(0).toUInt();
			nStop = list11.at(list11.size() - 1).toUInt();
			for (quint32 j = nStart; j <= nStop; j++)
			{
				vec_boss << j;
			}
		}
		iData << mapID << vec_normal << vec_boss;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	testDistribute(outFile);
}
