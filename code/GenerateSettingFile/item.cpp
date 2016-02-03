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

	qint32 ID, photo, vocation, level, coin, type, value;
	QString name, descr;

	qint32 count = 0;
	while (!out.atEnd())
	{
		out >> ID >> name >> photo >> vocation >> level >> coin >> type >> value >> descr;

		++count;
	}

	qDebug() << "find " << count << "equips define. the last equip was:";
	qDebug() << ID << name << photo << vocation << level << coin << type << value << descr;
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

	qint32 i,ID, photo, vocation, level, coin, type, value;
	QString name, descr;
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
		vocation = list.at(i++).toUInt();
		level = list.at(i++).toUInt();
		coin = list.at(i++).toUInt();
		type = list.at(i++).toUInt();
		value = list.at(i++).toUInt();
		descr = list.at(i++);

		iData << ID << name << photo << vocation << level << coin << type << value << descr;

		++nCount;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over. define " << nCount << "items";

	testItem(outFile);
}