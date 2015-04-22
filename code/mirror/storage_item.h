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
	void updateItemInfo(QVector<Info_Item> &ItemList);

private:
	const Info_Item* getItem(QVector<Info_Item> &ItemList, quint32 ID);

private:
	Ui::storage_item ui;

	MapItem *m_item;
};

#endif // STORAGE_ITEM_H
