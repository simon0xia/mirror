#include <QtCore\QtCore>
#include <QImage>

void testmonster(const QString &inFile)
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

	quint32 id, photo, level, exp, hp, mp, DC1, DC2, MC1, MC2, AC, MAC, Hit, interval;
	QString name, descr;
	bool boss;

	quint32 nCount = 0;
	while (!out.atEnd())
	{
		out >> id >> name >> photo >> boss >> level >> exp >> hp >> mp >> DC1 >> DC2 >> MC1 >> MC2 >> AC >> MAC >> Hit >> interval;
		nCount++;
	}
	qDebug() << "The last was:" << id << name << photo << boss << level << exp << hp;
	qDebug() << "Count: " << nCount;
}

void monster(const QString &inFile, const QString &outFile)
{
	qDebug() << __FUNCTION__ << inFile << outFile;

	QStringList list;
	QString strTmp, strPath;
	qint32 id, photo, level, exp, hp, mp, DC1, DC2, MC1, MC2, AC, MAC, hit, interval;
	bool boss;
	QString name;
	quint32 nCount = 0;

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
	quint32 i;
	Rfile.readLine(1000);		//第一行是标题
	while (!Rfile.atEnd())
	{
		strTmp = Rfile.readLine(1000);
		list = strTmp.split("\t");

		i = 0;
		id = list.at(i++).toInt();
		name = list.at(i++);
		photo = list.at(i++).toInt();
		boss = (list.at(i++).toInt() >= 1);
		level = list.at(i++).toInt();
		exp = list.at(i++).toInt();
		hp = list.at(i++).toInt();
		mp = list.at(i++).toInt();
		DC1 = list.at(i++).toUInt();
		DC2 = list.at(i++).toUInt();
		MC1 = list.at(i++).toUInt();
		MC2 = list.at(i++).toUInt();
		AC = list.at(i++).toUInt();
		MAC = list.at(i++).toUInt();
		hit = list.at(i++).toUInt();
		interval = list.at(i++).toUInt();

		iData << id << name << photo << boss << level << exp << hp << mp;
		iData << DC1 << DC2 << MC1 << MC2 << AC << MAC << hit << interval;

		++nCount;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << "monster count:" << nCount;
	qDebug() << __FUNCTION__ << "run over";

	testmonster(outFile);
}