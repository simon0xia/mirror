#include <QtCore/QtCore>
#include <QImage>

void testEquip(const QString &inFile)
{
	qDebug() << "\n"  << __FUNCTION__ << inFile;

	QFile file(inFile);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	QByteArray documentContent = file.readAll();
	file.close();

	QByteArray MD5arr = QCryptographicHash::hash(documentContent, QCryptographicHash::Md5).toHex();
	qDebug() << "MD5:" << MD5arr.data();

	QDataStream out(documentContent);

	QImage img;
	quint32 ID, lv, luck, acc, sacred, ag, spd, md, ep, ed, ac1, ac2, mac1, mac2, dc1, dc2, mc1, mc2, sc1, sc2, need, needLvl, price;
	QString name, strImgPath;

	qint32 count = 0;
	while (!out.atEnd())
	{
		out >> ID >> name >> img >> lv >> luck >> acc >> sacred >> ag >> spd >> md >> ep >> ed >> ac1 >> ac2 >> mac1 >> mac2
			>> dc1 >> dc2 >> mc1 >> mc2 >> sc1 >> sc2 >> need >> needLvl >> price;

		++count;
	}

	qDebug() << "find " << count << "equips define. the last equip was:";
	qDebug() << ID << name << lv;
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
	int i = 0, count = 0;
	QImage img;
	quint32 ID, photo, lv, luck, acc, sacred, ag, spd, md, ep, ed, ac1, ac2, mac1, mac2, dc1, dc2, mc1, mc2, sc1, sc2, need, needLvl, price;
	QString name, strImgPath;

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
		i = 0;
		ID = list.at(i++).toUInt();
		name = list.at(i++);

		photo = list.at(i++).toUInt();
		strImgPath = QString("./Resources/equip/");
		strImgPath += QString::number(photo) + QString(".png");
		if (!QFile::exists(strImgPath))
		{
			strImgPath = QString("./Resources/equip/0.png");
		}
		img = QImage(strImgPath);
		if (img.isNull())
		{
			qDebug() << "No Head:" << strImgPath;
			break;
		}

		lv = list.at(i++).toUInt();
		luck = list.at(i++).toUInt();
		acc = list.at(i++).toUInt();
		sacred = list.at(i++).toUInt();
		ag = list.at(i++).toUInt();
		spd = list.at(i++).toUInt();
		md = list.at(i++).toUInt();
		ep = list.at(i++).toUInt();
		ed = list.at(i++).toUInt();
		ac1 = list.at(i++).toUInt();
		ac2 = list.at(i++).toUInt();
		mac1 = list.at(i++).toUInt();
		mac2 = list.at(i++).toUInt();
		dc1 = list.at(i++).toUInt();
		dc2 = list.at(i++).toUInt();
		mc1 = list.at(i++).toUInt();
		mc2 = list.at(i++).toUInt();
		sc1 = list.at(i++).toUInt();
		sc2 = list.at(i++).toUInt();
		need = list.at(i++).toUInt();
		needLvl = list.at(i++).toUInt();
		price = list.at(i++).toUInt();
		
		iData << ID << name << img << lv << luck << acc << sacred << ag << spd << md << ep << ed << ac1 << ac2 << mac1 << mac2;
		iData << dc1 << dc2 << mc1 << mc2 << sc1 << sc2 << need << needLvl << price;

		++count;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over. define " << count << "equips";

	testEquip(outFile);
}