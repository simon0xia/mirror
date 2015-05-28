#include <QtCore\QtCore>
#include <QImage>

void testDistribute(const QString &inFile)
{
	qDebug() << __FUNCTION__ << inFile;

	QFile file(inFile);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	quint32 mapID, need_lv, expend_rep, expend_item;
	QString name;
	QImage img;
	QVector<quint32> vec_normal, vec_boss;

	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> mapID >> name >> img >> need_lv >> expend_rep >> expend_item >> vec_normal >> vec_boss;

		qDebug() << mapID << name << img.isNull() << need_lv << expend_rep << expend_item << vec_normal.size() << vec_boss.size();
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

	quint32 mapID, need_lv, photo, expend_rep, expend_item, index, nStart, nStop;
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
		need_lv = list.at(index++).toUInt();
		expend_rep = list.at(index++).toUInt();
		expend_item = list.at(index++).toUInt();

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
		iData << mapID << name << img << need_lv << expend_rep << expend_item << vec_normal << vec_boss;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	testDistribute(outFile);
}
