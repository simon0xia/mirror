#include <QtCore/QtCore>
#include <QImage>

void testItem(const QString &inFile)
{
	qDebug() << "\n" << __FUNCTION__ << inFile;

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

	QImage img;
	quint32 ID, vocation, level, sale, coin, type, value;
	QString name, descr;

	qint32 count = 0;
	while (!out.atEnd())
	{
		out >> ID >> name >> img >> vocation >> level >> sale >> coin >> type >> value >> descr;

		++count;
	}

	qDebug() << "find " << count << "equips define. the last equip was:";
	qDebug() << ID << name << vocation << level << sale << coin << type << value << descr;
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
	quint32 i,ID, photo, vocation, level, sale, coin, type, value;
	QString name, descr, strImgPath;
	qint32 nCount = 0;

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
		type = list.at(i++).toUInt();
		value = list.at(i++).toUInt();
		descr = list.at(i++);

		iData << ID << name << img << vocation << level << sale << coin << type << value << descr;

		++nCount;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over. define " << nCount << "items";

	testItem(outFile);
}