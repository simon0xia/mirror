#ifndef _MONSTERDEFINE_H
#define _MONSTERDEFINE_H

#include <QImage>

struct MonsterInfo {
	quint32 ID;
	QString name;
	QImage Head;
	qint32 level;
	qint32 exp;
	qint32 hp;			//最大体力值
	qint32 mp;			//最大魔法值
	quint32 DC1;		//物攻下限
	quint32 DC2;		//物攻上限
	quint32 MC1;		//魔攻下限
	quint32 MC2;		//魔攻上限
	quint32 AC;			//物防
	quint32 MAC;		//魔防
	quint32	interval;//攻击间隔
};

struct Info_Distribute {
	quint32 mapID;
	QVector<quint32> normal;
	QVector<quint32> boss;
};

#endif //#ifndef _MONSTERDEFINE_H

