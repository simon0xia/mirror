#ifndef FIGHT_FIGHT_H
#define FIGHT_FIGHT_H

#include <QDialog>
#include <QDateTime>
#include <time.h>
#include "ui_fight_fight.h"
#include "Player.h"
#include "Monster.h"
#include "Pet.h"
#include "MonsterDefine.h"
#include "def_item_equip.h"
#include "fight_orginfo.h"

const quint32 Max_monster = 15;
const quint32 Max_MonsterLive = 4;

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
	const int nPlayerMember = 3;

public:
	fight_fight(QWidget* parent, const Info_Distribute &w_dis, CPlayer *const w_player);
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

	void DisplayRoleParameter();
	
	//读取角色基本信息，然后根据规则计算出攻击、魔法、攻速等相关信息，并显示到界面。
	void DisplayRoleinfo();

	//召唤兽/宠物
	bool SummonPet(qint32 summonID, int32_t skillLv);
	void setPetVisible(bool Visible);

	//显示当前选定怪物信息到界面
	bool GenerateMonster();

	//遭遇BOSS？
	bool EncounterBoss();

	void ShowMonsterInfo(const CMonster *mon, QLabel *lbl_name, QLabel *lbl_level, QLabel *lbl_head, QProgressBar *pbHP);
	void CalcDropBasicAndDisplay();
	void CalcDropItemsAndDisplay();

	void setVisible_monster2(bool Visible);
	void setVisible_monster3(bool Visible);
	void setVisible_monster4(bool Visible);

	//加载道具背包中的补给药品到自动喝药设置列表中
	const Info_Item* FindItem(quint32 ID);
	const Info_Item* FindItem(const QString &name);

	//回合
	void Action_role(void);
	void Action_monster(CMonster *monster);
	void Action_pet(void);

	void Step_role_Skill(void);
	bool MStep_role_Attack(const skill_fight &skill);
	bool MStep_role_Buff(const skill_fight &skill);
	bool MStep_role_Debuff(const skill_fight &skill);
	bool MStep_role_Treat(const skill_fight &skill);

	bool Init_realBuff(const skill_fight &skill, bool &bLuck, realBuff &real);

	void updateSkillCD(void);
	void ResetSkillCD();

	void MonsterDead(const CMonster *const mon, qint32 whichMonster);

	//生成自动喝药设置列表的单行显示文本
	QString Generate_ItemComboBox_Text(const QString &name, const QString &type, quint32 value, quint32 count);
	//生成单次攻击动作信息的单行显示文本
	QString Generate_Display_LineText(const QString &str1, const QString &skill, const QString &str2, bool bLuck, bool bep, QList<qint32> listDamage);

	QString Generate_Display_buffInfo(bool bLuck, const QString &targetName, const QString &SkillName, const realBuff &real);

	void round_FindMonster();
	void round_Fighting();
	void round_Rest();
	void FightFinish(FightResult fr);

private:
	Ui::fight_fight ui;

	QWidget* m_MainFrame;

	const Info_Distribute &dis;
	//qint32 m_mapID;
	uint8_t pickFilter[8];

	//背包信息
	MapItem *m_bag_item;
	ListEquip *m_bag_equip;

	QVector<skill_fight> fightingSkill;
	QVector<QLabel *> OrganismsHead;
	fight_OrgInfo *dlg_orgInfo;

	//当前地图允许一次刷新的怪物数量
	qint32 monsterCount;

	//分配到当前地图的普通怪及BOSS怪数量。
	quint32 monster_normal_count, monster_boss_count;

	//当前战斗中存活的怪物数量及它们的序号
	qint32 monsterRemainderCount, monsterRemainderIndex[5];

	CPlayer *const player;
	CMonster *monster[Max_MonsterLive];
	CPet pet;

	qint32 nFightTimer, nXSpeedTimer, nTimeOutTime;
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


