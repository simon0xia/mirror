#include "item_equipstorage.h"

extern QVector<Info_equip> g_EquipList;

Item_equipStorage::Item_equipStorage(RoleInfo *info, ListEquip *item, ListEquip *storageItem)
	: myRole(info), m_item(item), m_storageItem(storageItem)
{
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(ui.tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(ShowItemInfo(int, int)));
	connect(ui.tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowContextMenu(QPoint)));
}

Item_equipStorage::~Item_equipStorage()
{

}


void Item_equipStorage::updateInfo()
{
	quint32 row_Count = ui.tableWidget->rowCount();
	quint32 Col_Count = ui.tableWidget->columnCount();
	quint32 row_cur = 0;
	quint32 col_cur = 0;

	QString strTmp = "";
	quint32 ID, nCount;
	QString Name;

	//必须先清除背包显示，否则当前装备数量小于之前数量时会在最尾显示原装备的假像。
	ui.tableWidget->clear();
	for (ListEquip::const_iterator iter = m_storageItem->begin(); iter != m_storageItem->end(); iter++)
	{
		const Info_equip *itemItem = FindItem_Equip(*iter);

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

void Item_equipStorage::ShowItemInfo(int row, int column)
{
	ShowItemInfo_equip(row, column, m_storageItem, myRole);
}

void Item_equipStorage::ShowContextMenu(QPoint pos)
{
	int row = ui.tableWidget->currentRow();
	int col = ui.tableWidget->currentColumn();
	quint32 ID = GetItemID(row, col, m_storageItem);
	quint32 index = row * ui.tableWidget->columnCount() + col;

	m_storageItem->removeAt(index);
	m_item->append(ID);

	emit UpdateEquipInfoSignals();
}