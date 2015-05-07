#ifndef _ROLEDEFINE_H
#define _ROLEDEFINE_H

#include <QPixmap>
#include "def_DataType.h"

const qint32
	SaveFileVer = 3,
	MaxEquipCountForRole = 12;
const QString SaveFileName = "save.sxv";

const QString def_vocation[4] = {
	QStringLiteral("无职业"),
	QStringLiteral("战士"),
	QStringLiteral("法师"),
	QStringLiteral("道士")
};


struct info_buff {
	quint32 ID;
	QString name;
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
};

const info_buff def_info_buff[6] = {
	{ 0, QStringLiteral("无"), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1, QStringLiteral("幽灵盾"), 0, 0, 2, 4, 0, 0, 0, 0, 0, 0 },
	{ 2, QStringLiteral("神圣战甲术"), 2, 4, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 3, QStringLiteral("魔法盾"), 2, 4, 2, 4, 0, 0, 0, 0, 0, 0 },
	{ 4, QStringLiteral("召唤骷髅"), 1, 2, 1, 2, 2, 4, 0, 0, 0, 0 },
	{ 5, QStringLiteral("召唤神兽"), 2, 3, 2, 3, 0, 0, 0, 0, 3, 5 }
};

struct Info_skill {
	skillID ID;
	QString name;
	QPixmap icon1;
	QPixmap icon2;
	quint32 level;		//技能列表中表示最大等级。角色技能中代表当前等级
	quint32 spell[3];
	quint32 times;
	quint32 damage[3];	//0：1级技能加成 1:2级技能加成 2:3级技能加成
	quint32 buff;
	quint32 buff_time;
	QString descr;
};

struct roleSkill
{
	skillID id;
	quint32 level;
};

typedef QVector<roleSkill> VecRoleSkill;

struct Info_jobAdd {
	quint32 level;			//等级
	quint32 hp;				//血
	quint32 mp;				//蓝
	quint32 dc1;			//攻击_下限
	quint32 dc2;			//攻击_上限
	quint32 mc1;			//魔法
	quint32 mc2;			//魔法
	quint32 sc1;			//道术
	quint32 sc2;			//道术
	quint32 ac1;			//物防
	quint32 ac2;			//物防
	quint32 mac1;			//魔防
	quint32 mac2;			//魔防
};
typedef QMap<quint32, QVector<Info_jobAdd>> mapJobAdd;

struct RoleInfo {
	QString name;			//角色名
	quint32 vocation;		//职业
	quint32 gender;
	quint64 coin;			//金币
	quint64 gold;			//元宝
	quint64 reputation;		//声望
	quint64 exp;			//当前经验值
	quint64 lvExp;			//当前升级所需经验值
	quint32 level;			//等级
	quint32 hp;				//血
	quint32 mp;				//蓝
	quint32 dc1;			//攻击_下限
	quint32 dc2;			//攻击_上限
	quint32 mc1;			//魔法
	quint32 mc2;			//魔法
	quint32 sc1;			//道术
	quint32 sc2;			//道术
	quint32 ac1;			//物防
	quint32 ac2;			//物防
	quint32 mac1;			//魔防
	quint32 mac2;			//魔防
	quint32 intervel;		//攻击间隔
	quint32 luck;			//幸运
	quint32 strength;		//属性点：力量
	quint32 wisdom;			//属性点：智慧
	quint32 spirit;			//属性点：精神
	quint32 life;			//属性点：生命
	quint32 agility;		//属性点：敏捷
	quint32 potential;		//潜能点。用于分配到对应属性点。
	quint32 equip[MaxEquipCountForRole];	//角色身上的装备
	VecRoleSkill skill;	//战斗时使用的技能
};

#endif //#ifndef _ROLEDEFINE_H

