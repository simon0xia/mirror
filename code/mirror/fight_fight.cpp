#include "fight_fight.h"

fight_fight::fight_fight(QWidget* parent, qint32 major, qint32 minor, RoleInfo *info)
: QDialog(parent), father(parent), mapID_major(major), mapID_minor(minor), myRole(info)
{
	ui.setupUi(this);

	QString strTmp;
	if (mapID_major < 10)
	{
		strTmp += "0";
	}
	strTmp += QString::number(mapID_major);
	strTmp += QString::number(mapID_minor);
	ui.edit_display->setText(strTmp);

	Cacl_Display_Role_Value();

	LoadMonsterSeries();
	Load_Display_Monster_Value(0);
}

fight_fight::~fight_fight()
{
	
}

void fight_fight::on_btn_quit_clicked(void)
{
	this->close();
}

void fight_fight::Cacl_Display_Role_Value()
{
	ui.edit_role_name->setText(myRole->name);
	ui.edit_role_level->setText(QString::number(myRole->level));

	ui.progressBar_role_exp->setMaximum(myRole->lvExp);
	ui.progressBar_role_exp->setValue(myRole->exp);

	role_Speed = qMin(2.5, 1 + myRole->agility * 0.01);
	ui.edit_role_AttackSpeed->setText(QString::number(role_Speed));

	role_DC = myRole->level * 1.0 + myRole->strength * 1;
	ui.edit_role_PhysicsAttack->setText(QString::number(role_DC));

	role_MC = myRole->level * 0.9 + myRole->wisdom * 1;
	ui.edit_role_MagicAttack->setText(QString::number(role_MC));

	role_SC = myRole->level * 0.8 + myRole->spirit * 1;
	ui.edit_role_SpellAttack->setText(QString::number(role_SC));

	role_AC = 0;
	ui.edit_role_PhysicsGuard->setText(QString::number(role_AC));

	role_MAC = 0;
	ui.edit_role_ElementGuard->setText(QString::number(role_MAC));

	role_extrarate = 0;
	ui.edit_role_extrarate->setText(QString::number(role_extrarate));

	role_extrahurt = 0;
	ui.edit_role_extrahurt->setText(QString::number(role_extrahurt));

	ui.progressBar_role_hp->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
	ui.progressBar_role_hp->setMaximum(myRole->hp_m);
	ui.progressBar_role_hp->setValue(myRole->hp_m);
	role_hp_c = myRole->hp_m;

	ui.progressBar_role_mp->setStyleSheet("QProgressBar::chunk { background-color: rgb(0, 0, 255) }");
	ui.progressBar_role_mp->setMaximum(myRole->mp_m);
	ui.progressBar_role_mp->setValue(myRole->mp_m);
	role_mp_c = myRole->mp_m;

	ui.progressBar_role_ap->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 255, 0) }");
	ui.progressBar_role_ap->setMaximum(myRole->ap_m);
	ui.progressBar_role_ap->setValue(0);
	role_ap_c = 0;

	role_rhp = 0;
	ui.edit_role_rhp->setText(QString::number(role_rhp));

	role_rmp = 0;
	ui.edit_role_rmp->setText(QString::number(role_rmp));

	role_rap = 1;
	ui.edit_role_rap->setText(QString::number(role_rap));
}

void fight_fight::LoadMonsterSeries()
{

}
void fight_fight::Load_Display_Monster_Value(qint32 id)
{

}