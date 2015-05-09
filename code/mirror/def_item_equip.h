#ifndef _DEF_ITEM_EQUIP_H
#define _DEF_ITEM_EQUIP_H

#include <QUuid>
#include <QPixmap>

#include "def_DataType.h"

const QString equip_need_info[4] = {
	QStringLiteral("需要等级"),
	QStringLiteral("需要攻击"),
	QStringLiteral("需要魔法"),
	QStringLiteral("需要道术")
};

struct Info_basic_equip {
	itemID ID;
	QString name;
	QPixmap icon;
	quint32 lv;
	quint32 acc;	//准确
	quint32 luck;	
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

struct EquipExtra {
	quint32 acc;
	quint32 luck;
	quint32 ac;
	quint32 mac;
	quint32 dc;
	quint32 mc;
	quint32 sc;
};

struct Info_Equip {
//	QUuid uuid;
	itemID ID;
	quint32 lvUp;
	quint32 extraAmount;
	EquipExtra extra;
};
typedef QList<Info_Equip> ListEquip;

#endif //#ifndef _DEF_ITEM_EQUIP_H