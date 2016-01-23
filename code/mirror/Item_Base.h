#ifndef ITEM_BASIC_H
#define ITEM_BASIC_H

#include <QWidget>
#include <QMenu>
#include "ui_item_base.h"
#include "ItemDefine.h"
#include "def_item_equip.h"
//#include "dlg_detail.h"

class Item_Base : public QWidget
{
	Q_OBJECT

public:
	Item_Base(QWidget *parent = 0);
	~Item_Base();

	static	const Info_Item* FindItem_Item(itemID ID);
	static	const Info_basic_equip* GetEquipBasicInfo(itemID id);

protected:
	void ShowItemInfo_item(int row, int column, int curPage, const MapItem *items);
	void ShowItemInfo_equip(int row, int column, int curPage, const ListEquip *items);

	QPoint CalcDlgPos(int row, int column);
	qint32 GetCurrentCellIndex(quint32 curPage);
	qint32 GetActiveCellIndex(quint32 curPage, quint32 row, quint32 col);
	itemID GetItemID(int row, int column, int curPage, const MapItem *items);
	const Info_Equip *GetEquip(int row, int column, int curPage, const ListEquip *items);

	Ui::item_base ui;

private:

signals :
	void si(int);
	void UpdateEquipInfoSignals(void);
	void UpdateCoin(void);
	void UpdateRep(void);
	void UpdateBag_BagEquip(void);
	void UpdateBag_StorageEquip(void);

private:
};

#endif // ITEM_BASIC_H
