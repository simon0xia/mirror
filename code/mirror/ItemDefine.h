#ifndef _ITEMDEFINE_H
#define _ITEMDEFINE_H

enum EffectType {
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
	et_multiple_reputation = 5000,
	et_multiple_exp,
	et_multiple_drop
};

struct Info_Item {
	quint32 ID; 
	QString name;
	QPixmap icon;
	quint32 sale;
	quint32 level;
	quint32 coin;
	quint32 gold;
	EffectType type;
	quint32 value;
	QString descr;
	QString msg;
};

typedef QMap<quint32, quint32> MapItem;

#endif //#ifndef _ITEMDEFINE_H