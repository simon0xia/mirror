#ifndef CITY_H
#define CITY_H

#include <QWidget>
#include "ui_city.h"
#include "mytabframe.h"

#include "drugs.h"
#include "smithy.h"

class city : public myTabFrame
{
	Q_OBJECT

public:
	city(RoleInfo *roleInfo, QMap<quint32, quint32> &bag_item);
	~city();
	virtual void updateRoleInfo(void);

private slots:
	void tabChanged(int index);

private:


private:
	Ui::city ui;

	RoleInfo *myRole;
	QMap<quint32, quint32> m_bag_item;

	drugs *m_drugs;
	smithy m_smithy;
};

#endif // CITY_H
