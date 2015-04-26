#ifndef ITEM_BASIC_H
#define ITEM_BASIC_H

#include <QWidget>
#include "ui_item_base.h"
#include "ItemDefine.h"
#include "dlg_detail.h"

class Item_Base : public QWidget
{
	Q_OBJECT

public:
	Item_Base();
	~Item_Base();

	static	const Info_Item* FindItem_Item(quint32 ID);
	static	const Info_equip* FindItem_Equip(quint32 id);

protected:
	void ShowItemInfo_item(int row, int column, const MapItem *items, quint32 role_lvl);
	void ShowItemInfo_equip(int row, int column, const ListEquip *items, const RoleInfo *roleInfo);

	QPoint CalcDlgPos(int row, int column);
	quint32 GetItemID(int row, int column, const MapItem *items);
	quint32 GetItemID(int row, int column, const ListEquip *items);

	Ui::item_base ui;

private:
	Dlg_Detail m_dlg_detail;
};

#endif // ITEM_BASIC_H
