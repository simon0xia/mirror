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
	void ShowItemInfo(int row, int column);
	void ShowContextMenu(QPoint pos);
	void on_btn_sale_clicked();
	void on_btn_sort_clicked();
	void on_btn_pgUp_clicked();
	void on_btn_pgDn_clicked();

	void on_action_use();
	void on_action_storage();
	void on_action_sale();

signals:
	void UpdateDisplayEquip(qint32 index);

private:
	quint32 CurrentPage, pages;

	ListEquip *bag_equip;

	QMenu *popMenu;
	QAction *action_use, *action_storage, *action_sale;
};

#endif // ITEM_EQUIPBAG_H
