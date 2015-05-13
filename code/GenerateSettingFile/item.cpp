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
	quint32 ID, vocation, level, sale, coin, gold, type, value;
	QString name, descr;

	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> ID >> name >> img >> vocation >> level >> sale >> coin >> gold >> type >> value >> descr;

		qDebug() << ID << name << img.isDetached() << vocation << level << sale << coin << gold << type << value << descr << "\n";
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
	quint32 i,ID, photo, vocation, level, sale, coin, gold, type, value;
	QString name, descr, strImgPath;

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

		strImgPath = QString("./Resources/item/");
		strImgPath += QString::number(photo) + QString(".png");
		if (!QFile::exists(strImgPath))
		{
			qDebug() << "Cannot find file." << strImgPath;
			break;
		}
		img = QImage(strImgPath);
		if (img.isNull())
		{
			qDebug() << "No Head:" << strImgPath;
			break;
		}

		vocation = list.at(i++).toUInt();
		level = list.at(i++).toUInt();
		sale = list.at(i++).toUInt();		
		coin = list.at(i++).toUInt();
		gold = list.at(i++).toUInt();
		type = list.at(i++).toUInt();
		value = list.at(i++).toUInt();
		descr = list.at(i++);

		iData << ID << name << img << vocation << level << sale << coin << gold << type << value << descr;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	testItem(outFile);
}