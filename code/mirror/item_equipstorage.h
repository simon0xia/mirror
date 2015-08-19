#ifndef ITEM_EQUIPSTORAGE_H
#define ITEM_EQUIPSTORAGE_H

#include "Item_Base.h"

class Item_equipStorage : public Item_Base
{
	Q_OBJECT

public:
	Item_equipStorage(CPlayer *w_player);
	~Item_equipStorage();
	void updateInfo(void);

private slots:
	void ShowItemInfo(int row, int column);
	void ShowContextMenu(QPoint pos);

private:
	CPlayer *player;
	ListEquip *m_item;
	ListEquip *m_storageItem;

	quint32 CurrentPage;
};

#endif // ITEM_EQUIPSTORAGE_H
