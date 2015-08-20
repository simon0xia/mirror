#ifndef _MONSTERDEFINE_H
#define _MONSTERDEFINE_H

#include <QImage>
#include <QIcon>
#include "def_DataType.h"

struct MonsterInfo {
	monsterID ID;
	QString name;
	QImage Head;
	qint32 level;
	qint32 exp;
	qint32 hp;			//最大体力值
	qint32 mp;			//最大魔法值
	qint32 DC1;		//物攻下限
	qint32 DC2;		//物攻上限
	qint32 MC1;		//魔攻下限
	qint32 MC2;		//魔攻上限
	qint32 AC;			//物防
	qint32 MAC;		//魔防
	qint32	interval;	//攻击间隔
};

struct Info_Distribute {
	mapID ID;
	QString name;
	QIcon img;
	qint32 need_lv;
	qint32 expend_rep;
	qint32 expend_item;
	QVector<monsterID> normal;
	QVector<monsterID> boss;
};

struct Rational {
	itemID ID;
	//quint32 num;		//始终为1
	quint32 den;
};
typedef QList<Rational> ListDrop;
typedef QMap<monsterID, ListDrop>  mapDrop;

#endif //#ifndef _MONSTERDEFINE_H

