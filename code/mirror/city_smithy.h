#ifndef SMITHY_H
#define SMITHY_H

#include <QWidget>
#include "ui_city_smithy.h"
#include "RoleDefine.h"

class city_smithy : public QWidget
{
	Q_OBJECT

public:
	city_smithy(RoleInfo *roleInfo);
	~city_smithy();

private:
	Ui::smithy ui;

	RoleInfo *myRole;
};

#endif // SMITHY_H
