#ifndef CITY_H
#define CITY_H

#include <QWidget>
#include "ui_city.h"

#include "city_smithy.h"
#include "city_shop.h"
#include "city_mercenaries.h"
#include "city_liandan.h"
#include "city_make.h"
#include "city_auction.h"
#include "tower.h"

class city : public QWidget
{
	Q_OBJECT

public:
	city(CPlayer *w_player);
	~city();
	void hideAllDlg(void);

private slots:
	void on_btn_drugstore_clicked(void);
	void on_btn_smelt_clicked(void);
	void on_btn_mercenaries_clicked(void);
	void on_btn_liandan_clicked(void);
	void on_btn_smithy_clicked(void);
	void on_btn_auction_clicked(void);
	void on_btn_tower_clicked(void);
	void on_btn_reserved_4_clicked(void);
	void on_btn_reserved_5_clicked(void);

private:


private:
	Ui::city ui;

	CPlayer *player;

	city_shop *dlg_shop;
	city_smithy *dlg_smity;
	city_Mercenaries *dlg_merc;
	city_auction *dlg_auction;
//	city_liandan *dlg_liandian;
//	city_make *dlg_make;
//	Tower *dlg_tower;

};

#endif // CITY_H
