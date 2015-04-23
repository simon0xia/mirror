#include <QtCore/QtCore>
#include <QImage>

void testEquip(const QString &inFile)
{
	qDebug() << __FUNCTION__ << inFile;

	QFile file(inFile);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	QImage img;
	quint32 ID, type, ac1, ac2, mac1, mac2, dc1, dc2, mc1, mc2, sc1, sc2, need, needLvl, price;
	QString name, msg, strImgPath;

	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> ID >> name >> img >> type >> ac1 >> ac2 >> mac1 >> mac2 
			>> dc1 >> dc2 >> mc1 >> mc2 >> sc1 >> sc2 >> need >> needLvl >> price >> msg;

		qDebug() << ID << name << type << ac1 << ac2 << mac1 << mac2
		         << dc1 << dc2 << mc1 << mc2 << sc1 << sc2 << need << needLvl << price << msg;
	}

	file.close();
}

void Equip(const QString &inFile, const QString &outFile)
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

	QString strTmp;
	QStringList list;

	QImage img;
	quint32 ID, type, ac1, ac2, mac1, mac2, dc1, dc2, mc1, mc2, sc1, sc2, need, needLvl, price;
	QString name, msg, strImgPath;

	QDataStream iData(&Wfile);

	Rfile.readLine(1000);		//第一行是标题
	while (!Rfile.atEnd())
	{
		strTmp = Rfile.readLine(1000);
		if (strTmp.isEmpty() || strTmp.isNull())
		{
			//防止文件尾部有空白行。
			break;
		}
		list = strTmp.split("\t");

		ID = list.at(0).toUInt();
		name = list.at(1);

		strImgPath = QString("./Resources/equip/");
		strImgPath += QString::number(ID) + QString(".png");
		if (!QFile::exists(strImgPath))
		{
			strImgPath = QString("./Resources/equip/0.png");
		}
		img = QImage(strImgPath);

		type = list.at(2).toUInt();
		ac1 = list.at(3).toUInt();
		ac2 = list.at(4).toUInt();
		mac1 = list.at(5).toUInt();
		mac2 = list.at(6).toUInt();
		dc1 = list.at(7).toUInt();
		dc2 = list.at(8).toUInt();
		mc1 = list.at(9).toUInt();
		mc2 = list.at(10).toUInt();
		sc1 = list.at(11).toUInt();
		sc2 = list.at(12).toUInt();
		need = list.at(13).toUInt();
		needLvl = list.at(14).toUInt();
		price = list.at(15).toUInt();
		msg = list.at(16);
		
		iData << ID << name << img << type << ac1 << ac2 << mac1 << mac2;
		iData << dc1 << dc2 << mc1 << mc2 << sc1 << sc2 << need << needLvl << price << msg;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	testEquip(outFile);
}