#ifndef SHOP_H
#define SHOP_H

#include <QWidget>
#include "ui_city_drugs.h"
#include "ItemDefine.h"
#include "RoleDefine.h"

class shop : public QWidget
{
	Q_OBJECT

public:
	shop(qint32 type, RoleInfo *roleInfo, MapItem *bag_item);
	~shop();

private:
	//调节表格控件的大小
	void AdjustTableWidget(void);
	//显示待出售道具
	bool DisplayItemList(void);

	

private slots:
	void cellClicked(int row, int column);

private:
	Ui::drugs ui;

	qint32 m_ShopType;
	RoleInfo *myRole;
	MapItem *m_bag_item;
};

#endif // SHOP_H
