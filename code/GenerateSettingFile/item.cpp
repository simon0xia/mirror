#include <QtCore/QtCore>
#include <QImage>

void testItem(const QString &inFile)
{
	qDebug() << __FUNCTION__ << inFile;

	QFile file(inFile);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	QImage img;
	quint32 ID, level, coin, gold, type, value;
	QString name, info1, info2;

	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> ID >> name >> img >> level >> coin >> gold >> type >> value >> info1 >> info2;

		qDebug() << ID << name << coin << gold << type << value << info1 << info2 << "\n";
	}

	file.close();
}

void item(const QString &inFile, const QString &outFile)
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
	quint32 ID, level, coin, gold, type, value;
	QString name, info1, info2, strImgPath;

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

		strImgPath = QString::fromUtf8("./Resources/item/");
		strImgPath += QString::number(ID) + QString::fromUtf8(".bmp");
		img = QImage(strImgPath);

		level = list.at(2).toUInt();
		coin = list.at(3).toUInt();
		gold = list.at(4).toUInt();
		type = list.at(5).toUInt();
		value = list.at(6).toUInt();
		info1 = list.at(7);
		info2 = list.at(8);

		iData << ID << name << img << level << coin << gold << type << value << info1 << info2;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	testItem(outFile);
}