#ifndef ROLE_SKILL_H
#define ROLE_SKILL_H

#include <QDialog>
#include "ui_role_skill.h"
#include "RoleDefine.h"

class role_skill : public QDialog
{
	Q_OBJECT

	const qint32 MaxFightSkill = 5;
public:
	role_skill(QWidget *parent, const VecRoleSkill *skill_study, VecRoleSkill *skill_fight);
	~role_skill();

protected:
	void closeEvent(QCloseEvent * event);

private slots:
	void on_btn_ok_clicked(void);
	void on_btn_close_clicked(void);
	void on_btn_skill_1_clicked(void);
	void on_btn_skill_2_clicked(void);
	void on_btn_skill_3_clicked(void);
	void on_btn_skill_4_clicked(void);
	void on_btn_skill_5_clicked(void);

	void SetSkillInFighting(QListWidgetItem * item);

private:
	const Info_skill *FindSkill(skillID id);
	void DisplaySkillSequence(void);

private:
	Ui::role_skill ui;

	QVector<QPushButton *> skillSequence;
	VecRoleSkill tmpSkill_fight;
	VecRoleSkill *m_skill_fight;
	const VecRoleSkill *m_skill_study;
};

#endif // ROLE_SKILL_H
