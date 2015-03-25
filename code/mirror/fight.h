#ifndef FIGHT_H
#define FIGHT_H

#include <QWidget>
#include "ui_fight.h"

class fight : public QWidget
{
	Q_OBJECT

public:
	fight(QWidget *parent = 0);
	~fight();

private:
	Ui::fight ui;
};

#endif // FIGHT_H
