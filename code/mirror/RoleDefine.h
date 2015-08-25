#ifndef _ROLEDEFINE_H
#define _ROLEDEFINE_H

#include <QPixmap>
#include "def_DataType.h"
#include "def_System_para.h"
#include "def_item_equip.h"
#include "def_skill.h"

const QString def_vocation[4] = {
	QStringLiteral("无职业"),
	QStringLiteral("战士"),
	QStringLiteral("法师"),
	QStringLiteral("道士")
};

const QString def_xiulian[12] = {
	QStringLiteral("凝气"),
	QStringLiteral("筑基"),
	QStringLiteral("结丹"),
	QStringLiteral("元婴"),
	QStringLiteral("化神"),
	QStringLiteral("婴变"),
	QStringLiteral("问鼎"),
	QStringLiteral("阴虚"),
	QStringLiteral("阳实"),
	QStringLiteral("窥涅"),
	QStringLiteral("净涅"),
	QStringLiteral("碎涅")
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

typedef QMap<skillID, roleSkill2> MapRoleSkill;

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

#endif //#ifndef _ROLEDEFINE_H

