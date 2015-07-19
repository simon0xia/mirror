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

	QByteArray documentContent = file.readAll();
	file.close();

	QByteArray MD5arr = QCryptographicHash::hash(documentContent, QCryptographicHash::Md5).toHex();
	qDebug() << "MD5:" << MD5arr.data();

	QDataStream out(documentContent);

	QImage img;
	quint32 ID, type, lv, spell1, spell2, spell3, cd, times, basic, damage1, damage2, damage3, buff, stiff;
	QString name, descr;

	while (!out.atEnd())
	{
		out >> ID >> name >> img >> type >> lv >> spell1 >> spell2 >> spell3 >> cd
			>> times >> basic >> damage1 >> damage2 >> damage3 >> buff >> stiff >> descr;

		qDebug() << ID << name << img.isDetached() << type << lv << spell1 << spell2 << spell3 << cd
			<< times << basic << damage1 << damage2 << damage3 << buff << stiff << descr;
	}
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

	QImage img;
	quint32 i, ID, photo, type, lv, spell[3], cd, times, basic,damage[3], buff, stiff;
	QString name, descr, strImgPath;

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

		strImgPath = QString("./Resources/skill/");
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
		type = list.at(i++).toUInt();
		lv = list.at(i++).toUInt();
		spell[0] = list.at(i++).toUInt();
		spell[1] = list.at(i++).toUInt();
		spell[2] = list.at(i++).toUInt();
		cd = list.at(i++).toUInt();
		times = list.at(i++).toUInt();
		basic = list.at(i++).toUInt();
		damage[0] = list.at(i++).toUInt();
		damage[1] = list.at(i++).toUInt();
		damage[2] = list.at(i++).toUInt();
		buff = list.at(i++).toUInt();
		stiff = list.at(i++).toUInt();
		descr = list.at(i++);

		iData << ID << name << img << type << lv << spell[0] << spell[1] << spell[2] << cd 
			<< times << basic<< damage[0] << damage[1] << damage[2] << buff << stiff << descr;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	testSkill(outFile);
}