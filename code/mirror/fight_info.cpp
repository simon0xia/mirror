#include "fight_info.h"

fight_info::fight_info(QWidget *parent, quint32 time, quint32 count, quint32 exp, quint32 coin, quint32 rep)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.lbl_time->setText(QString::number(time));
	ui.lbl_count->setText(QString::number(count));
	ui.lbl_exp->setText(QString::number(exp));
	ui.lbl_coin->setText(QString::number(coin));
	ui.lbl_rep->setText(QString::number(rep));
}

fight_info::~fight_info()
{

}