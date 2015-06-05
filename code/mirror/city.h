#ifndef CITY_H
#define CITY_H

#include <QWidget>
#include "ui_city.h"

#include "city_smithy.h"
#include "city_shop.h"

class city : public QWidget
{
	Q_OBJECT

public:
	city(RoleInfo *roleInfo, MapItem *bag_item);
	~city();
	void hideAllDlg(void);

private slots:
	void on_btn_drugstore_clicked(void);
	void on_btn_smelt_clicked(void);

private:


private:
	Ui::city ui;

	RoleInfo *myRole;
	MapItem *m_bag_item;

	city_shop *dlg_shop;
	city_smithy *dlg_smity;
};

#endif // CITY_H
