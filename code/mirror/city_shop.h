#ifndef SHOP_H
#define SHOP_H

#include <QWidget>
#include "ui_city_drugs.h"
#include "Item_Base.h"

class city_shop : public QWidget
{
	Q_OBJECT

public:
	city_shop(QWidget *parent, CPlayer *const w_player);
	~city_shop();

protected:
	void mousePressEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent * ev);
	void mouseReleaseEvent(QMouseEvent *ev);

private:
	//显示待出售道具
	bool DisplayItemList(void);

private slots:
	void itemClicked(QListWidgetItem * item);
	void itemDoubleClicked(QListWidgetItem * item);
	void on_btn_close_clicked(void);

private:
	Ui::drugs ui;
	QWidget *m_parrent;

	qint32 m_ShopType;
	CPlayer *const player;
	MapItem *m_bag_item;

	QPoint mouseMovePos;
};

#endif // SHOP_H
