#ifndef BAG_ITEM_H
#define BAG_ITEM_H

#include <QWidget>
#include "ui_bag_item.h"
#include "ItemDefine.h"

typedef QMap<quint32, quint32> MapItem;

class bag_item : public QWidget
{
	Q_OBJECT

public:
	bag_item(MapItem &item);
	~bag_item();
	void updateItemInfo(QList<ItemInfo> &ItemList, MapItem item);

private:
	ItemInfo* getItem(QList<ItemInfo> &ItemList, quint32 ID);
	

private:
	Ui::bag_item ui;

	MapItem m_item;
};

#endif // BAG_ITEM_H
