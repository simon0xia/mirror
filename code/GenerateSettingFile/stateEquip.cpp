#include <QtCore/QtCore>
#include <QImage>

void testStateEquip(const QString &inFile)
{
	qDebug() << __FUNCTION__ << inFile;

	QFile file(inFile);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	QImage img;
	quint32 ID;
	qint32 offset_x, offset_y;

	QDataStream out(&file);

	while (!out.atEnd())
	{
		out >> ID >> img >> offset_x >> offset_y;

		qDebug() << ID << img.isDetached() << offset_x << offset_y;
	}
	file.close();
}

void StateEquip(const QString &inFile, const QString &outFile)
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
	int i = 0;
	QImage img;
	quint32 ID, photo, count = 0;
	qint32 offset_x, offset_y;
	QString strImgPath;

	QDataStream iData(&Wfile);

	Rfile.readLine(1000);		//第一行是标题
	while (!Rfile.atEnd())
	{
		strTmp = Rfile.readLine(1000);
		if (strTmp.isEmpty() || strTmp.isNull())
		{
			break;	//防止文件尾部有空白行。
		}
		list = strTmp.split("\t");
		i = 0;
		ID = list.at(i++).toUInt();

		photo = list.at(i++).toUInt();
		strImgPath = QString("./Resources/StateEquip/");
		strImgPath += QString::number(photo) + QString(".png");
		if (!QFile::exists(strImgPath))
		{
			qDebug() << "Can not find image:" << strImgPath;
			break;
		}
		img = QImage(strImgPath);
		if (img.isNull())
		{
			qDebug() << "The image was invalid:" << strImgPath;
			break;
		}

		offset_x = list.at(i++).toInt();
		offset_y = list.at(i++).toInt();

		iData << ID << img << offset_x << offset_y;
		++count;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over. Has " << count << "equip";

	testStateEquip(outFile);
}