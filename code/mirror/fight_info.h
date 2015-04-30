#ifndef FIGHT_INFO_H
#define FIGHT_INFO_H

#include <QWidget>
#include "ui_fight_info.h"

class fight_info : public QWidget
{
	Q_OBJECT

public:
	fight_info(QWidget *parent, quint32 time, quint32 count, quint32 exp, quint32 coin, quint32 rep);
	~fight_info();

private:
	Ui::fight_info ui;
};

#endif // FIGHT_INFO_H
