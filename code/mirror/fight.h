#ifndef FIGHT_H
#define FIGHT_H

#include <QDialog>
#include "ui_fight.h"
#include "fight_map.h"

class fight : public QDialog
{
	Q_OBJECT

public:
	fight(QWidget *parent);
	~fight();
	qint32 get_SelectMap(void) const { return currentMap; }

private slots:
	void on_SelectMap(qint32 id);

private:
	Ui::fight ui;

	fight_map *map_normal, *map_limit;
	quint32 CountOfMapType;
	qint32 currentMap;
};

#endif // FIGHT_H
