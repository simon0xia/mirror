#ifndef CITY_H
#define CITY_H

#include <QWidget>
#include "ui_city.h"
#include "mytabframe.h"

#include "city_shop.h"
#include "city_smithy.h"

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

	city_shop *m_drugs;
	city_shop *m_variety;
	city_smithy *m_smithy;
};

#endif // CITY_H
