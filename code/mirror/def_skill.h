#ifndef _DEF_SKILL_H
#define _DEF_SKILL_H

#include <QPixmap>
#include "def_DataType.h"

struct Info_skill {
	skillID ID;
	QString name;
	QPixmap icon;
	quint32 type;
	quint32 level;		//技能列表中表示最大等级。角色技能中代表当前等级
	quint32 spell_basic;
	quint32 spell_add;
	quint32 cd;
	quint32 times;
	quint32 basic;
	quint32 damage_basic;
	quint32 damage_add;
	quint32 buff;
	quint32 stiff;
	QString descr;
};

struct roleSkill
{
	skillID id;
	quint32 level;
};

struct roleSkill2
{
	skillID id;
	quint32 level;
	bool Used;
};

struct skill_fight
{
	skill_fight() { 0; }
	skill_fight(const Info_skill &);
	
	skillID id;
	QString name;
	QPixmap icon;
	qint32 type;
	qint32 level;
	qint32 spell;
	qint32 cd;
	qint32 cd_c;
	qint32 times;
	qint32 basic;
	qint32 damage;
	qint32 stiff;
	qint32 buff;
};


#endif	//#ifndef _DEF_SKILL_H