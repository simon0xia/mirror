#ifndef _DEF_ITEM_EQUIP_H
#define _DEF_ITEM_EQUIP_H

#include <QUuid>

struct Info_equip {
	quint32 ID;
	QString name;
	QPixmap icon;
	quint32 type;
	quint32 ac1;
	quint32 ac2;
	quint32 mac1;
	quint32 mac2;
	quint32 dc1;
	quint32 dc2;
	quint32 mc1;
	quint32 mc2;
	quint32 sc1;
	quint32 sc2;
	quint32 need;
	quint32 needLvl;
	quint32 price;
	QString msg;
};

typedef QMap<quint32, QUuid> MapEquip;

struct EquitExtra {
	quint32 ID;
	QUuid uuid;
	quint32 level;
};

#endif //#ifndef _DEF_ITEM_EQUIP_H