#include <QtCore\QtCore>


void testChenhao(const QString &inFile)
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

	quint32 ID, need, dc1, dc2, mc1, mc2, sc1, sc2;
	QString name;

	while (!out.atEnd())
	{
		out >> ID >> name >> need >> dc1 >> dc2 >> mc1 >> mc2 >> sc1 >> sc2;

	}
	qDebug() << ID << name << need << dc1 << dc2 << mc1 << mc2 << sc1 << sc2;

	file.close();
}

void Chenhao(const QString &inFile, const QString &outFile)
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

	QDataStream iData(&Wfile);

	QString strTmp;
	QStringList list;
	int i = 0;
	quint32 ID, need, dc1, dc2, mc1, mc2, sc1, sc2;
	QString name;

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

		need = list.at(i++).toUInt();
		dc1 = list.at(i++).toUInt();
		dc2 = list.at(i++).toUInt();
		mc1 = list.at(i++).toUInt();
		mc2 = list.at(i++).toUInt();
		sc1 = list.at(i++).toUInt();
		sc2 = list.at(i++).toUInt();

		iData << ID << name << need << dc1 << dc2 << mc1 << mc2 << sc1 << sc2;
	}
	printf_s("\n");

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	testChenhao(outFile);
}
