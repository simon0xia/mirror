#ifndef _ROLEDEFINE_H
#define _ROLEDEFINE_H

struct RoleInfo {
	QString name;			//角色名
	quint64 coin;			//金币
	quint64 gold;			//元宝
	quint64 reputation;		//声望
	quint64 exp;			//当前经验值
	quint64 lvExp;			//当前升级所需经验值
	quint32 level;			//等级
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


#endif //#ifndef _ROLEDEFINE_H

