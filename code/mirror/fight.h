#ifndef FIGHT_H
#define FIGHT_H

#include <QWidget>
#include "ui_fight.h"
#include "mytabframe.h"
#include "fight_map.h"

class fight : public myTabFrame
{
	Q_OBJECT

public:
	fight(RoleInfo *roleInfo);
	~fight();

public:
	virtual void updateRoleInfo(void);

private slots:
	

private:
	Ui::fight ui;

	fight_map *fightmap[7];

	RoleInfo *myRole;
};

#endif // FIGHT_H
