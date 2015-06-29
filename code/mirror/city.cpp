#include "city.h"
#include <QMessageBox>

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

void city::on_btn_reserved_1_clicked(void)
{
	QString message = QStringLiteral("暂未开放，敬请期待");
	QMessageBox::information(this, QStringLiteral("铁匠铺"), message);
}
void city::on_btn_reserved_2_clicked(void)
{
	QString message = QStringLiteral("暂未开放，敬请期待");
	QMessageBox::information(this, QStringLiteral("拍卖行"), message);
}
void city::on_btn_reserved_3_clicked(void)
{
	QString message = QStringLiteral("暂未开放，敬请期待");
	QMessageBox::information(this, QStringLiteral("通天塔"), message);
}
void city::on_btn_reserved_4_clicked(void)
{
	QString message = QStringLiteral("暂未开放，敬请期待");
	QMessageBox::information(this, QStringLiteral("演武场"), message);

}
void city::on_btn_reserved_5_clicked(void)
{
	QString message = QStringLiteral("暂未开放，敬请期待");
	QMessageBox::information(this, QStringLiteral("皇宫"), message);
}