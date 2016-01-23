#include "Item_Base.h"
#include "def_System_para.h"
#include "mirrorlog.h"
#include "dlg_detail.h"

extern QMap<itemID, Info_Item> g_ItemList;
extern QMap<itemID, Info_basic_equip> g_EquipList;

extern Dlg_Detail *g_dlg_detail;
extern QWidget *g_widget;

Item_Base::Item_Base(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.btn_sort->setVisible(false);
	ui.btn_sale->setVisible(false);

	ui.btn_pgUp->setEnabled(false);
	ui.btn_pgDn->setEnabled(false);
	ui.edit_page_all->setText(QString::number(1));
	ui.edit_page_cur->setText(QString::number(1));

	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidget->setIconSize(QSize(38, 38));
	ui.tableWidget->horizontalHeader()->setDefaultSectionSize(42);
	ui.tableWidget->verticalHeader()->setDefaultSectionSize(40);

	ui.tableWidget->setRowCount(6);
	ui.tableWidget->setColumnCount(10);
}

Item_Base::~Item_Base()
{

}
const Info_Item* Item_Base::FindItem_Item(itemID ID)
{
	if (g_ItemList.contains(ID))
	{
		return &g_ItemList[ID];
	}
	else
		return nullptr;
}

const Info_basic_equip * Item_Base::GetEquipBasicInfo(itemID id)
{
	if (id <= g_itemID_start_equip || id > g_itemID_stop_equip)
		return nullptr;

	if (g_EquipList.contains(id)) {
		return &g_EquipList[id]; 
	} else{
		return &g_EquipList.first();
	}
}

QPoint Item_Base::CalcDlgPos(int row, int column)
{
	//计算当前鼠标所在单元格，然后向右下偏移一个单元格
 	int Height = ui.tableWidget->rowHeight(0);
 	int Width = ui.tableWidget->columnWidth(0);
	QPoint point = QPoint(Width * column + Width, Height * row + Height);
	if (column >= ui.tableWidget->columnCount() - 5)
	{
		point -= QPoint(Width * (column - (ui.tableWidget->columnCount() - 5)), 0);
	}
	QPoint pos = ui.tableWidget->mapTo(g_widget, point);
	return pos;
}
qint32 Item_Base::GetCurrentCellIndex(quint32 curPage)
{
	int row = ui.tableWidget->currentRow();
	int col = ui.tableWidget->currentColumn();
	qint32 row_Count = ui.tableWidget->rowCount();
	qint32 Col_Count = ui.tableWidget->columnCount();
	qint32 index = row * Col_Count + col;
	index += (curPage - 1) * (row_Count * Col_Count);

	return index;
}
qint32 Item_Base::GetActiveCellIndex(quint32 curPage, quint32 row, quint32 col)
{
	qint32 row_Count = ui.tableWidget->rowCount();
	qint32 Col_Count = ui.tableWidget->columnCount();
	qint32 index = row * Col_Count + col;
	index += (curPage - 1) * (row_Count * Col_Count);

	return index;
}
itemID Item_Base::GetItemID(int row, int column, int curPage, const MapItem *items)
{
	//计算单元格物品在背包列表中的索引，并根据索引得到道具ID
	qint32 row_Count = ui.tableWidget->rowCount();
	qint32 Col_Count = ui.tableWidget->columnCount();
	qint32 index = Col_Count * row + column;
	index += (curPage - 1) * (row_Count * Col_Count);

	MapItem::const_iterator iter = items->constBegin();
	for (qint32 i = 0; i < index && iter != items->end(); iter++, i++)	{ ; }
	return iter.key();
}
const Info_Equip *Item_Base::GetEquip(int row, int column, int curPage, const ListEquip *items)
{
	qint32 row_Count = ui.tableWidget->rowCount();
	qint32 Col_Count = ui.tableWidget->columnCount();
	qint32 index = Col_Count * row + column;
	index += (curPage - 1) * (row_Count * Col_Count);

	if (index >= 0 && index < items->size())
	{
		return &items->at(index);
	}
	else
	{
		return nullptr;
	}
}

void Item_Base::ShowItemInfo_item(int row, int column, int curPage, const MapItem *items)
{
	qint32 index = GetActiveCellIndex(curPage, row, column);
	if (items->size() == 0 || index >= items->size())
	{
		//点击空白地方，返回
		g_dlg_detail->hide();
		return;
	}

	QPoint pos = CalcDlgPos(row, column);
	itemID ID = GetItemID(row, column, curPage, items);
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
	qint32 index = GetActiveCellIndex(curPage, row, column);
	//if (items->size() == 0 || (index + 1) > items->size())
	if (items->size() == 0 || index >= items->size())
	{
		//点击空白地方，返回
		g_dlg_detail->hide();
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

