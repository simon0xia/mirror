#include <QtCore/QtCore>
#include <QImage>

void testBuff(const QString &inFile)
{
	qDebug() << __FUNCTION__ << inFile;

	QFile file(inFile);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	quint32 ID;
	double time, rhp, ac, mac;
	QString name;

	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> ID >> name >> time >> rhp >> ac >> mac;

		qDebug() << ID << name << time << rhp << ac << mac;
	}

	file.close();
}

void Buff(const QString &inFile, const QString &outFile)
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

	quint32 i, ID;
	double time, rhp, ac, mac;
	QString name;

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
		time = list.at(i++).toDouble();
		rhp = list.at(i++).toDouble();
		ac = list.at(i++).toDouble();
		mac = list.at(i++).toDouble();
		
		iData << ID << name << time << rhp << ac << mac;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	testBuff(outFile);
}