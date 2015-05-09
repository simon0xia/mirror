#include "item_itembag.h"
#include <QMessageBox>

item_itemBag::item_itemBag(MapItem *item, RoleInfo *info)
	: m_item(item), myRole(info)
{
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	CurrentPage = 1;

	popMenu = new QMenu();
	action_use = new QAction(QStringLiteral("使用"), this);
	action_storage = new QAction(QStringLiteral("入库"), this);
	action_sale = new QAction(QStringLiteral("销售"), this);
	popMenu->addAction(action_use);
//	popMenu->addAction(action_storage);
	popMenu->addAction(action_sale);

	connect(ui.tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(ShowItemInfo(int, int)));
	connect(ui.tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowContextMenu(QPoint)));

	connect(action_use, SIGNAL(triggered(bool)), this, SLOT(on_action_use(bool)));
//	connect(action_storage, SIGNAL(triggered(bool)), this, SLOT(on_action_storage(bool)));
	connect(action_sale, SIGNAL(triggered(bool)), this, SLOT(on_action_sale(bool)));
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

	QString strTmp = "";
	quint32 ID;

	pages = (m_item->size() + row_Count * Col_Count - 1) / (row_Count * Col_Count);
	ui.edit_page_cur->setText(QString::number(CurrentPage));
	ui.edit_page_all->setText(QString::number(pages));

	//必须先清除背包显示，否则当前道具种类小于之前道具种类时会在最尾显示原道具的假像。
	ui.tableWidget->clear();
	MapItem::const_iterator iter = m_item->constBegin();
	for (quint32 i = 0; i < (CurrentPage - 1) * (row_Count * Col_Count); i++, iter++) { ; }

	for (; iter != m_item->constEnd(); iter++)
	{
		ID = iter.key();
		const Info_Item *itemItem = FindItem_Item(ID);
		if (itemItem == nullptr)
		{
			continue;
		}

		ui.tableWidget->setItem(row_cur, col_cur++, new QTableWidgetItem(QIcon(itemItem->icon), strTmp));
		if (col_cur >= Col_Count)
		{
			++row_cur;
			col_cur = 0;
		}
	}
}

void item_itemBag::on_btn_pgUp_clicked()
{
	if (CurrentPage > 1)
	{
		--CurrentPage;
		updateInfo();
	}
}
void item_itemBag::on_btn_pgDn_clicked()
{
	if (CurrentPage < pages)
	{
		++CurrentPage;
		updateInfo();
	}
}

void item_itemBag::ShowItemInfo(int row, int column)
{
	ShowItemInfo_item(row, column, CurrentPage, m_item, myRole->vocation, myRole->level);
}

void item_itemBag::ShowContextMenu(QPoint pos)
{
	popMenu->exec(ui.tableWidget->mapToGlobal(pos));
}

void item_itemBag::on_action_use(bool checked)
{
	int row = ui.tableWidget->currentRow();
	int col = ui.tableWidget->currentColumn();
	quint32 ID = GetItemID(row, col, CurrentPage, m_item);

	const Info_Item* item = FindItem_Item(ID);
	if (myRole->level < item->level)
	{
		QString message = QStringLiteral("等级不足！");
		QMessageBox::critical(this, QStringLiteral("提示"), message);
		return;
	}
	if (item->vocation != 0 && item->vocation != myRole->vocation)
	{
		QString message = QStringLiteral("职业不符合！");
		QMessageBox::critical(this, QStringLiteral("提示"), message);
		return;
	}
	emit UsedItem(ID);
}

void item_itemBag::on_action_sale(bool checked)
{
	int row = ui.tableWidget->currentRow();
	int col = ui.tableWidget->currentColumn();
	quint32 ID = GetItemID(row, col, CurrentPage, m_item);
	quint32 Number = m_item->value(ID);
	quint32 index = row * ui.tableWidget->columnCount() + col;

	const Info_Item *itemitem = FindItem_Item(ID);
	if (itemitem != NULL)
	{
		myRole->coin += Number * (itemitem->coin >> 1);		//半价出售
		emit UpdatePlayerInfoSignals();

		m_item->remove(ID);
		updateInfo();
	}
}

void item_itemBag::on_action_storage(bool checked)
{
}