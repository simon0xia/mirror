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
	fight_map(qint32 mapID, RoleInfo *info);
	~fight_map();

private slots:
	void itemClicked(QListWidgetItem * item);

private:
	Ui::fight_map ui;

	RoleInfo *myRole;
	qint32 m_mapID;
};

#endif // FIGHT_MAP_H
