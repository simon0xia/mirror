#include "cryptography.h"

cryptography::cryptography(QObject *parent)
	: QObject(parent)
{

}

cryptography::~cryptography()
{

}

/*
a、计算有效存档的MD5码。
b、对a的结果进行base64编码，得到44字节文本。每4字节编为一组，共11组。
c、随机生成一段2000字节的杂乱文本（ascii 65-122)。
d、取c文本的第100*n字节的ascii码 - 65作为起始位置，覆盖写入c结果的第n组到c中。
e、改写后的c结果共2000字节作为存档的起始头。
*/
bool cryptography::Encrypt(QByteArray &dst, const QByteArray &src)
{
	dst.clear();

	QByteArray MD5arr = QCryptographicHash::hash(src, QCryptographicHash::Md5).toHex();
	QByteArray TmpArr1,TmpArr2;
	QString strTmp;
	char ch;
	int nIndex;

 	TmpArr1 = MD5arr.toBase64();
 
	for (qint32 i = 0; i < 2000; i++)
	{
		ch = qrand() % (122 - 65) + 65;
		TmpArr2.append(ch);
	}

	for (qint32 i = 0; i < 11; i++)
	{
		nIndex = TmpArr2.at(i * 100) - 60 + i * 100;
		TmpArr2.replace(nIndex, 4, TmpArr1.mid(i * 4, 4));
	}

	dst = TmpArr2;
	return true;
}
bool cryptography::Decrypt(QByteArray &dst, const QByteArray &src)
{
	if (src.size() < 2000)
	{
		return false;
	}
	dst.clear();

	QByteArray MD5arr;
	QByteArray TmpArr1, TmpArr2;
	QString strTmp;
	int nIndex;

	for (qint32 i = 0; i < 11; i++)
	{
		nIndex = src.at(i * 100) - 60 + i * 100;
		TmpArr2.append(src.mid(nIndex, 4));
	}

	TmpArr1 = QByteArray::fromBase64(TmpArr2);

	MD5arr = TmpArr1;
	dst = MD5arr;

	return true;
}
