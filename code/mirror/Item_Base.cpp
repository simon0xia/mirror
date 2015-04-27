#include "Item_Base.h"

extern QVector<Info_Item> g_ItemList;
extern QVector<Info_equip> g_EquipList;

Item_Base::Item_Base()
	: QWidget(NULL)
{
	ui.setupUi(this);
	m_dlg_detail.setWindowFlags(Qt::WindowStaysOnTopHint);

	ui.btn_unname->setVisible(false);
	ui.btn_sort->setVisible(false);
	ui.btn_storage->setVisible(false);
	ui.btn_sale->setVisible(false);

	ui.edit_page_cur->setText("1");
	ui.edit_page_all->setText("1");

	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.tableWidget->setIconSize(QSize(32, 32));
	ui.tableWidget->horizontalHeader()->setDefaultSectionSize(80);
	ui.tableWidget->verticalHeader()->setDefaultSectionSize(35);

	ui.tableWidget->setRowCount(6);
	ui.tableWidget->setColumnCount(7);
}

Item_Base::~Item_Base()
{

}
const Info_Item* Item_Base::FindItem_Item(quint32 ID)
{
	if (ID <= 200000 || ID >= 300000)
		return NULL;

	foreach(const Info_Item &item, g_ItemList)
	{
		if (item.ID == ID)
		{
			return &item;
		}
	}
	return NULL;
}

const Info_equip * Item_Base::FindItem_Equip(quint32 id)
{
	if (id <= 300000 || id >= 400000)
		return NULL;

	foreach(const Info_equip &equip, g_EquipList)
	{
		if (equip.ID == id)
		{
			return &equip;
		}
	}
	return NULL;
}

QPoint Item_Base::CalcDlgPos(int row, int column)
{
	//计算当前鼠标位置，然后向右下偏移一个单元格
	int Height = ui.tableWidget->rowHeight(0);
	int Width = ui.tableWidget->columnWidth(0);
	QPoint pos = ui.tableWidget->mapToGlobal(QPoint(Width * (column + 1), Height * (row + 1)));
	return pos;
}
quint32 Item_Base::GetItemID(int row, int column, const MapItem *items)
{
	//计算单元格物品在背包列表中的索引，并根据索引得到道具ID
	quint32 Col_Count = ui.tableWidget->columnCount();
	quint32 index = Col_Count * row + column;
	MapItem::const_iterator iter = items->constBegin();
	for (quint32 i = 0; i < index && iter != items->end(); iter++, i++)	{ ; }
	return iter.key();
}
quint32 Item_Base::GetItemID(int row, int column, const ListEquip *items)
{
	//计算单元格物品在背包列表中的索引，并根据索引得到道具ID
	quint32 Col_Count = ui.tableWidget->columnCount();
	quint32 index = Col_Count * row + column;
	if (index > items->size())
	{
		return 0;
	}
	ListEquip::const_iterator iter = items->begin();
	for (quint32 i = 0; i < index && iter != items->end(); iter++, i++)	{ ; }
	if (iter == items->end())
		return 0;
	else
		return *iter;
}

void Item_Base::ShowItemInfo_item(int row, int column, const MapItem *items, quint32 role_lvl)
{
	QPoint pos = CalcDlgPos(row, column);
	quint32 ID = GetItemID(row, column, items);

	//根据道具ID查询道具列表，并返回道具的详细信息
	const Info_Item *item = FindItem_Item(ID);
	if (item != NULL)
	{
		m_dlg_detail.DisplayItemInfo(pos, item, role_lvl);
		m_dlg_detail.show();
	}
}

void Item_Base::ShowItemInfo_equip(int row, int column, const ListEquip *items, const RoleInfo *roleInfo)
{
	QPoint pos = CalcDlgPos(row, column);
	quint32 ID = GetItemID(row,column,items);

	//根据道具ID查询道具列表，并返回道具的详细信息
	const Info_equip *item = FindItem_Equip(ID);
	if (item != NULL)
	{
		m_dlg_detail.DisplayEquipInfo(pos, item, roleInfo);
		m_dlg_detail.show();
	}
}

