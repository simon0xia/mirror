#include "Item_Base.h"
#include "mirrorlog.h"
#include "dlg_detail.h"

extern QMap<itemID, Info_Item> g_ItemList;
extern QMap<itemID, Info_basic_equip> g_EquipList;

extern Dlg_Detail *g_dlg_detail;
extern QWidget *g_widget;


const Info_Item* FindItem_Item(itemID ID)
{
	if (g_ItemList.contains(ID))
	{
		return &g_ItemList[ID];
	}
	else
		return &g_ItemList[0];
}

const Info_basic_equip * GetEquipBasicInfo(itemID id)
{
	if (g_EquipList.contains(id)) {
		return &g_EquipList[id];
	} else{
		return &g_EquipList[0];
	}
}

Item_Base::Item_Base(QWidget *parent)
	: QWidget(parent), row_Count(6), Col_Count(10)
{
	ui.setupUi(this);

	grid_w = 50;
	grid_h = 48;

	//ui.bagView->setShowGrid(true);
	model = new MiTableModel(row_Count, Col_Count);
	ui.bagView->setModel(model);
	ui.bagView->setItemDelegate(&delegate);
	ui.bagView->horizontalHeader()->setDefaultSectionSize(grid_w);
	ui.bagView->verticalHeader()->setDefaultSectionSize(grid_h);
	ui.bagView->setContextMenuPolicy(Qt::CustomContextMenu);
}

Item_Base::~Item_Base()
{
	delete model;

}

QPoint Item_Base::CalcDlgPos(int row, int column)
{
	//计算当前鼠标所在单元格，然后向右下偏移一个单元格
	QPoint point;
	if (column < Col_Count - 3)
	{
		point = QPoint(grid_w * column + grid_w, grid_h * row + grid_h);
	}
	else
	{
		point = QPoint(grid_w * 7, grid_h * row + grid_h);
	}

	QPoint pos = ui.bagView->mapTo(g_widget, point);
	return pos;
}
qint32 Item_Base::GetCurrentCellIndex(qint32 curPage)
{
	QModelIndex mIndex = ui.bagView->currentIndex();
	qint32 col = mIndex.column();
	qint32 row = mIndex.row();
	qint32 index = row * Col_Count + col + curPage * Col_Count * row_Count;
	return index;
}

qint32 Item_Base::GetActiveCellIndex(qint32 curPage, qint32 row, qint32 col)
{
/*	qint32 row_Count = ui.listView->rowCount();
	qint32 Col_Count = ui.listView->columnCount();
	qint32 index = row * Col_Count + col;
	index += (curPage - 1) * (row_Count * Col_Count);

	return index;*/
	return 0;
}
itemID Item_Base::GetItemID(qint32 index, const MapItem *items)
{
	MapItem::const_iterator iter = items->constBegin();
	for (qint32 i = 0; i < index && iter != items->end(); iter++, i++)	{ ; }
	return iter.key();
}
const Info_Equip *Item_Base::GetEquip(int row, int column, int curPage, const ListEquip *items)
{
/*	qint32 row_Count = ui.listView->rowCount();
	qint32 Col_Count = ui.listView->columnCount();
	qint32 index = Col_Count * row + column;
	index += (curPage - 1) * (row_Count * Col_Count);

	if (index >= 0 && index < items->size())
	{
		return &items->at(index);
	}
	else
	{
		return nullptr;
	}*/
	return nullptr;
}

void Item_Base::ShowItemInfo_item(int row, int column, int curPage, const MapItem *items)
{
	qint32 index = row * Col_Count + column + curPage * row_Count * Col_Count;
	if (index >= items->count())
	{
		return;
	}

	QPoint pos = CalcDlgPos(row,column);
	itemID ID = GetItemID(index, items);
	qint32 Number = items->value(ID);

	//根据道具ID查询道具列表，并返回道具的详细信息
	const Info_Item *item = FindItem_Item(ID);
	if (item != nullptr)
	{
		g_dlg_detail->DisplayItemInfo(pos, item, Number);
		g_dlg_detail->show();
	}
	else
	{
		LogIns.append(LEVEL_ERROR, __FUNCTION__, mirErr_UnknowItemID);
	}
}

void Item_Base::ShowItemInfo_equip(int row, int column, int curPage, const ListEquip *items)
{
	qint32 index = row * Col_Count + column + curPage * row_Count * Col_Count;
	if (index >= items->count())
	{
		return;
	}

	QPoint pos = CalcDlgPos(row, column);
	const Info_Equip equip = items->at(index);

	//根据道具ID查询道具列表，并返回道具的详细信息
	const Info_basic_equip *EquipBasicInfo = GetEquipBasicInfo(equip.ID);
	if (EquipBasicInfo != nullptr)
	{
		g_dlg_detail->DisplayEquipInfo(pos, EquipBasicInfo, &equip);
		g_dlg_detail->show();
	}
	else
	{
		LogIns.append(LEVEL_ERROR, __FUNCTION__, mirErr_UnknowEquipID);
	}
}

