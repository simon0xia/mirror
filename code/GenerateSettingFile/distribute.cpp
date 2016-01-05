#include <QtCore\QtCore>
#include <QImage>

void testDistribute(const QString &inFile)
{
	qDebug() << "\r " << __FUNCTION__ << inFile;

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

	quint32 mapID;
	qint32 need_lv, monsterCount;
	QString name;
	QImage img;
	QVector<quint32> vec_normal, vec_boss;

	qint32 nCount = 0;
	while (!out.atEnd())
	{
		out >> mapID >> name >> img >> need_lv >> monsterCount >> vec_normal >> vec_boss;
		++nCount;		
	}

	qDebug() << "has " << nCount << "map distribute define. the last define:";
	qDebug() << mapID << name << img.isNull() << need_lv << monsterCount << vec_normal.size() << vec_boss.size();
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

	quint32 mapID, photo;
	qint32 need_lv, monsterCount, index, nStart, nStop, nCount = 0;
	QImage img;
	QString name, strPath, strTmp, monster, boss;
	QStringList list, list1, list11;
	QVector<quint32> vec_normal, vec_boss;

	QDataStream iData(&Wfile);
	Rfile.readLine(1000);		//第一行是标题
	while (!Rfile.atEnd())
	{
		index = 0;
		strTmp = Rfile.readLine(1000);
		list = strTmp.split("\t");

		mapID = list.at(index++).toUInt();
		name = list.at(index++);

		photo = list.at(index++).toUInt();
		strPath = QString("./Resources/map/");
		strPath += QString::number(photo) + QString(".png");
		if (!QFile::exists(strPath))
		{
			strPath = QString("./Resources/map/0.png");
		}
		img = QImage(strPath);
		if (img.isNull())
		{
			qDebug() << "\n****No Head:" << strPath;
			break;
		}
		need_lv = list.at(index++).toInt();
		monsterCount = list.at(index++).toInt();

		vec_normal.clear();
		monster = list.at(index++);
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
		boss = list.at(index++);
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
		iData << mapID << name << img << need_lv << monsterCount << vec_normal << vec_boss;

		++nCount;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over. has" << nCount << "maps";

	testDistribute(outFile);
}
