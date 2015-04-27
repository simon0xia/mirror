#ifndef ITEM_EQUIPBAG_H
#define ITEM_EQUIPBAG_H

#include "Item_Base.h"

class item_equipBag : public Item_Base
{
	Q_OBJECT

public:
	item_equipBag(RoleInfo *info, ListEquip *item);
	~item_equipBag();
	void updateInfo(void);
	
	
private slots :
	void ShowItemInfo(int row, int column);
	void ShowContextMenu(QPoint pos);
	void on_btn_sale_clicked();
	void on_btn_sort_clicked();

signals:
	void wearEquip(quint32 ID_for_new, quint32 index);
	void UpdatePlayerInfoSignals(void);

private:
	ListEquip *m_item;
	RoleInfo *myRole;
};

#endif // ITEM_EQUIPBAG_H
