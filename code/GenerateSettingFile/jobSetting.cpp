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

	Info_jobAdd job;
	quint32 count;

	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> count;
		while (count--)
		{
			out >> job.level >> job.hp >> job.mp >> job.dc1 >> job.dc2 >> job.mc1 >> job.mc2
				>> job.sc1 >> job.sc2 >> job.ac1 >> job.ac2 >> job.mac1 >> job.mac2;

			qDebug() << job.level << job.hp << job.mp << job.dc1 << job.dc2 << job.mc1 << job.mc2
				<< job.sc1 << job.sc2 << job.ac1 << job.ac2 << job.mac1 << job.mac2;
		}
	}

	file.close();
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

		QVector<Info_jobAdd> vec;
		Info_jobAdd job;

		RFile.readLine(1000);		//第一行是标题
		while (!RFile.atEnd())
		{
			strTmp = RFile.readLine(1000);
			list = strTmp.split("\t");

			job.level = list.at(0).toUInt();
			job.hp = list.at(1).toUInt();
			job.mp = list.at(2).toUInt();
			job.dc1 = list.at(3).toUInt();
			job.dc2 = list.at(4).toUInt();
			job.mc1 = list.at(5).toUInt();
			job.mc2 = list.at(6).toUInt();
			job.sc1 = list.at(7).toUInt();
			job.sc2 = list.at(8).toUInt();
			job.ac1 = list.at(9).toUInt();
			job.ac2 = list.at(10).toUInt();
			job.mac1 = list.at(11).toUInt();
			job.mac2 = list.at(12).toUInt();

			vec.append(job);
		}
		RFile.close();

		iData << vec.size();	//单个职业设定文件长度。
		for (quint32 i = 0; i < vec.size(); i++)
		{
			iData << vec[i].level << vec[i].hp << vec[i].mp << vec[i].dc1 << vec[i].dc2 << vec[i].mc1 << vec[i].mc2
				<< vec[i].sc1 << vec[i].sc2 << vec[i].ac1 << vec[i].ac2 << vec[i].mac1 << vec[i].mac2;
		}
	}
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	testjobSet(outFile);
}
