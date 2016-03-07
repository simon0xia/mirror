#include <QtCore\QtCore>
#include <QImage>


void dat_item(const QString &outFile, qint32 count)
{
	qDebug() << __FUNCTION__ << outFile;

	QFile Wfile(outFile);
	if (!Wfile.open(QIODevice::WriteOnly))
	{
		qDebug() << "open " << outFile << "error.";
		return;
	}

	qint32 i = 0;
	QString strPath;
	QImage img;

	QDataStream iData(&Wfile);
	while (i < count)
	{
		strPath = QString("./Resources/item/%1.png").arg(i);
		if (!QFile::exists(strPath))
		{
			strPath = QString("./Resources/item/0.png");
		}
		img = QImage(strPath);
		if (img.isNull())
		{
			qDebug() << "\n****Load Image error:" << strPath;
			break;
		}
		iData << img;

		++i;
	}
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over. has" << i << "items";
}


void dat_monster(const QString &outFile, qint32 count)
{
	qDebug() << __FUNCTION__ << outFile;

	QFile Wfile(outFile);
	if (!Wfile.open(QIODevice::WriteOnly))
	{
		qDebug() << "open " << outFile << "error.";
		return;
	}

	qint32 i = 0;
	QString strPath;
	QImage img;

	QDataStream iData(&Wfile);
	while (i < count)
	{
		strPath = QString("./Resources/monster/%1.png").arg(i);
		if (!QFile::exists(strPath))
		{
			strPath = QString("./Resources/monster/0.png");
		}
		img = QImage(strPath);
		if (img.isNull())
		{
			qDebug() << "\n****Load Image error:" << strPath;
			break;
		}
		iData << img;

		++i;
	}
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over. has" << i << "items";
}

void dat_dat(const QString resPath, const QString &outFile, qint32 count)
{
	qDebug() << __FUNCTION__ << outFile;

	QFile Wfile(outFile);
	if (!Wfile.open(QIODevice::WriteOnly))
	{
		qDebug() << "open " << outFile << "error.";
		return;
	}

	qint32 i = 0;
	QString strPath;
	QImage img;

	QDataStream iData(&Wfile);
	while (i < count)
	{
		strPath = resPath + QString("%1.png").arg(i);
		if (!QFile::exists(strPath))
		{
			strPath = resPath + QString("0.png");
		}
		img = QImage(strPath);
		if (img.isNull())
		{
			qDebug() << "\n****Load Image error:" << strPath;
			break;
		}
		iData << img;

		++i;
	}
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over. has" << i << "items";
}