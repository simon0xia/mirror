#include "item_itembag.h"

item_itemBag::item_itemBag(MapItem *item, RoleInfo *info)
	: m_item(item), myRole(info)
{
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

	

	connect(ui.tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(ShowItemInfo(int, int)));
	connect(ui.tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowContextMenu(QPoint)));
}

item_itemBag::~item_itemBag()
{

}

void item_itemBag::updateInfo()
{
	quint32 row_Count = ui.tableWidget->rowCount();
	quint32 Col_Count = ui.tableWidget->columnCount();
	quint32 row_cur = 0;
	quint32 col_cur = 0;

	QString strTmp;
	quint32 ID, nCount;
	QString Name;

	//必须先清除背包显示，否则当前道具种类小于之前道具种类时会在最尾显示原道具的假像。
	ui.tableWidget->clear();
	for (MapItem::const_iterator iter = m_item->constBegin(); iter != m_item->constEnd(); iter++)
	{
		ID = iter.key();
		const Info_Item *itemItem = FindItem_Item(ID);
		strTmp = QString::number((iter.value()));

		ui.tableWidget->setItem(row_cur, col_cur++, new QTableWidgetItem(QIcon(itemItem->icon), strTmp));
		if (col_cur >= Col_Count)
		{
			++row_cur;
			col_cur = 0;
		}

		if (row_cur >= row_Count)
		{
			//添加到第二页。
			break;	//暂不处理
		}
	}
}

void item_itemBag::ShowItemInfo(int row, int column)
{
	ShowItemInfo_item(row, column, m_item, myRole->level);
}

void item_itemBag::ShowContextMenu(QPoint pos)
{
	int row = ui.tableWidget->currentRow();
	int col = ui.tableWidget->currentColumn();
	quint32 ID = GetItemID(row, col, m_item);
	emit UsedItem(ID);
}