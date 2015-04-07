#ifndef FIGHT_FIGHT_H
#define FIGHT_FIGHT_H

#include <QDialog>
#include "ui_fight_fight.h"
#include "RoleDefine.h"

class fight_fight : public QDialog
{
	Q_OBJECT

public:
	fight_fight(QWidget* parent, qint32 major, qint32 minor, RoleInfo *info);
	~fight_fight();

private slots:
	void on_btn_quit_clicked(void);

private:
	void Cacl_Display_Role_Value();

	void LoadMonsterSeries();
	void Load_Display_Monster_Value(qint32 id);

private:
	Ui::fight_fight ui;
	QWidget *father;
	qint32 mapID_major, mapID_minor;
	RoleInfo *myRole;

	double role_Speed;
	qint32 role_hp_c, role_mp_c, role_ap_c, role_DC, role_MC, role_SC, role_AC, role_MAC, role_penetrate;
	qint32 role_extrarate, role_extrahurt, role_rhp, role_rmp, role_rap;
};

#endif // FIGHT_FIGHT_H
