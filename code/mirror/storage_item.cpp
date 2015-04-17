#include "storage_item.h"

storage_item::storage_item(MapItem *item)
	: QWidget(NULL), m_item(item)
{
	ui.setupUi(this);

	ui.edit_page_cur->setText("1");
	ui.edit_page_all->setText("1");

	ui.tableWidget->setIconSize(QSize(32, 32));
	ui.tableWidget->horizontalHeader()->setDefaultSectionSize(80);
	ui.tableWidget->verticalHeader()->setDefaultSectionSize(35);

	ui.tableWidget->setRowCount(6);
	ui.tableWidget->setColumnCount(7);
}

storage_item::~storage_item()
{

}


ItemInfo* storage_item::getItem(QVector<ItemInfo> &ItemList, quint32 ID)
{
	for (QVector<ItemInfo>::iterator iter = ItemList.begin(); iter != ItemList.end(); iter++)
	{
		if (iter->ID == ID)
		{
			return &*iter;
		}
	}
	return NULL;
}

void storage_item::updateItemInfo(QVector<ItemInfo> &ItemList)
{
	quint32 row_Count = ui.tableWidget->rowCount();
	quint32 Col_Count = ui.tableWidget->columnCount();
	quint32 row_cur = 0;
	quint32 col_cur = 0;

	QString strTmp;
	quint32 ID, nCount;
	QString Name;
	ItemInfo *itemItem;
	for (MapItem::iterator iter = m_item->begin(); iter != m_item->end(); iter++)
	{
		ID = iter.key();
		itemItem = getItem(ItemList, ID);
		strTmp = QString::number((iter.value()));

		ui.tableWidget->setItem(row_cur, col_cur++, new QTableWidgetItem(QIcon(itemItem->icon), strTmp));
		if (col_cur > Col_Count)
		{
			++row_cur;
		}

		if (row_cur > row_Count)
		{
			//添加到第二页。
			break;	//暂不处理
		}
	}
}