#ifndef BAG_ITEM_H
#define BAG_ITEM_H

#include <QWidget>
#include "ui_bag_item.h"
#include "ItemDefine.h"

class bag_item : public QWidget
{
	Q_OBJECT

public:
	bag_item(MapItem *item);
	~bag_item();
	void updateItemInfo(QVector<Info_Item> &ItemList);

private:
	Info_Item* getItem(QVector<Info_Item> &ItemList, quint32 ID);
	

private:
	Ui::bag_item ui;

	MapItem *m_item;
};

#endif // BAG_ITEM_H
