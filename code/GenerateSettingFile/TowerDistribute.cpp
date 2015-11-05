#include <QtCore\QtCore>

void testTowerDistribute(const QString &inFile)
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

	quint32 mapID, cost, monster1, monster2, monster3;

	qint32 nCount = 0;
	while (!out.atEnd())
	{
		out >> mapID >> cost >> monster1 >> monster2 >> monster3;
		++nCount;
	}

	qDebug() << "has " << nCount << "map distribute define. the last define:";
	qDebug() << mapID << cost << monster1 << monster2 << monster3;
}

void TowerDistribute(const QString &inFile, const QString &outFile)
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

	quint32 mapID, cost, monster1, monster2, monster3;
	qint32 index, nCount;
	QString strTmp;
	QStringList list;

	nCount = 0;
	QDataStream iData(&Wfile);
	Rfile.readLine(1000);		//第一行是标题
	while (!Rfile.atEnd())
	{
		index = 0;
		strTmp = Rfile.readLine(1000);
		list = strTmp.split("\t");

		mapID = list.at(index++).toUInt();
		cost = list.at(index++).toUInt();
		monster1 = list.at(index++).toInt();
		monster2 = list.at(index++).toInt();
		monster3 = list.at(index++).toInt();

		iData << mapID << cost << monster1 << monster2 << monster3;

		++nCount;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over. has" << nCount << "maps";

	testTowerDistribute(outFile);
}
