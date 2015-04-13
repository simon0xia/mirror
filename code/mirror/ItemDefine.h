#ifndef _ITEMDEFINE_H
#define _ITEMDEFINE_H

#include <QUuid>

struct ItemItem {
	quint32 ID;
	quint32 count;
};

struct ItemEquip {
	quint32 ID;
	QUuid uuid;
};

const quint32
	BagCapacity = 40,
	StorageCapacity = 40;

#endif //#ifndef _ITEMDEFINE_H