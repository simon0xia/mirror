#ifndef ITEM_BASIC_H
#define ITEM_BASIC_H

#include <QWidget>
#include <QMenu>
#include "ui_item_base.h"
#include "ItemDefine.h"
#include "def_item_equip.h"

#include "./define/MiTableModel.h"
#include "./define/MiItemDelegate.h"

const Info_Item* FindItem_Item(itemID ID);
const Info_basic_equip* GetEquipBasicInfo(itemID id);

class Item_Base : public QWidget
{
	Q_OBJECT

public:
	Item_Base(QWidget *parent = 0);
	~Item_Base();

protected:
	void ShowItemInfo_item(int row, int column, int curPage, const MapItem *items);
	void ShowItemInfo_equip(int row, int column, int curPage, const ListEquip *items);

	QPoint CalcDlgPos(int row, int column);
	qint32 GetCurrentCellIndex(qint32 curPage);
	qint32 GetActiveCellIndex(qint32 curPage, qint32 row, qint32 col);
	itemID GetItemID(qint32 index, const MapItem *items);
	const Info_Equip *GetEquip(int row, int column, int curPage, const ListEquip *items);

	Ui::item_base ui;
	const int row_Count, Col_Count;
	MiTableModel *model;
	MiItemDelegate delegate;

private:
	qint32 grid_w, grid_h;

signals :
	void SmithyEquip(void);
	void UpdateEquipInfoSignals(void);
	void UpdateCoin(void);
	void UpdateRep(void);
	void UpdateBag_BagEquip(void);
	void UpdateBag_StorageEquip(void);

private:
};

#endif // ITEM_BASIC_H
