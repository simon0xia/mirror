#ifndef SMITHY_H
#define SMITHY_H

#include <QWidget>
#include "ui_city_smithy.h"
#include "RoleDefine.h"

class city_smithy : public QWidget
{
	Q_OBJECT

public:
	city_smithy(QWidget *parrent, RoleInfo *roleInfo);
	~city_smithy();

private slots:
	void on_btn_close_clicked(void);

private:
	Ui::smithy ui;

	RoleInfo *myRole;
};

#endif // SMITHY_H
