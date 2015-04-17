#ifndef STORAGE_ITEM_H
#define STORAGE_ITEM_H

#include <QWidget>
#include "ui_storage_item.h"
#include "ItemDefine.h"

class storage_item : public QWidget
{
	Q_OBJECT

public:
	storage_item(MapItem *item);
	~storage_item();
	void updateItemInfo(QVector<ItemInfo> &ItemList);

private:
	ItemInfo* getItem(QVector<ItemInfo> &ItemList, quint32 ID);

private:
	Ui::storage_item ui;

	MapItem *m_item;
};

#endif // STORAGE_ITEM_H
