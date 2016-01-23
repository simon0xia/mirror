#include <QtCore\QtCore>

void testTowerDrop(const QString &inFile)
{
	qDebug() << __FUNCTION__ << inFile;

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

	quint32 TowerLv, ListSize, item, no;
	QList<quint32> dList;

	no = 0;
	while (!out.atEnd())
	{
		no++;

		out >> TowerLv >> ListSize;

		dList.clear();
		while (ListSize)
		{
			out >> item;
			dList.append(item);
			--ListSize;
		};
	}

	QString strTmp;
	foreach(auto n, dList)
	{
		strTmp += QString::number(n) + " ";
	}
	qDebug() << "has " << no << "Tower level drop setting. the last define:";
	qDebug() << TowerLv << " " << strTmp;
}

void TowerDrop(const QString &inFile, const QString &outFile)
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

	qint32 TowerLv, item[20], no;

	QString strTmp;
	QStringList list;

	QDataStream iData(&Wfile);

	qint32 index;
	Rfile.readLine(1000);		//第一行是标题
	while (!Rfile.atEnd())
	{
		strTmp = Rfile.readLine(1000);
		list = strTmp.split("\t");

		index = no = 0;
		TowerLv = list.at(index++).toUInt();
		while (index < list.size())
		{
			item[no++] = list.at(index++).toUInt();
		}

		iData << TowerLv << no;
		for (auto i = 0; i < no; i++)
		{
			iData << item[i];
		}
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	testTowerDrop(outFile);
}
