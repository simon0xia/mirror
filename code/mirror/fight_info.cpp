#include "fight_info.h"

fight_info::fight_info(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);	
}

fight_info::~fight_info()
{

}

void fight_info::updateInfo(quint32 time, quint32 countNormal, quint32 countBoss, quint32 exp, quint32 coin, quint32 rep)
{
	ui.lbl_time->setText(QString::number(time));
	ui.lbl_count1->setText(QString::number(countNormal));
	ui.lbl_count2->setText(QString::number(countBoss));
	ui.lbl_exp->setText(QString::number(exp));
	ui.lbl_coin->setText(QString::number(coin));
	ui.lbl_rep->setText(QString::number(rep));
}