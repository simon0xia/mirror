#ifndef SHOP_H
#define SHOP_H

#include <QWidget>
#include "ui_city_drugs.h"
#include "ItemDefine.h"
#include "RoleDefine.h"

class city_shop : public QWidget
{
	Q_OBJECT

public:
	city_shop(QWidget *parent, qint32 type, RoleInfo *roleInfo, MapItem *bag_item);
	~city_shop();

protected:
	void mouseMoveEvent(QMouseEvent * ev);

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
	RoleInfo *myRole;
	MapItem *m_bag_item;
};

#endif // SHOP_H
