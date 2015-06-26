#include "fight_info.h"

fight_info::fight_info(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);	
}

fight_info::~fight_info()
{

}

void fight_info::on_btn_close_clicked()
{
	close();
}

void fight_info::updateInfo(qint32 time, qint32 countFail, qint32 countTimeOut, qint32 countNormal, qint32 countBoss, qint32 exp, qint32 coin, qint32 rep)
{
	ui.lbl_time->setText(QString::number(time));
	ui.lbl_count_fail->setText(QString::number(countFail));
	ui.lbl_count_timeout->setText(QString::number(countTimeOut));
	ui.lbl_count_mon1->setText(QString::number(countNormal));
	ui.lbl_count_mon2->setText(QString::number(countBoss));
	ui.lbl_exp->setText(QString::number(exp));
	ui.lbl_coin->setText(QString::number(coin));
	ui.lbl_rep->setText(QString::number(rep));
}