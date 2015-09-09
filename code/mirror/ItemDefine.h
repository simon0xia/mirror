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
	et_skill = 5001,
	et_lucky,
	et_ResetPotential = 5101,
	et_Level100
};

struct Info_Item {
	itemID ID; 
	QString name;
	QPixmap icon;
	quint32 vocation;
	quint32 level;
	quint32 sale;	
	quint32 coin;
	quint32 gold;
	EffectType type;
	quint32 value;
	QString descr;
};

typedef QMap<itemID, qint32> MapItem;

#endif //#ifndef _ITEMDEFINE_H