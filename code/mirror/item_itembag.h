#ifndef ITEM_ITEMBAG_H
#define ITEM_ITEMBAG_H

#include "Item_Base.h"

class item_itemBag : public Item_Base
{
	Q_OBJECT

public:
	item_itemBag(QWidget *parent = 0);
	~item_itemBag();
	void DisplayItems(void);

private slots:
	void ShowItemInfo(int row, int column);
	void ShowContextMenu(QPoint pos);
	void on_btn_pgUp_clicked();
	void on_btn_pgDn_clicked();

	void on_action_use();
	void on_action_sale();

private:
	int32_t getUsedCount(itemID id);

	void UsedItem(const Info_Item* itemItem, int32_t usedCount);
	int32_t UsedItem_skill(itemID book, int32_t &usedCount);

	//更新指定id的道具数量，count为正，代表增加，否则为减少。
	void updateItem(itemID id, int32_t count);

private:
	quint32 CurrentPage, pages;

	MapItem *m_item;

	QMenu *popMenu;
	QAction *action_use, *action_storage, *action_sale;
};

#endif // ITEM_ITEMBAG_H
