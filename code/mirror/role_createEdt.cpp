#include "role_createEdt.h"
#include <QMessageBox>
#include "Player.h"

role_createEdt::role_createEdt(QWidget *parent, Vocation w_voc)
	: QDialog(parent), voc(w_voc)
{
	ui.setupUi(this);

	ui.comboBox_voc->setCurrentIndex(w_voc);
}


role_createEdt::~role_createEdt()
{

}

void role_createEdt::on_btn_quit_clicked()
{
	done(QDialog::Rejected);
}

void role_createEdt::on_btn_ok_clicked(void)
{
	QString name = ui.edit_name->text();
	int32_t gender = ui.radio_male->isChecked() ? 1 : 2;
	CHuman *edt = nullptr;
	int32_t spend;

	if (name.isEmpty() || name.isEmpty())
	{
		QMessageBox::critical(this, QStringLiteral("出错啦"), QStringLiteral("名字不能为空。"));
		return;
	}
	spend = (voc == PlayerIns.get_edt_role().get_voc()) ? 500000 : 1500000;
	if (PlayerIns.get_coin() < spend)
	{
		QMessageBox::critical(this, QStringLiteral("出错啦"), QStringLiteral("金币不足。"));
		return;
	}
	PlayerIns.sub_coin(spend);

	switch (voc)
	{
	case Voc_Unknow: 	break; //nothing 
	case Voc_Warrior: edt = &PlayerIns.get_edt_warrior(); break;
	case Voc_Magic: edt = &PlayerIns.get_edt_magic(); break;
	case Voc_Taoist: edt = &PlayerIns.get_edt_taoshi(); break;
	default:
		//nothing
		break;
	}

	if (edt != nullptr)
	{
		edt->set_BasicInfo(name, gender, voc);
		edt->set_levelInfo(1, 0);
		edt->updateParameter();
		done(QDialog::Accepted);
	}	
}