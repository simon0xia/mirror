#ifndef SMITHY_H
#define SMITHY_H

#include <QWidget>
#include "ui_city_smithy.h"
#include "RoleDefine.h"

class smithy : public QWidget
{
	Q_OBJECT

public:
	smithy(RoleInfo *roleInfo);
	~smithy();

private:
	Ui::smithy ui;

	RoleInfo *myRole;
};

#endif // SMITHY_H
