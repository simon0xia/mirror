#ifndef CITY_AUCTION_H
#define CITY_AUCTION_H

#include <QWidget>
#include "ui_city_auction.h"
#include "Item_Base.h"

class city_auction : public QWidget
{
	Q_OBJECT

public:
	city_auction(QWidget *w_parent, CPlayer *const w_player);
	~city_auction();

protected:
	void mousePressEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent * ev);
	void mouseReleaseEvent(QMouseEvent *ev);

private:
	//显示待出售道具
	bool InitSales(void);
	bool DisplayItemList(void);

private slots:
	void on_btn_close_clicked(void);
	void itemClicked(QListWidgetItem * item);
	void itemDoubleClicked(QListWidgetItem * item);

private:
	Ui::city_auction ui;
	QWidget *parrent;

	CPlayer *const player;
	MapItem *m_bag_item;

	QPoint mouseMovePos;

	QMap<itemID, uint32_t> sales;
};

#endif // CITY_AUCTION_H
