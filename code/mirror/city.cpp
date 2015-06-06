#include "city.h"

city::city(RoleInfo *roleInfo, MapItem *bag_item)
	: QWidget(NULL), myRole(roleInfo), m_bag_item(bag_item)
{
	ui.setupUi(this);
	dlg_shop = nullptr;
	dlg_smity = nullptr;
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
		dlg_smity = new city_smithy(this, myRole);
		dlg_smity->move(100, 100);
	}
	dlg_smity->show();
}
