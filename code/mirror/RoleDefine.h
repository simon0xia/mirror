#ifndef _ROLEDEFINE_H
#define _ROLEDEFINE_H

#include <QPixmap>
#include "def_DataType.h"
#include "def_System_para.h"
#include "def_item_equip.h"


const QString def_vocation[4] = {
	QStringLiteral("无职业"),
	QStringLiteral("战士"),
	QStringLiteral("法师"),
	QStringLiteral("道士")
};


struct info_buff {
	qint32 ID;
	QString name;
	qint32 time;
	qint32 rhp;
	qint32 ac;
	qint32 mac;
};
typedef QVector<info_buff> vecBuff;

struct realBuff
{
	quint32 id;
	QString name;
	QPixmap icon;
	qint32 time;
	qint32 rhp;
	qint32 ac;
	qint32 mac;
};

struct Info_skill {
	skillID ID;
	QString name;
	QPixmap icon;
	quint32 type;
	quint32 level;		//技能列表中表示最大等级。角色技能中代表当前等级
	quint32 spell[3];
	quint32 cd;
	quint32 times;
	quint32 damage[3];	//0：1级技能加成 1:2级技能加成 2:3级技能加成
	quint32 buff;
	QString descr;
};
struct skill_fight
{
	skillID id;
	QString name;
	QPixmap icon;
	qint32 type;
	qint32 level;
	qint32 spell;
	qint32 cd;
	qint32 cd_c;
	qint32 times;
	qint32 damage;
	qint32 buff;
};
struct roleSkill
{
	skillID id;
	quint32 level;
};

typedef QVector<roleSkill> VecRoleSkill;

struct Info_jobAdd {
	qint32 level;		//等级
	qint64 exp;			//等级所需经验
	qint32 hp;			//血
	qint32 mp;			//蓝
	qint32 dc1;			//攻击_下限
	qint32 dc2;			//攻击_上限
	qint32 mc1;			//魔法
	qint32 mc2;			//魔法
	qint32 sc1;			//道术
	qint32 sc2;			//道术
	qint32 ac1;			//物防
	qint32 ac2;			//物防
	qint32 mac1;		//魔防
	qint32 mac2;		//魔防
};
typedef QMap<quint32, QVector<Info_jobAdd>> mapJobAdd;

struct RoleInfo {
	char name[128];			//角色名
	quint32 vocation;		//职业
	quint32 gender;			//性别
	quint64 coin;			//金币
	quint64 gold;			//元宝
	quint64 reputation;		//声望
	quint64 exp;			//当前经验值
	quint64 lvExp;			//当前升级所需经验值
	quint32 level;			//等级
//	quint32 hp;				//血
	quint8 hp_1;
	quint8 mark_1;
	quint8 hp_2;
	quint8 mark_2;
	quint8 hp_3;
	quint8 mark_3;
	quint8 hp_4;
	quint8 mark_4;
//	quint32 mp;				//蓝
	quint8 mp_1;
	quint8 mark_5;
	quint8 mp_2;
	quint8 mark_6;
	quint8 mp_3;
	quint8 mark_7;
	quint8 mp_4;
	quint8 mark_8;
//	quint32 dc1;			//攻击_下限
//	quint32 dc2;			//攻击_上限
	quint8 dc1_1;
	quint8 mark_9;
	quint8 dc1_2;
	quint8 mark_10;
	quint8 dc1_3;
	quint8 mark_11;
	quint8 dc1_4;
	quint8 mark_12;
	quint8 dc2_1;
	quint8 mark_13;
	quint8 dc2_2;
	quint8 mark_14;
	quint8 dc2_3;
	quint8 mark_15;
	quint8 dc2_4;
	quint8 mark_16;
//	quint32 mc1;			//魔法
//	quint32 mc2;			//魔法
	quint8 mc1_1;
	quint8 mark_17;
	quint8 mc1_2;
	quint8 mark_18;
	quint8 mc1_3;
	quint8 mark_19;
	quint8 mc1_4;
	quint8 mark_20;
	quint8 mc2_1;
	quint8 mark_21;
	quint8 mc2_2;
	quint8 mark_22;
	quint8 mc2_3;
	quint8 mark_23;
	quint8 mc2_4;
	quint8 mark_24;
//	quint32 sc1;			//道术
//	quint32 sc2;			//道术
	quint8 sc1_1;
	quint8 mark_25;
	quint8 sc1_2;
	quint8 mark_26;
	quint8 sc1_3;
	quint8 mark_27;
	quint8 sc1_4;
	quint8 mark_28;
	quint8 sc2_1;
	quint8 mark_29;
	quint8 sc2_2;
	quint8 mark_30;
	quint8 sc2_3;
	quint8 mark_31;
	quint8 sc2_4;
	quint8 mark_32;
//	quint32 ac1;			//物防
//	quint32 ac2;			//物防
	quint8 ac1_1;
	quint8 mark_33;
	quint8 ac1_2;
	quint8 mark_34;
	quint8 ac1_3;
	quint8 mark_35;
	quint8 ac1_4;
	quint8 mark_36;
	quint8 ac2_1;
	quint8 mark_37;
	quint8 ac2_2;
	quint8 mark_38;
	quint8 ac2_3;
	quint8 mark_39;
	quint8 ac2_4;
	quint8 mark_40;
//	quint32 mac1;			//魔防
//	quint32 mac2;			//魔防
	quint8 mac1_1;
	quint8 mark_41;
	quint8 mac1_2;
	quint8 mark_42;
	quint8 mac1_3;
	quint8 mark_43;
	quint8 mac1_4;
	quint8 mark_44;
	quint8 mac2_1;
	quint8 mark_45;
	quint8 mac2_2;
	quint8 mark_46;
	quint8 mac2_3;
	quint8 mark_47;
	quint8 mac2_4;
	quint8 mark_48;
//	quint32 intervel;		//攻击间隔
	quint8 intervel_1;
	quint8 mark_49;
	quint8 intervel_2;
	quint8 mark_50;
	quint8 luck;			//幸运
	quint8 mark_51;
	VecRoleSkill skill;	//战斗时使用的技能
};

struct roleAddition
{
	quint32 strength;		//属性点：力量
	quint32 wisdom;			//属性点：智慧
	quint32 spirit;			//属性点：精神
	quint32 life;			//属性点：生命
	quint32 agility;		//属性点：敏捷
	quint32 potential;		//潜能点。用于分配到对应属性点。

	Info_Equip vecEquip[MaxEquipCountForRole];	//角色身上的装备

	quint32 taskStep;
	quint32 reserved1[100];
	quint32 reserved2[100];
	quint32 reserved3[100];
	quint32 reserved4[80];
};

#endif //#ifndef _ROLEDEFINE_H

