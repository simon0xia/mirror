#include <QtCore\QtCore>

struct Rational {
	quint32 ID;
	//quint32 num;		//始终为1
	quint32 den;
};
typedef QList<Rational> DropList;
typedef QMap<quint32, DropList> mapDrop;

void testDrop(const QString &inFile)
{
	qDebug() << __FUNCTION__ << inFile;

	QFile file(inFile);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	quint32 monsterID, ListSize;
	Rational rRat;
	DropList dList;

	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> monsterID >> ListSize;

		dList.clear();
		while (ListSize)
		{
			out >> rRat.ID >> rRat.den;
			dList.append(rRat);
			--ListSize;
		} ;	
		qDebug() << monsterID << dList.size();
	}

	file.close();
}

void Drop(const QString &inFile, const QString &outFile)
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

	quint32 monsterID;
	Rational rRat;
	DropList dList;
	mapDrop mMap;

	QString strTmp;
	QStringList list;

	QDataStream iData(&Wfile);

	quint32 i;
	Rfile.readLine(1000);		//第一行是标题
	while (!Rfile.atEnd())
	{
		strTmp = Rfile.readLine(1000);
		list = strTmp.split("\t");
		
		i = 0;
		monsterID = list.at(i++).toUInt();
		iData << monsterID;

		dList.clear();
		while (i < list.size())
		{
			rRat.ID = list.at(i++).toUInt();
			rRat.den = list.at(i++).toUInt();

			if (rRat.ID != 0)
				dList.append(rRat);
		}

		iData << dList.size();
		foreach(const Rational &Rat, dList)
			iData << Rat.ID << Rat.den;	

		mMap[monsterID] = dList;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	testDrop(outFile);
}
