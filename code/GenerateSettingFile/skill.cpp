#include <QtCore/QtCore>
#include <QImage>

void test_skill_basic(const QString &inFile)
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

	qint32 ID, icon, lv, cd[4], type, no;
	QString name, descr;

	while (!out.atEnd())
	{
		out >> ID >> name >> icon >> lv >> cd[0] >> cd[1] >> cd[2] >> cd[3] >> type >> no >> descr;
		qDebug() << ID << name << icon << lv << cd[0] << cd[1] << cd[2] << cd[3] << type << no << descr;
	}
}

void Skill_basic(const QString &inFile, const QString &outFile)
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

	quint32 i, ID, icon, lv, cd[4], type, no;
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
		icon = list.at(i++).toUInt();	
		lv = list.at(i++).toUInt();
		cd[0] = list.at(i++).toUInt();
		cd[1] = list.at(i++).toUInt();
		cd[2] = list.at(i++).toUInt();
		cd[3] = list.at(i++).toUInt();
		type = list.at(i++).toUInt();
		no = list.at(i++).toUInt();
		descr = list.at(i++);

		iData << ID << name << icon << lv << cd[0] << cd[1] << cd[2] << cd[3] << type << no << descr;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	test_skill_basic(outFile);
}


void test_skill_buff(const QString &inFile)
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

	qint32 ID, icon, time, type, targets, effectType, basic, add;
	bool control;
	QString name;

	while (!out.atEnd())
	{
		out >> ID >> name >> icon >> time >> control >> type >> targets >> effectType >> basic >> add;
		qDebug() << ID << name << icon << time << control << type << targets << effectType << basic << add;
	}
}

void skill_buff(const QString &inFile, const QString &outFile)
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

	QString strTmp, name;
	QStringList list;

	qint32 i, ID, icon, time, type, targets, effectType, basic, add;
	bool control;

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
		ID = list.at(i++).toInt();
		name = list.at(i++);
		icon = list.at(i++).toInt();
		time = list.at(i++).toInt();
		control = (list.at(i++).toInt() == 1);
		type = list.at(i++).toInt();
		targets = list.at(i++).toInt();
		effectType = list.at(i++).toInt();
		basic = list.at(i++).toInt(); 
		add = list.at(i++).toInt();

		iData << ID << name << icon << time << control << type << targets << effectType << basic << add;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	test_skill_buff(outFile);
}

void test_skill_damage(const QString &inFile)
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

	qint32 ID, type, targets, times, extra, basic, add;

	while (!out.atEnd())
	{
		out >> ID >> type >> targets >> times >> extra >> basic >> add;
		qDebug() << ID << type << targets << times << extra << basic << add;
	}
}

void skill_damage(const QString &inFile, const QString &outFile)
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

	qint32 i, ID, type, targets, times, extra, basic, add;

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
		ID = list.at(i++).toInt();
		type = list.at(i++).toInt();
		targets = list.at(i++).toInt();
		times = list.at(i++).toInt();
		extra = list.at(i++).toInt();
		basic = list.at(i++).toInt();
		add = list.at(i++).toInt();

		iData << ID << type << targets << times << extra << basic << add;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	test_skill_damage(outFile);
}

void test_skill_summon(const QString &inFile)
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

	qint32 ID, photo, type, hp, damage1, damage2, defense;

	while (!out.atEnd())
	{
		out >> ID >> photo >> type >> hp >> damage1 >> damage2 >> defense;
		qDebug() << ID << photo << type << hp << damage1 << damage2 << defense;
	}
}

void skill_summon(const QString &inFile, const QString &outFile)
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

	qint32 i, ID, photo, type, hp, damage1, damage2, defense;

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
		ID = list.at(i++).toInt();
		photo = list.at(i++).toInt();
		type = list.at(i++).toInt();
		hp = list.at(i++).toInt();
		damage1 = list.at(i++).toInt();
		damage2 = list.at(i++).toInt();
		defense = list.at(i++).toInt();

		iData << ID << photo << type << hp << damage1 << damage2 << defense;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	test_skill_summon(outFile);
}



void test_skill_treat(const QString &inFile)
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

	qint32 ID, targets, hpr_basic, hpr_add;
	QString name;

	while (!out.atEnd())
	{
		out >> ID >> name >> targets >> hpr_basic >> hpr_add;
		qDebug() << ID << name << targets << hpr_basic << hpr_add;
	}
}

void skill_treat(const QString &inFile, const QString &outFile)
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

	QString strTmp, name;
	QStringList list;

	qint32 i, ID, targets, hpr_basic, hpr_add;

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
		ID = list.at(i++).toInt();
		name = list.at(i++);
		targets = list.at(i++).toInt();
		hpr_basic = list.at(i++).toInt();
		hpr_add = list.at(i++).toInt();

		iData << ID << name << targets << hpr_basic << hpr_add;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	test_skill_treat(outFile);
}