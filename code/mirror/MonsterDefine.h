#ifndef _MONSTERDEFINE_H
#define _MONSTERDEFINE_H

#include <QImage>

struct MonsterInfo {
	quint32 ID;
	QString name;
	QImage Head;
	qint32 level;
	qint32 exp;
	qint32 hp_m;		//最大体力值
	qint32 hp_c;		//当前体力值
	qint32 hp_r;		//每次体力恢复量
	qint32 mp_m;		//最大魔法值
	qint32 mp_c;		//当前魔法值
	qint32 mp_r;		//每次魔法恢复量
	qint32 ap_m;		//最大怒气值，恒定10
	qint32 ap_c;		//当前怒气值
	qint32 ap_r;		//每次怒气恢复量,恒定1
	quint32 DC;			//物攻
	quint32 MC;			//魔攻
	quint32 SC;			//道攻
	quint32 AC;			//物防
	quint32 MAC;		//魔防
	quint32 extrarate;	
	quint32 extrahurt;
	quint32 penetrate;
	double	Speed;		//攻击速度
};

#endif //#ifndef _MONSTERDEFINE_H

