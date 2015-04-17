
#include "lvExp.h"

void lvExp(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly))
	{
		return;
	}

	QDataStream iData(&file);
	for (qint32 i = 0; i < maxLv; i++)
	{
		iData << expNeed[i];
	}

	file.close();
}