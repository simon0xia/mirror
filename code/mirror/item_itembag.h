#ifndef ITEM_ITEMBAG_H
#define ITEM_ITEMBAG_H

#include "Item_Base.h"

class item_itemBag : public Item_Base
{
	Q_OBJECT

public:
	item_itemBag(MapItem *item, RoleInfo *info);
	~item_itemBag();
	void updateInfo(void);

private slots:
	void ShowItemInfo(int row, int column);
	void ShowContextMenu(QPoint pos);

	void on_action_use(bool checked);
	void on_action_storage(bool checked);
	void on_action_sale(bool checked);

signals:
	void UsedItem(quint32 ID);

private:
	RoleInfo *myRole;
	MapItem *m_item;

	QMenu *popMenu;
	QAction *action_use, *action_storage, *action_sale;
};

#endif // ITEM_ITEMBAG_H
