#include "city.h"
#include <QMessageBox>


city::city(CPlayer *w_player)
	: QWidget(NULL), player(w_player)
{
	ui.setupUi(this);
	dlg_shop = nullptr;
	dlg_smity = nullptr;
	dlg_merc = nullptr;
	dlg_auction = nullptr;
//	dlg_liandian = nullptr;
//	dlg_make = nullptr;
//	dlg_tower = nullptr;
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
	if (dlg_auction != nullptr)
	{
		dlg_auction->hide();
	}
// 	if (dlg_make != nullptr)
// 	{
// 		dlg_make->hide();
// 	}
// 	if (dlg_liandian != nullptr)
// 	{
// 		dlg_liandian->hide();
// 	}
// 
// 	if (dlg_tower != nullptr)
// 	{
// 		dlg_tower->hide();
// 	}
}

void city::on_btn_drugstore_clicked(void)
{
	if (dlg_shop == nullptr)
	{
		dlg_shop = new city_shop(this, player);
		dlg_shop->move(200, 80);
	}
	dlg_shop->show();
}
void city::on_btn_smelt_clicked(void)
{
	if (dlg_smity == nullptr)
	{
		dlg_smity = new city_smithy(this, player);
		dlg_smity->move(100, 100);
	}
	dlg_smity->show();
}

void city::on_btn_mercenaries_clicked(void)
{
	if (dlg_merc == nullptr)
	{
		dlg_merc = new city_Mercenaries(this, player);
		dlg_merc->move(100, 100);
	}
	dlg_merc->updateInfo();
	dlg_merc->show();
}
void city::on_btn_liandan_clicked(void)
{
	QString message = QStringLiteral("暂未开放，敬请期待");
	QMessageBox::information(this, QStringLiteral("炼丹坊"), message);
}

void city::on_btn_smithy_clicked(void)
{
// 	if (dlg_make == nullptr)
// 	{
// 		dlg_make = new city_make(this, player);
// 		dlg_make->move(100, 100);
// 	}
// 	dlg_make->updateInfo();
// 	dlg_make->show();
	QString message = QStringLiteral("暂未开放，敬请期待");
	QMessageBox::information(this, QStringLiteral("铁匠铺"), message);
}
void city::on_btn_auction_clicked(void)
{
	if (dlg_auction == nullptr)
	{
		dlg_auction = new city_auction(this, player);
		dlg_auction->move(100, 100);
	}
	dlg_auction->show();
}
void city::on_btn_tower_clicked(void)
{
// 	if (dlg_tower == nullptr)
// 	{
// 		dlg_tower = new Tower(this, player);
// 		if (!dlg_tower->init())
// 		{
// 			delete dlg_tower;
// 			dlg_tower = nullptr;
// 
// 			QString message = QStringLiteral("出错了。。。");
// 			QMessageBox::critical(this, QStringLiteral("通天塔"), message);
// 			return;
// 		}
// 		dlg_tower->move(100, 100);
// 	}
// 	dlg_tower->show();
	QString message = QStringLiteral("因为UI问题，请先去野外--挑战秘境，与通天塔设置一致。");
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