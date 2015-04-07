#ifndef _ROLEDEFINE_H
#define _ROLEDEFINE_H

struct RoleInfo {
	QString name;
	quint64 coin;
	quint64 gold;
	quint64 reputation;
	quint64 exp;
	quint64 lvExp;
	quint32 level;
	quint32 hp_m;
	quint32 mp_m;
	quint32 ap_m;
	quint32 strength;
	quint32 wisdom;
	quint32 spirit;
	quint32 life;
	quint32 agility;
	quint32 potential;
};

struct MonsterInfo {
	QString name;
	quint32 ID;
	quint32 level;
	quint32 exp;
	quint32 hp_m;		//最大体力值
	quint32 hp_c;		//当前体力值
	quint32 hp_r;		//每次体力恢复量
	quint32 mp_m;		//最大魔法值
	quint32 mp_c;		//当前魔法值
	quint32 mp_r;		//每次魔法恢复量
	quint32 ap_m;		//最大怒气值，恒定10
	quint32 ap_c;		//当前怒气值
	quint32 ap_r;		//每次怒气恢复量,恒定1
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

#endif //#ifndef _ROLEDEFINE_H

