#ifndef FIGHT_INFO_H
#define FIGHT_INFO_H

#include <QDialog>
#include "ui_fight_info.h"

class fight_info : public QDialog
{
	Q_OBJECT

public:
	fight_info(QWidget *parent);
	~fight_info();
	void updateInfo(quint32 time, quint32 countNormal, quint32 countBoss, quint32 exp, quint32 coin, quint32 rep);

private:
	Ui::fight_info ui;
};

#endif // FIGHT_INFO_H
