#include <QtCore/QtCore>
#include <QImage>

void testSkill(const QString &inFile)
{
	qDebug() << __FUNCTION__ << inFile;

	QFile file(inFile);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	QImage img1, img2;
	quint32 ID, lv, spell1, spell2, spell3, cd, times, damage1, damage2, damage3, buff, buff_time;
	QString name, descr;

	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> ID >> name >> img1 >> img2 >> lv >> spell1 >> spell2 >> spell3 >> cd >> times >> damage1 >> damage2 >> damage3 >> buff >> buff_time >> descr;

		qDebug() << ID << name << img1.isDetached() << img2.isDetached() << lv << spell1 << spell2 << spell3 << cd << times << damage1 << damage2 << damage3 << buff << buff_time << descr;
	}

	file.close();
}

void Skill(const QString &inFile, const QString &outFile)
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

	QImage img1,img2;
	quint32 i, ID, photo, lv, spell[3], cd, times, damage[3], buff, buff_time;
	QString name, descr, strImgPath1, strImgPath2;

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

		strImgPath1 = QString("./Resources/skill/");
		strImgPath1 += QString::number(photo) + QString("0.bmp");
		strImgPath2 = QString("./Resources/skill/");
		strImgPath2 += QString::number(photo) + QString("1.bmp");
		if (!QFile::exists(strImgPath1) || !QFile::exists(strImgPath2))
		{
			qDebug() << "Cannot find file." << strImgPath1 << strImgPath2;
			break;
		}
		img1 = QImage(strImgPath1);
		img2 = QImage(strImgPath2);
		if (img1.isNull() || img2.isNull())
		{
			qDebug() << "No Head:" << strImgPath1 << strImgPath2;
			break;
		}
		lv = list.at(i++).toUInt();
		spell[0] = list.at(i++).toUInt();
		spell[1] = list.at(i++).toUInt();
		spell[2] = list.at(i++).toUInt();
		cd = list.at(i++).toUInt();
		times = list.at(i++).toUInt();
		damage[0] = list.at(i++).toUInt();
		damage[1] = list.at(i++).toUInt();
		damage[2] = list.at(i++).toUInt();
		buff = list.at(i++).toUInt();
		buff_time = list.at(i++).toUInt();
		descr = list.at(i++);

		iData << ID << name << img1 << img2 << lv << spell[0] << spell[1] << spell[2] << cd << times << damage[0] << damage[1] << damage[2] << buff << buff_time << descr;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	testSkill(outFile);
}