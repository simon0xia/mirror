#include <QtCore\QtCore>
#include "..\mirror\MonsterDefine.h"

void monster(const QString &inFile, const QString &outFile)
{
	qDebug() << __FUNCTION__ << inFile << outFile;

	quint32 nPhoto;
	QStringList list;
	QString strTmp, strPath;
	MonsterInfo mon = { 0 };
	QImage img;

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

	QDataStream iData(&Wfile);

	Rfile.readLine(1000);		//第一行是标题
	while (!Rfile.atEnd())
	{
		strTmp = Rfile.readLine(1000);
		list = strTmp.split("\t");
 
		mon.ID = list.at(0).toUInt();
		mon.name = list.at(1);

		nPhoto = list.at(2).toUInt();
		strPath = ("./Resources/monster/");
		strPath += QString::number(nPhoto) + (".png");
		mon.Head = QImage(strPath);

		if (mon.Head.isNull())
		{
			qDebug() << "No Head:" << strPath;
			break;
		}

		mon.level = list.at(3).toInt();
		mon.exp = list.at(4).toInt();
		mon.hp = list.at(5).toInt();
		mon.mp = list.at(6).toInt();
		mon.DC1 = list.at(7).toUInt();
		mon.DC2 = list.at(8).toUInt();
		mon.MC1 = list.at(9).toUInt();
		mon.MC2 = list.at(10).toUInt();
		mon.AC = list.at(11).toUInt();
		mon.MAC = list.at(12).toUInt();
		mon.interval = list.at(13).toUInt();

		iData << mon.ID << mon.name << mon.Head << mon.level << mon.exp << mon.hp << mon.mp;
		iData << mon.DC1 << mon.DC2 << mon.MC1 << mon.MC2 << mon.AC << mon.MAC << mon.interval;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";
}