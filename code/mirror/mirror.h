#ifndef MIRROR_H
#define MIRROR_H

#include <QtWidgets/QMainWindow>
#include "ui_mirror.h"

#include "role.h"
#include "fight.h"
#include "city.h"

class mirror : public QMainWindow
{
	Q_OBJECT

public:
	mirror(QWidget *parent = 0);
	~mirror();

private slots:
	void tabChanged(int index);

private:
	Ui::mirrorClass ui;

	RoleInfo roleInfo;

	role m_tab_role;
	fight m_tab_fight;
	city m_tab_city;
};

#endif // MIRROR_H
