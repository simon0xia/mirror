#include <QtCore\QtCore>

typedef QList<quint32> DropList;

struct _tagDrop {
	quint32 chance;
	DropList list;
};
typedef QMap<quint32, _tagDrop> mapDrop;

void testDrop(const QString &inFile)
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

	quint32 mapid, chance, itemCount, nTmp;
	quint32 ListSize;

	DropList dList;

	while (!out.atEnd())
	{
		out >> mapid >> chance >> ListSize;
		itemCount = ListSize;

		dList.clear();
		while (ListSize)
		{
			out >> nTmp;
			dList.append(nTmp);
			--ListSize;
		}

		qDebug() << mapid << chance << itemCount;
	}
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

	quint32 mapid, chance;
	DropList droplist;

	QString strTmp;
	QStringList list;

	QDataStream iData(&Wfile);

	quint32 i, nTmp;
	Rfile.readLine(1000);		//第一行是标题
	while (!Rfile.atEnd())
	{
		strTmp = Rfile.readLine(1000);
		list = strTmp.split("\t");
		
		i = 0;
		mapid = list.at(i++).toUInt();
		chance = list.at(i++).toUInt();
		iData << mapid << chance;

		droplist.clear();
		while (i < list.size())
		{
			nTmp = list.at(i++).toUInt();
			if (nTmp > 0)
			{
				droplist.append(nTmp);
			}	
		}

		iData << droplist.size();
		foreach(auto n, droplist)
			iData << n;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	testDrop(outFile);
}
