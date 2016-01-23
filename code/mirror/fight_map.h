#ifndef FIGHT_MAP_H
#define FIGHT_MAP_H

#include <QWidget>
#include "ui_fight_map.h"

class fight_map : public QWidget
{
	Q_OBJECT

public:
	fight_map(qint32 mapType);
	~fight_map();

private slots:
	void itemClicked(QListWidgetItem * item);

private:
	void DisplayMap();

signals:
	void SelectMap(qint32 id);

private:
	Ui::fight_map ui;
	qint32 mapType;
};

#endif // FIGHT_MAP_H
