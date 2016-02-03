#ifndef ITEM_EQUIPSTORAGE_H
#define ITEM_EQUIPSTORAGE_H

#include "Item_Base.h"

class Item_equipStorage : public Item_Base
{
	Q_OBJECT

public:
	Item_equipStorage(QWidget *parent = 0);
	~Item_equipStorage();

	public slots:
	void updateInfo(void);

private slots:
	void ShowItemInfo(const QModelIndex &index);
	void TakeOutEquip(void);

private:
	ListEquip *m_item;
	ListEquip *m_storageItem;

	quint32 CurrentPage;
};

#endif // ITEM_EQUIPSTORAGE_H
