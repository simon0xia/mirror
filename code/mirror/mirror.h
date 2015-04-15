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
	void GiveSomeItem(void);

private:
	bool LoadItemList(const QString &dbName);
	bool LoadEquipList(const QString &dbName);

private slots:
	void tabChanged(int index);

private:
	Ui::mirrorClass ui;

	RoleInfo roleInfo;
	QMap<quint32, quint32> m_bag_item;
	QMap<quint32, quint32> m_storage_item;

	role *m_tab_role;
	fight m_tab_fight;
	city *m_tab_city;
};

#endif // MIRROR_H
