#ifndef ROLE_SKILL_H
#define ROLE_SKILL_H

#include <QDialog>
#include "ui_role_skill.h"
#include "RoleDefine.h"

class role_skill : public QDialog
{
	Q_OBJECT

	const qint32 MaxFightSkill = 10;
public:
	role_skill(QWidget *parent, const VecRoleSkill *skill_study, VecRoleSkill *skill_fight);
	~role_skill();

protected:
	void closeEvent(QCloseEvent * event);

private slots:
	void on_btn_ok_clicked(void);
	void on_btn_close_clicked(void);
	void on_btn_skill_1_clicked(void) { btnIndex = 1;  ProcessSkillBtn(); }
	void on_btn_skill_2_clicked(void) { btnIndex = 2;  ProcessSkillBtn(); }
	void on_btn_skill_3_clicked(void) { btnIndex = 3;  ProcessSkillBtn(); }
	void on_btn_skill_4_clicked(void) { btnIndex = 4;  ProcessSkillBtn(); }
	void on_btn_skill_5_clicked(void) { btnIndex = 5;  ProcessSkillBtn(); }
	void on_btn_skill_6_clicked(void) { btnIndex = 6;  ProcessSkillBtn(); }
	void on_btn_skill_7_clicked(void) { btnIndex = 7;  ProcessSkillBtn(); }
	void on_btn_skill_8_clicked(void) { btnIndex = 8;  ProcessSkillBtn(); }
	void on_btn_skill_9_clicked(void) { btnIndex = 9;  ProcessSkillBtn(); }
	void on_btn_skill_10_clicked(void) { btnIndex = 10;  ProcessSkillBtn(); }

	void SetSkillInFighting(QListWidgetItem * item);

private:
	const Info_skill *FindSkill(skillID id);
	void ProcessSkillBtn(void);
	void DisplaySkillSequence(void);

private:
	Ui::role_skill ui;
	qint32 btnIndex;

	QVector<QPushButton *> skillSequence;
	VecRoleSkill tmpSkill_fight;
	VecRoleSkill *m_skill_fight;
	const VecRoleSkill *m_skill_study;
};

#endif // ROLE_SKILL_H
