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
	qint32 icon;
	qint32 lv;
	qint32 luck;
	qint32 spd;	//速度
	qint32 hp;
	qint32 ac;
	qint32 mac;
	qint32 dc1;
	qint32 dc2;
	qint32 mc1;
	qint32 mc2;
	qint32 sc1;
	qint32 sc2;
	qint32 need;
	qint32 needLvl;
	qint32 price;
};

struct Info_StateEquip
{
	QPixmap img;
	qint32 offset_x;
	qint32 offset_y;
};

enum EquipExtraType
{
	//固定值
	eet_fixed_hp,
	eet_fixed_mp,
	eet_fixed_hpr,
	eet_fixed_mpr,
	eet_fixed_dc,
	eet_fixed_mc,
	eet_fixed_sc,
	eet_fixed_ac,
	eet_fixed_mac,
	eet_fixed_spd,
	eet_fixed_luck,

	//百分比
	//eet_percent_hp,
	//eet_percent_mp,
	//eet_percent_hpr,
	//eet_percent_mpr,
	eet_percent_dc = 15,
	eet_percent_mc,
	eet_percent_sc,
	eet_percent_ac,
	eet_percent_mac,

	eet_limit
};

struct EquipExtra2
{
	EquipExtraType eet;
	qint32 value;
};

struct Info_Equip {
	itemID ID;
	qint32 lvUp;
	qint32 extraAmount;
	EquipExtra2 extra[4];
};
typedef QList<Info_Equip> ListEquip;

// struct info_formula {
// 	itemID ID;
// 	qint32	npc;
// 	itemID	m_m;
// 	itemID	m1_t;
// 	qint32	m1_c;
// 	itemID	m2_t;
// 	qint32	m2_c;
// 	itemID	m3_t;
// 	qint32	m3_c;
// 	itemID	m4_t;
// 	qint32	m4_c;
// };
//
// struct info_formula_liandan {
// 	itemID ID;
// 	itemID	m1_t;
// 	qint32	m1_c;
// 	itemID	m2_t;
// 	qint32	m2_c;
// 	itemID	m3_t;
// 	qint32	m3_c;
// 	itemID	m4_t;
// 	qint32	m4_c;
// 	itemID	m5_t;
// 	qint32	m5_c;
// };

#endif //#ifndef _DEF_ITEM_EQUIP_H