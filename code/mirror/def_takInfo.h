#ifndef _DEF_TASKINFO_H
#define _DEF_TASKINFO_H

#include "def_DataType.h"

struct info_task
{
	itemID requireItem;
	quint32 requireCount;
	QVector<itemID> giveItem;
	quint32 giveCount;
	QString msg;
};

#endif //#ifndef _DEF_TASKINFO_H