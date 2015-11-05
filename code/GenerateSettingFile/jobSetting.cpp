#include <QtCore\QtCore>
#include "..\mirror\RoleDefine.h"

void testjobSet(const QString &inFile)
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

	Info_jobAdd job;
	quint32 count, nCount;

	while (!out.atEnd())
	{
		nCount = 0;
		out >> count;
		while (count--)
		{
			out >> job.level >> job.exp >> job.hp >> job.mp >> job.dc1 >> job.dc2 >> job.mc1 >> job.mc2
				>> job.sc1 >> job.sc2 >> job.ac >> job.mac;

			++nCount;
		}

		qDebug() << "\nCurrent job has " << nCount << "line defines.\n The last define was:";
		qDebug() << job.level << job.exp << job.hp << job.mp << job.dc1 << job.dc2 << job.mc1 << job.mc2
			<< job.sc1 << job.sc2 << job.ac << job.mac;
	}

}

void jobSet(const QStringList &jobSetFiles, const QString &outFile)
{
	qDebug() << __FUNCTION__ << jobSetFiles.at(0) << jobSetFiles.at(1) << jobSetFiles.at(2) << outFile;

	QFile Wfile(outFile);
	if (!Wfile.open(QIODevice::WriteOnly))
	{
		qDebug() << "open " << outFile << "error.";
		return;
	}
	QDataStream iData(&Wfile);

	foreach(QString FilePath, jobSetFiles)
	{
		QFile RFile(FilePath);
		if (!RFile.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qDebug() << "open " << FilePath << "error.";
			return;
		}

		QString strTmp;
		QStringList list;
		qint32 i;

		QVector<Info_jobAdd> vec;
		Info_jobAdd job;

		RFile.readLine(1000);		//第一行是标题
		while (!RFile.atEnd())
		{
			i = 0;
			strTmp = RFile.readLine(1000);
			list = strTmp.split("\t");

			job.level = list.at(i++).toInt();
			job.exp = list.at(i++).toLongLong();
			job.hp = list.at(i++).toInt();
			job.mp = list.at(i++).toInt();
			job.dc1 = list.at(i++).toInt();
			job.dc2 = list.at(i++).toInt();
			job.mc1 = list.at(i++).toInt();
			job.mc2 = list.at(i++).toInt();
			job.sc1 = list.at(i++).toInt();
			job.sc2 = list.at(i++).toInt();
			job.ac = list.at(i++).toInt();
			job.mac = list.at(i++).toInt();

			vec.append(job);
		}
		RFile.close();

		iData << vec.size();	//单个职业设定文件长度。
		for (quint32 i = 0; i < vec.size(); i++)
		{
			iData << vec[i].level << vec[i].exp << vec[i].hp << vec[i].mp << vec[i].dc1 << vec[i].dc2 << vec[i].mc1 << vec[i].mc2
				<< vec[i].sc1 << vec[i].sc2 << vec[i].ac << vec[i].mac;
		}
	}
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	testjobSet(outFile);
}
