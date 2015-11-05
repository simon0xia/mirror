#ifndef TOWER_H
#define TOWER_H

#include <QWidget>
#include "ui_tower.h"


#include "tower_fight.h"

class Tower : public QWidget
{
	Q_OBJECT

public:
	Tower(QWidget *parent, CPlayer *const w_player);
	~Tower();
	bool init();

private slots:
	void itemClicked(QListWidgetItem * item);

private:
	bool loadDistrubute();

private:
	Ui::Tower ui;

	QVector<TowerDistributeInfo> dis;

	tower_fight *m_dlg_fightfight;

	CPlayer *const player;
	int MaxLevel;
};

#endif // TOWER_H
