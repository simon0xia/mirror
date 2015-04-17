#ifndef FIGHT_FIGHT_H
#define FIGHT_FIGHT_H

#include <QDialog>
#include "ui_fight_fight.h"
#include "RoleDefine.h"
#include "MonsterDefine.h"
#include "ItemDefine.h"

const quint32 Max_monster = 100;

class fight_fight : public QDialog
{
	Q_OBJECT

public:
	fight_fight(QWidget* parent, qint32 id, RoleInfo *info, MapItem *bag_item);
	~fight_fight();

private:
	void timerEvent(QTimerEvent *event);

private slots:
	void on_btn_quit_clicked(void);
	void on_btn_start_clicked(void);


private:
	//读取角色基本信息，然后根据规则计算出攻击、魔法、攻速等相关信息，并显示到界面。
	void Cacl_Display_Role_Value();

	//加载怪物分布信息数据库
	void LoadDistribute();
	//加载普通怪信息数据库
	void LoadMonster();
	//加载BOSS怪信息数据库
	void LoadBoss();
	//显示当前选定怪物信息到界面
	void Display_CurrentMonsterInfo();

	//加载道具背包中的补给药品到自动喝药设置列表中
	void LoadItem();
	ItemInfo* FindItem(quint32 ID);
	ItemInfo* FindItem(const QString &name);

	//回合
	void Action_role(void);
	void Action_monster(void);

	//动作，每回合只能执行其中一个动作
	void Step_role_UsingItem_hp(void);
	void Step_role_UsingItem_mp(void);
	void Step_role_UsingItem_ap(void);
	void Step_role_NormalAttack(void);
	void Step_role_SkillAttack(void);
	void Step_role_BoostAccack(void);

	//生成自动喝药设置列表的单行显示文本
	QString Generate_ItemComboBox_Text(const QString &name, const QString &type, quint32 value, quint32 count);
	//生成单次攻击动作信息的单行显示文本
	QString Generate_Display_LineText(const QString &str1, const QString &skill, const QString &str2, quint32 damage);

private:
	Ui::fight_fight ui;
	qint32 m_mapID;
	RoleInfo *myRole;
	MapItem *m_bag_item;
	MonsterInfo monsterArr[Max_monster], BossArr[Max_monster], *monster_cur;
	quint32 monster_count, Boss_count;
	quint32 monster_id_start, monster_id_stop, boss_id_start, boss_id_stop;

	double role_Speed;
	qint32 role_hp_c, role_mp_c, role_ap_c, role_DC, role_MC, role_SC, role_AC, role_MAC, role_penetrate;
	qint32 role_extrarate, role_extrahurt, role_rhp, role_rmp, role_rap;

	bool bFighting, bKeepFight;
	qint32 nFightTimer, nDelayTimer, nShowStatusRound, nRound, nCount_attack, nCount_parry, nCount_item;
	double time_remain_role, time_remain_monster, time_remain;
};

#endif // FIGHT_FIGHT_H
