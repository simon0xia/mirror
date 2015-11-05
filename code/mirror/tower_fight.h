#ifndef TOWER_FIGHT_H
#define TOWER_FIGHT_H

#include <QDialog>
#include "ui_tower_fight.h"

#include "Player.h"
#include "Monster.h"
#include "Pet.h"

struct TowerDistributeInfo
{
	quint32 id;
	quint32 cost;
	quint32 monster1;
	quint32 monster2;
	quint32 monster3;
};

class tower_fight : public QDialog
{
	Q_OBJECT

private:
	const int nFightInterval = 100;

public:
	tower_fight(QWidget* parent, qint32 w_towerLv, const TowerDistributeInfo &w_dis, CPlayer *const w_player);
	~tower_fight();
	bool getFightRes(void) { return bSuccess; }

private slots:
	void on_btn_quit_clicked(void);
	void on_btn_start_clicked(void);

private:
	void timerEvent(QTimerEvent *event);

	bool loadDropSet();
	void InitUI();
	void DisplayRoleinfo();

	void DisplayDropSetting();

	//’ŸªΩ ﬁ/≥ËŒÔ
	void SummonPet(const skill_fight &skill);
	void PetDead(void);
	void setVisible_pet(bool Visible);
	void UpdatePetParameter();

	void setVisible_monster2(bool Visible);
	void setVisible_monster3(bool Visible);

	void MonsterDead();

	void Step_role_Skill(void);
	bool MStep_role_Attack(const skill_fight &skill);
	bool MStep_role_Buff(const skill_fight &skill);
	bool MStet_role_Debuff(const skill_fight &skill);

	bool Init_realBuff(const skill_fight &skill, bool &bLuck, realBuff &real);
	void updateSkillCD();

	//ªÿ∫œ
	void Action_role(void);
	void Action_pet(void);
	void Action_monster(CMonster *monster);

	void GenerateMonster();
	void ShowMonsterInfo(const CMonster& mon, QLabel *lbl_name, QLabel *lbl_level, QLabel *lbl_head, QProgressBar *pbHP, QProgressBar *pbMP);

	QString Generate_Display_LineText(const QString &str1, const QString &skill, const QString &str2, bool bLuck, bool bep, QList<qint32> listDamage);
	QString Generate_Display_buffInfo(bool bLuck, const QString &SkillName, const realBuff &real);
		
public:
	const qint32 MaxBuffCount = 4;

private:
	Ui::tower_fight ui;
	bool bSuccess;

	CPlayer *const player;
	CMonster monster[3];
	CPet pet;

	MapItem *m_bag_item;

	qint32 towerLv, monsterCount, nSkillIndex, whichMonster;
	QList<realBuff> buffInRole, buffInMonster;

	QVector<QVector<quint32>> drop;
	const TowerDistributeInfo &dis;

	QLabel *buffDisp_Role[4], *buffDisp_pet[4], *buffDisp_Mon1[4], *buffDisp_Mon2[4], *buffDisp_Mon3[4];
	QVector<skill_fight> fightingSkill;
	skill_fight basicSkill;

	qint32 nFightTimer;
	qint32 nTimeOutTime;
	double  time_remain;
};

#endif // TOWER_FIGHT_H
