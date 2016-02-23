#ifndef ITEM_ITEMBAG_H
#define ITEM_ITEMBAG_H

#include "Item_Base.h"

class item_itemBag : public Item_Base
{
	Q_OBJECT

public:
	item_itemBag(const LeftWindow& p_lw, QWidget *parent = 0);
	~item_itemBag();
	void DisplayItems(void);

private slots:
	void ShowItemInfo(const QModelIndex &index);
	void ShowContextMenu(QPoint pos);
	void on_btn_pgUp_clicked();
	void on_btn_pgDn_clicked();

	void on_btn_sale_clicked();

private:
	void on_action_use(qint32 index);
	void on_action_make(qint32 index);
	int32_t getUsedCount(itemID id);

	void UsedItem(const Info_Item* itemItem, int32_t usedCount);
	int32_t UsedItem_skill(itemID book, int32_t &usedCount);

	//更新指定id的道具数量，count为正，代表增加，否则为减少。
	void updateItem(itemID id, int32_t count);

private:
	const LeftWindow& lw;

	qint32 CurrentPage, pages;

	MapItem *m_item;
};

#endif // ITEM_ITEMBAG_H
