#include <QtCore\QtCore>
#include "..\mirror\MonsterDefine.h"

void monster(QString inFile, QString outFile)
{
	qDebug() << __FUNCTION__ << inFile << outFile;

	QStringList list;
	QString strTmp, strHeadPath;
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

		strHeadPath = QString::fromUtf8("./Resources/monster/");
		strHeadPath += QString::number(mon.ID) + QString::fromUtf8(".bmp");
//		pix = QPixmap(strHeadPath);
		mon.Head = QImage(strHeadPath);

		mon.level = list.at(2).toInt();
		mon.exp = list.at(3).toInt();
		mon.hp_m = list.at(4).toInt();
		mon.hp_r = list.at(5).toInt();
		mon.mp_m = list.at(6).toInt();
		mon.mp_r = list.at(7).toInt();
		mon.ap_m = list.at(8).toInt();
		mon.ap_r = 1;
		mon.DC = list.at(9).toUInt();
		mon.MC = list.at(10).toUInt();
		mon.SC = list.at(11).toUInt();
		mon.AC = list.at(12).toUInt();
		mon.MAC = list.at(13).toUInt();
		mon.extrarate = list.at(14).toUInt();
		mon.extrahurt = list.at(15).toUInt();
		mon.penetrate = list.at(16).toUInt();
		mon.Speed = list.at(17).toDouble();

		iData << mon.ID << mon.name << mon.Head << mon.level << mon.exp;
		iData << mon.hp_m << mon.hp_r << mon.mp_m << mon.mp_r << mon.ap_m << mon.ap_r;
		iData << mon.DC << mon.MC << mon.SC << mon.AC << mon.MAC;
		iData << mon.extrarate << mon.extrahurt << mon.penetrate << mon.Speed;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";
}