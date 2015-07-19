#ifndef FIGHT_FIGHT_H
#define FIGHT_FIGHT_H

#include <QDialog>
#include <QTime>
#include <time.h>
#include "ui_fight_fight.h"
#include "RoleDefine.h"
#include "MonsterDefine.h"
#include "ItemDefine.h"
#include "def_item_equip.h"

#include "fight_info.h"

const quint32 Max_monster = 15;
const qint32 MaxBuffCount = 3;

class fight_fight : public QDialog
{
	Q_OBJECT
public:
	const qint32 nXSpeedInvterval = 1000;
	const int nFightInterval = 100;

public:
	fight_fight(QWidget* parent, qint32 id, RoleInfo *info, MapItem *bag_item, ListEquip *bag_equip);
	~fight_fight();

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	void timerEvent(QTimerEvent *event);
	
private slots:
	void on_checkBox_hp_clicked(void) { bCheckHp = ui.checkBox_hp->isChecked(); }
	void on_checkBox_mp_clicked(void) { bCheckMp = ui.checkBox_mp->isChecked(); }
//	void on_checkBox_auto_clicked(void);
	void on_checkBox_concise_clicked(void) { bCheckConcise = ui.checkBox_concise->isChecked(); }
	void on_checkBox_boss_clicked(void) { bCheckFindBoss = ui.checkBox_boss->isChecked(); }
	void on_btn_quit_clicked(void);
	void on_btn_statistics_clicked(void);	

	void pickFilterChange(int);

private:
	//初始化界面
	void InitUI(void);

	void levelUp();
	void CalcRoleInfo();
	void Broken32Bit(quint32 nSrc, quint8 &n1, quint8 &n2, quint8 &n3, quint8 &n4);
	//读取角色基本信息，然后根据规则计算出攻击、魔法、攻速等相关信息，并显示到界面。
	void Cacl_Display_Role_basic_info();
//	void Cacl_Display_Role_fight_info(void);

	//显示当前选定怪物信息到界面
	void GenerateMonster();
	void Display_CurrentMonsterInfo();
	void DisplayDropBasic(quint32 nDropExp, quint32 nDropCoin, quint32 nDropRep);
	void CreateEquip(itemID id, Info_Equip &equip);
	void CalcDropItemsAndDisplay(monsterID id);

	//加载道具背包中的补给药品到自动喝药设置列表中
	void LoadItem();
	const Info_Item* FindItem(quint32 ID);
	const Info_Item* FindItem(const QString &name);

	//为当前地图分配怪物
	bool AssignMonster(QVector<MonsterInfo> normalList, QVector<MonsterInfo> bossList, QMap<mapID, Info_Distribute> Distribute);

	//回合
	void Action_role(void);
	void Action_monster(void);

	//动作，每回合只能执行其中一个动作
	void Step_role_UsingItem_hp(void);
	void Step_role_UsingItem_mp(void);
	void Step_role_Skill(void);

	bool MStep_role_Buff(const skill_fight &skill);
	bool MStep_role_Attack(const skill_fight &skill);

	void updateSkillCD(void);
	void updateRoleBuffInfo(void);
	void updateMonsterBuffInfo(void);

	//杂项
	quint32 GetRoleATK(qint32 type, bool &bLuck);

	//生成自动喝药设置列表的单行显示文本
	QString Generate_ItemComboBox_Text(const QString &name, const QString &type, quint32 value, quint32 count);
	//生成单次攻击动作信息的单行显示文本
	QString Generate_Display_LineText(const QString &str1, const QString &skill, const QString &str2, bool bLuck, bool bep, QList<qint32> listDamage);

private:
	Ui::fight_fight ui;
	static bool bCheckHp, bCheckMp, bCheckConcise, bCheckFindBoss;
	static qint32 pickFilter, limit_rhp, limit_rmp;

	bool bCheckAuto;

	QWidget* m_MainFrame;

	qint32 m_mapID;
	RoleInfo *myRole;
	MapItem *m_bag_item;
	ListEquip *m_bag_equip;

	QVector<skill_fight> fightingSkill;
	QVector<realBuff> buffInRole, buffInMonster;
	QLabel *buffDisp_Role[MaxBuffCount], *buffDisp_Mon[MaxBuffCount];

	fight_info *m_dlg_fightInfo;

	MonsterInfo *monster_cur;
	quint32 monster_normal_assign[Max_monster], monster_boss_assign[Max_monster], monster_normal_count, monster_boss_count;

	qint32 Role_Lvl, role_hp_c, role_rhp, role_mp_c, role_rmp, role_ac1, role_ac2, role_mac1, role_mac2;
	qint32 monster_cur_hp, monster_cur_rhp, monster_cur_mp, monster_cur_ac, monster_cur_mac;

	bool bFighting, bBoss;
	qint32 nFightTimer, nXSpeedTimer, nShowStatusRound, nBuffer_remain, nTimeOutTime;
	qint32 nCount_attack, nCount_parry, nCount_fail, nCount_timeout, nCount_normalMonster, nCount_boss, nCount_exp, nCount_coin, nCount_rep;
	qint32 nSkillIndex;
	qint32 nElapse_pre_boss;
	double time_remain_role, time_remain_monster, time_remain;

	clock_t ct_start;
	time_t t_Count_start;
	qint32 nXSpeedCount;
	QTime xSpeedTime, Time_fight;
};

#endif // FIGHT_FIGHT_H
