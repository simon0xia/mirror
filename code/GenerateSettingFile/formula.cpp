#include <QtCore/QtCore>
#include <QImage>

void testFormula(const QString &inFile)
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

	quint32 ID, enable, m_m, m1_t, m2_t, m3_t, m4_t, m1_c, m2_c, m3_c, m4_c;

	qint32 count = 0;
	while (!out.atEnd())
	{
		out >> ID >> enable >> m_m >> m1_t >> m1_c >> m2_t >> m2_c >> m3_t >> m3_c >> m4_t >> m4_c;

		++count;
	}

	qDebug() << "find " << count << "equips define. the last equip was:";
	qDebug() << ID << enable << m_m << m1_t << m1_c << m2_t << m2_c << m3_t << m3_c << m4_t << m4_c;
}

void Formula(const QString &inFile, const QString &outFile)
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

	quint32 i, ID, enable, m_m, m1_t, m2_t, m3_t, m4_t, m1_c, m2_c, m3_c, m4_c;
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
		enable = list.at(i++).toUInt();
		m_m = list.at(i++).toUInt();
		m1_t = list.at(i++).toUInt();
		m1_c = list.at(i++).toUInt();
		m2_t = list.at(i++).toUInt();
		m2_c = list.at(i++).toUInt();
		m3_t = list.at(i++).toUInt();
		m3_c = list.at(i++).toUInt();
		m4_t = list.at(i++).toUInt();
		m4_c = list.at(i++).toUInt();

		iData << ID << enable << m_m << m1_t << m1_c << m2_t << m2_c << m3_t << m3_c << m4_t << m4_c;

		++nCount;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over. define " << nCount << "items";

	testFormula(outFile);
}