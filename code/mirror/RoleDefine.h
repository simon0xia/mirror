#ifndef _ROLEDEFINE_H
#define _ROLEDEFINE_H

const QString def_vocation[] = {
	QString::fromLocal8Bit("无职业"),
	QString::fromLocal8Bit("战士"),
	QString::fromLocal8Bit("法师"),
	QString::fromLocal8Bit("道士")
};

struct Info_jobAdd {
	quint32 level;			//等级
	quint32 hp;				//血
	quint32 mp;				//蓝
	quint32 dc1;			//攻击_下限
	quint32 dc2;			//攻击_上限
	quint32 mc1;			//魔法
	quint32 mc2;			//魔法
	quint32 sc1;			//道术
	quint32 sc2;			//道术
	quint32 ac1;			//物防
	quint32 ac2;			//物防
	quint32 mac1;			//魔防
	quint32 mac2;			//魔防
};
typedef QMap<quint32, QVector<Info_jobAdd>> mapJobAdd;

struct RoleInfo {
	QString name;			//角色名
	quint32 vocation;		//职业
	quint64 coin;			//金币
	quint64 gold;			//元宝
	quint64 reputation;		//声望
	quint64 exp;			//当前经验值
	quint64 lvExp;			//当前升级所需经验值
	quint32 level;			//等级
	quint32 hp;				//血
	quint32 mp;				//蓝
	quint32 dc1;			//攻击_下限
	quint32 dc2;			//攻击_上限
	quint32 mc1;			//魔法
	quint32 mc2;			//魔法
	quint32 sc1;			//道术
	quint32 sc2;			//道术
	quint32 ac1;			//物防
	quint32 ac2;			//物防
	quint32 mac1;			//魔防
	quint32 mac2;			//魔防
	quint32 intervel;		//攻击间隔
	quint32 luck;			//幸运
	quint32 strength;		//属性点：力量
	quint32 wisdom;			//属性点：智慧
	quint32 spirit;			//属性点：精神
	quint32 life;			//属性点：生命
	quint32 agility;		//属性点：敏捷
	quint32 potential;		//潜能点。用于分配到对应属性点。
};


#endif //#ifndef _ROLEDEFINE_H

