#include "city.h"

city::city(RoleInfo *roleInfo, MapItem *bag_item, ListEquip *bag_equip)
	: QWidget(NULL), myRole(roleInfo), m_bag_item(bag_item), m_bag_equip(bag_equip)
{
	ui.setupUi(this);
	dlg_shop = nullptr;
	dlg_smity = nullptr;
	dlg_merc = nullptr;
}

city::~city()
{

}
void city::hideAllDlg()
{
	if (dlg_shop != nullptr)
	{
		dlg_shop->hide();
	}
	
	if (dlg_smity != nullptr)
	{
		dlg_smity->hide();
	}
	if (dlg_merc != nullptr)
	{
		dlg_merc->hide();
	}
}

void city::on_btn_drugstore_clicked(void)
{
	if (dlg_shop == nullptr)
	{
		dlg_shop = new city_shop(this, 1, myRole, m_bag_item);
		dlg_shop->move(200, 80);
	}
	dlg_shop->show();
}
void city::on_btn_smelt_clicked(void)
{
	if (dlg_smity == nullptr)
	{
		dlg_smity = new city_smithy(this, myRole, m_bag_item, m_bag_equip);
		dlg_smity->move(100, 100);
	}
	dlg_smity->show();
}

void city::on_btn_mercenaries_clicked(void)
{
	if (dlg_merc == nullptr)
	{
		dlg_merc = new city_Mercenaries(this, myRole, m_bag_equip);
		dlg_merc->move(100, 100);
	}
	dlg_merc->show();
}