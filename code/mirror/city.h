#ifndef CITY_H
#define CITY_H

#include <QWidget>
#include "ui_city.h"
#include "mytabframe.h"

class city : public myTabFrame
{
	Q_OBJECT

public:
	city(RoleInfo *roleInfo);
	~city();
	virtual void updateRoleInfo(void);

private:
	Ui::city ui;

	RoleInfo *myRole;
};

#endif // CITY_H
