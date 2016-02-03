#ifndef ITEM_EQUIPBAG_H
#define ITEM_EQUIPBAG_H

#include "Item_Base.h"

class item_equipBag : public Item_Base
{
	Q_OBJECT

public:
	item_equipBag(QWidget *parent = 0);
	~item_equipBag();
	void updateInfo(void);
	
	
private slots :
	void ShowItemInfo(const QModelIndex &index);
	void ShowContextMenu(QPoint pos);
	void on_btn_sale_clicked();
	void on_btn_clear_clicked();

	void on_btn_sort_clicked();
	void on_btn_storage_clicked();

	void on_btn_pgUp_clicked();
	void on_btn_pgDn_clicked();

private:
	void on_action_use(qint32 index);
signals:
	void UpdateDisplayEquip(qint32 index);

private:
	quint32 CurrentPage, pages;

	ListEquip *bag_equip;
};

#endif // ITEM_EQUIPBAG_H
