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
	void updateInfo(qint32 time, qint32 countFail, qint32 countTimeOut, qint32 countNormal, qint32 countBoss, qint32 exp, qint32 coin, qint32 rep);

private:
	Ui::fight_info ui;
};

#endif // FIGHT_INFO_H
