#ifndef CITY_H
#define CITY_H

#include <QWidget>
#include "ui_city.h"
#include "mytabframe.h"

#include "shop.h"
#include "smithy.h"

class city : public myTabFrame
{
	Q_OBJECT

public:
	city(RoleInfo *roleInfo, MapItem *bag_item);
	~city();
	virtual void updateRoleInfo(void);

private slots:
	void tabChanged(int index);

private:


private:
	Ui::city ui;

	RoleInfo *myRole;
	MapItem *m_bag_item;

	shop *m_drugs;
	shop *m_variety;
	smithy *m_smithy;
};

#endif // CITY_H
