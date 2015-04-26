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
	quint32 ID, sale, level, coin, gold, type, value;
	QString name, descr, msg;

	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> ID >> name >> img >> sale >> level >> coin >> gold >> type >> value >> descr >> msg;

		qDebug() << ID << name << sale << level << coin << gold << type << value << descr << msg << "\n";
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
	quint32 ID, sale, level, coin, gold, type, value;
	QString name, descr, msg, strImgPath;

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

		strImgPath = QString("./Resources/item/");
		strImgPath += QString::number(ID) + QString(".bmp");
		if (!QFile::exists(strImgPath))
		{
			strImgPath = QString("./Resources/item/0.bmp");
		}
		img = QImage(strImgPath);
		if (img.isNull())
		{
			qDebug() << "No Head:" << strImgPath;
			break;
		}

		sale = list.at(2).toUInt();
		level = list.at(3).toUInt();
		coin = list.at(4).toUInt();
		gold = list.at(5).toUInt();
		type = list.at(6).toUInt();
		value = list.at(7).toUInt();
		descr = list.at(8);
		msg = list.at(9);

		iData << ID << name << img << sale << level << coin << gold << type << value << descr << msg;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	testItem(outFile);
}