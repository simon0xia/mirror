#ifndef _ITEMDEFINE_H
#define _ITEMDEFINE_H

#include "def_DataType.h"

enum EffectType {
	et_NoEffect=0,
	et_immediate_coin = 1000,
	et_immediate_gold,
	et_immediate_reputation,
	et_immediate_level,
	et_immediate_exp,
	et_immediate_strength,
	et_immediate_wisdom,
	et_immediate_spirit,
	et_immediate_life,
	et_immediate_agility,
	et_immediate_potential,
	et_immediate_hp = 1100,
	et_immediate_mp,
	et_Skill = 5001,
};

struct Info_Item {
	itemID ID; 
	QString name;
	qint32 icon;
	qint32 vocation;
	qint32 level;
	qint32 coin;
	qint32 gold;
	EffectType type;
	qint32 value;
	QString descr;
};

typedef QMap<itemID, qint32> MapItem;

#endif //#ifndef _ITEMDEFINE_H