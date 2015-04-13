#include <QtCore/QtCore>

void testItem(QString inFile)
{
	qDebug() << __FUNCTION__ << inFile;

	QFile file(inFile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return;
	}

	quint32 ID, coin, gold, type, value;
	QString name, info;

	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> ID >> name >> coin >> gold >> type >> value >> info;

		qDebug() << ID << name << coin << gold << type << value << info << "\n";
	}

	file.close();
}

void item(QString inFile, QString outFile)
{
	qDebug() << __FUNCTION__ << inFile << outFile;

	QFile Rfile(inFile);
	if (!Rfile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "open " << inFile << "error.";
		return;
	}

	QFile Wfile(outFile);
	if (!Wfile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "open " << outFile << "error.";
		return;
	}

	QString strTmp;
	QStringList list;

	quint32 ID, coin, gold, type, value;
	QString name, info;

	QDataStream iData(&Wfile);

	Rfile.readLine(1000);		//第一行是标题
	while (!Rfile.atEnd())
	{
		strTmp = Rfile.readLine(1000);
		list = strTmp.split("\t");

		ID = list.at(0).toUInt();
		name = list.at(1);
		coin = list.at(2).toUInt();
		gold = list.at(3).toUInt();
		type = list.at(4).toUInt();
		value = list.at(5).toUInt();
		info = list.at(6);

		ID += 300000;	//ID等于13，或199935-200191都会出错。

		iData << ID << name << coin << gold << type << value << info;
	}

	Rfile.close();
	Wfile.close();

	qDebug() << __FUNCTION__ << "run over";

	testItem(outFile);
}