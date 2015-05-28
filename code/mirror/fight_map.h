#ifndef FIGHT_MAP_H
#define FIGHT_MAP_H

#include <QWidget>
#include "ui_fight_map.h"
#include "RoleDefine.h"
#include "MapDefine.h"
#include "fight_fight.h"

class fight_map : public QWidget
{
	Q_OBJECT

public:
	fight_map(qint32 mapID, RoleInfo *info, MapItem *bag_item, ListEquip *bag_equip);
	~fight_map();

private slots:
	void itemClicked(QListWidgetItem * item);

private:
	void timerEvent(QTimerEvent *event);

private:
	Ui::fight_map ui;
	qint32 deleyTimer;
	fight_fight *m_dlg_fightfight;

	RoleInfo *myRole;
	MapItem *m_bag_item;
	ListEquip *m_bag_equip;
	qint32 m_mapID;					//Ö÷µØÍ¼ID
};

#endif // FIGHT_MAP_H
