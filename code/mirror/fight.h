#ifndef FIGHT_H
#define FIGHT_H

#include <QWidget>
#include "ui_fight.h"
#include "fight_map.h"

class fight : public QWidget
{
	Q_OBJECT

public:
	fight(RoleInfo *roleInfo, MapRoleSkill *skill, MapItem *bag_item, ListEquip *bag_equip);
	~fight();

private:
	Ui::fight ui;

	fight_map *fightmap[mapCount_major];

	RoleInfo *myRole;
	MapItem *m_bag_item;
	ListEquip *m_bag_equip;
};

#endif // FIGHT_H
