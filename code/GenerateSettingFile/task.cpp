#include <QtCore\QtCore>


void testTask(const QString &inFile)
{
	qDebug() << __FUNCTION__ << inFile;

	QFile file(inFile);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	quint32 requireItem, requireCount, giveCount;
	QVector<quint32> vecGiveItem;
	QString msg;

	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> requireItem >> requireCount;
		out >> vecGiveItem >> giveCount;
		out >> msg;

		qDebug() << requireItem << requireCount << vecGiveItem << giveCount << "Len(msg):" << msg.size();
	}

	file.close();
}

void Task(const QString &inFile, const QString &outFile)
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

	QDataStream iData(&Wfile);

	quint32 index, requireItem, requireCount, giveCount, nStart, nStop;
	QString strTmp, giveItem, Msg;
	QStringList list, list1, list11;
	QVector<quint32> vecItem;

	Rfile.readLine(1000);		//第一行是标题
	while (!Rfile.atEnd())
	{
		strTmp = Rfile.readLine(1000);
		list = strTmp.split("\t");

		index = 0;
		requireItem = list.at(index++).toUInt();
		requireCount = list.at(index++).toUInt();
		
		vecItem.clear();
		giveItem = list.at(index++);
		list1 = giveItem.split(",");
		for (quint32 i = 0; i < list1.size(); i++)
		{
			list11 = list1.at(i).split("-");
			nStart = list11.at(0).toUInt();
			nStop = list11.at(list11.size() - 1).toUInt();
			for (quint32 j = nStart; j <= nStop; j++)
			{
				vecItem << j;
			}
		}

		giveCount = list.at(index++).toUInt();
		Msg = list.at(index++);

		iData << requireItem << requireCount << vecItem << giveCount << Msg;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	testTask(outFile);
}
