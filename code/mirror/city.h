#ifndef CITY_H
#define CITY_H

#include <QWidget>
#include "ui_city.h"

#include "city_smithy.h"
#include "city_shop.h"
#include "city_mercenaries.h"

class city : public QWidget
{
	Q_OBJECT

public:
	city(RoleInfo *roleInfo, MapItem *bag_item, ListEquip *bag_equip);
	~city();
	void hideAllDlg(void);

private slots:
	void on_btn_drugstore_clicked(void);
	void on_btn_smelt_clicked(void);
	void on_btn_mercenaries_clicked(void);

private:


private:
	Ui::city ui;

	RoleInfo *myRole;
	MapItem *m_bag_item;
	ListEquip *m_bag_equip;

	city_shop *dlg_shop;
	city_smithy *dlg_smity;
	city_Mercenaries *dlg_merc;
};

#endif // CITY_H
