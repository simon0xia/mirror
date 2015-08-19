#ifndef ITEM_EQUIPBAG_H
#define ITEM_EQUIPBAG_H

#include "Item_Base.h"

class item_equipBag : public Item_Base
{
	Q_OBJECT

public:
	item_equipBag(CPlayer *w_player);
	~item_equipBag();
	void updateInfo(void);
	
	
private slots :
	void ShowItemInfo(int row, int column);
	void ShowContextMenu(QPoint pos);
	void on_btn_sale_clicked();
	void on_btn_sort_clicked();
	void on_btn_pgUp_clicked();
	void on_btn_pgDn_clicked();

	void on_action_use(bool checked);
	void on_action_storage(bool checked);
	void on_action_sale(bool checked);

signals:
	void wearEquip(quint32 ID_for_new, quint32 index);
//	void UpdatePlayerInfoSignals(void);

private:
	CPlayer *player;
	ListEquip *m_item;
	ListEquip *m_storageItem;

	quint32 CurrentPage, pages;

	QMenu *popMenu;
	QAction *action_use, *action_storage, *action_sale;
};

#endif // ITEM_EQUIPBAG_H
