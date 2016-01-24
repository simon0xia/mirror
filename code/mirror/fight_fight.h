#ifndef FIGHT_FIGHT_H
#define FIGHT_FIGHT_H

#include <QDialog>
#include <QDateTime>
#include <time.h>
#include "ui_fight_fight.h"
#include "Player.h"
#include "Monster.h"
#include "pet.h"

#include "fight_orginfo.h"

const quint32 Max_monster = 15;
const quint32 Max_MonsterLive = 4;
const int nPlayerMember = 3;

enum RoundType
{
	RT_FindMonster,
	RT_Fighting,
	RT_Rest
};

enum FightResult
{
	FR_victory,
	FR_fail,
	FR_draw
};

class fight_fight : public QDialog
{
	Q_OBJECT
public:
	const qint32 nXSpeedInvterval = 1000;
	const int nFightInterval = 100;
	

public:
	fight_fight(QWidget* parent, const Info_Distribute &w_dis);
	~fight_fight();

protected:
	void keyPressEvent(QKeyEvent *event);
	bool eventFilter(QObject *obj, QEvent *ev);

private:
	void timerEvent(QTimerEvent *event);

	void DisplayStatistics();

	void InitSystemConfigure(void);


private slots:
	void on_btn_quit_clicked(void);

private:
	//初始化界面
	void InitUI(void);

	//读取角色基本信息，然后根据规则计算出攻击、魔法、攻速等相关信息，并显示到界面。
	void DisplayRoleinfo(const CHuman *edt);
	void DisplayRoleParameter(const CHuman *edt);

	//读取当前英雄的基本信息并显示。
	void DisplayEmbodimentInfo(const CHuman *edt);
	void DisplayEmbodimentParameter(const CHuman *edt);

	//召唤兽/宠物
	void setPetVisible(bool Visible);
	void SetPetPos(bool hasEdt);

	//显示当前选定怪物信息到界面
	bool GenerateMonster();

	//遭遇BOSS？
	bool EncounterBoss();

	void ShowMonsterInfo(const CMonster *mon, QLabel *lbl_name, QLabel *lbl_level, QLabel *lbl_head, QProgressBar *pbHP);
	void CalcDropBasicAndDisplay();
	void CalcDropItemsAndDisplay();

	void setEdtVisible(bool Visible);
	void setVisible_monster2(bool Visible);
	void setVisible_monster3(bool Visible);
	void setVisible_monster4(bool Visible);

	//加载道具背包中的补给药品到自动喝药设置列表中
	const Info_Item* FindItem(itemID ID);
	const Info_Item* FindItem(const QString &name);

	qint32 RestTime();

	//回合
	void Action(COrganisms *org);

	bool Step_Attack(COrganisms *org, const SkillFight &skill);			//攻击步
	bool Step_Attack_La(COrganisms *attacker, const SkillFight &skill);		//La阵营(角色)攻击Rb阵营(怪物)
	bool Step_Attack_Rb(COrganisms *attacker, const SkillFight &skill);		//Rb阵营(怪物)攻击La阵营(角色)

	bool Step_Summon(COrganisms *org, const SkillFight &skill);

	bool MStep_role_Buff(COrganisms *org, const SkillFight &skill);
	bool MStep_role_Debuff(COrganisms *org, const SkillFight &skill);
	bool Step_role_Treat(COrganisms *org, const SkillFight &skill);

	bool Init_realBuff(COrganisms *org, const SkillFight &skill, bool &bLuck, realBuff &real);

	//生成单次攻击动作信息的单行显示文本
	QString Generate_Display_LineText(const QString &str1, const QString &skill, const QString &str2, bool bLuck, bool bep, QList<qint32> listDamage);

	QString Generate_Display_buffInfo(const QString &name1, bool bLuck, const QString &targetName, const QString &SkillName, const realBuff &real);
	QString Generate_Display_DebuffInfo(const QString &name1, bool bLuck, const QString &targetName, const QString &SkillName, const realBuff &real);

	void round_FindMonster();
	void round_Fighting();
	void round_Rest();
	void FightFinish(FightResult fr);

private:
	Ui::fight_fight ui;

	const Info_Distribute &dis;
	//qint32 m_mapID;
	uint8_t pickFilter[8];

	//背包信息
	MapItem *m_bag_item;
	ListEquip *m_bag_equip;

	QVector<QLabel *> OrganismsHead;
	fight_OrgInfo *dlg_orgInfo;

	//当前地图允许一次刷新的怪物数量
	qint32 monsterCount;

	//分配到当前地图的普通怪及BOSS怪数量。
	quint32 monster_normal_count, monster_boss_count;

	//当前战斗中存活的怪物数量
	qint32 monsterRemainderCount;

	CMonster *monster[Max_MonsterLive];
	CHuman *edt_role, *edt_edt;
	CPet pet;

	QVector<COrganisms *> camps_La, camps_Rb;
	bool bHasEdt, bHasPet;

	qint32 nFightTimer, nXSpeedTimer;
	qint32 nCount_totalWar, nCount_victory, nCount_normalMonster, nCount_boss, nCount_exp, nCount_coin, nCount_items;
	qint32 nSkillIndex;
	qint32 time_remain, time_findMonster;
	qint32 nRound;
	RoundType rt;

	QDateTime ct_start;
	time_t t_Count_start;
	qint32 nXSpeedCount;
	QTime xSpeedTime, Time_fight;
};

#endif // FIGHT_FIGHT_H


