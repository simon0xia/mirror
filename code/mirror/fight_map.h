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
	fight_map(qint32 mapID, RoleInfo *info, MapItem *bag_item);
	~fight_map();

private slots:
	void itemClicked(QListWidgetItem * item);

private:
	Ui::fight_map ui;

	RoleInfo *myRole;
	MapItem *m_bag_item;
	qint32 m_mapID;					//主地图ID

	bool bHasBoss[mapCount_minor];	//数组，标记某个地图为普通怪，或BOSS怪。
};

#endif // FIGHT_MAP_H
