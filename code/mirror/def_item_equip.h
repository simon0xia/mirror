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
	quint32 luck;
	quint32 acc;	//准确
	quint32 ag;		//敏捷	
	quint32 spd;	//速度
	quint32 md;		//魔法躲避
	quint32 ep;		//暴击概率
	quint32 ed;		//暴击伤害
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
};

struct Info_StateEquip
{
	QPixmap img;
	qint32 offset_x;
	qint32 offset_y;
};

struct EquipExtra {
	quint32 acc;			//历史遗留，以后找机会去掉。
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


struct info_formula {
	itemID ID;
	quint32	npc;
	itemID	m_m;
	itemID	m1_t;
	quint32	m1_c;
	itemID	m2_t;
	quint32	m2_c;
	itemID	m3_t;
	quint32	m3_c;
	itemID	m4_t;	
	quint32	m4_c;
};

#endif //#ifndef _DEF_ITEM_EQUIP_H