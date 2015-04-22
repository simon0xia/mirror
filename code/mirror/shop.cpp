#include "shop.h"
#include "dlg_count.h"
#include <QMessageBox>

extern QVector<Info_Item> g_ItemList;

shop::shop(qint32 type, RoleInfo *roleInfo, MapItem *bag_item)
	: QWidget(NULL), m_ShopType(type), myRole(roleInfo), m_bag_item(bag_item)
{
	ui.setupUi(this);
		
	AdjustTableWidget();
	DisplayItemList();

	connect(ui.tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(cellClicked(int, int)));
}

shop::~shop()
{

}
void shop::AdjustTableWidget(void)
{
	ui.tableWidget->setColumnHidden(0, true);
	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

	ui.tableWidget->setColumnWidth(2, 50);
	ui.tableWidget->setColumnWidth(3, 100);
	ui.tableWidget->setColumnWidth(5, 50);

	ui.tableWidget->setColumnWidth(4, 280);
}

bool shop::DisplayItemList(void)
{
	quint32 i = 0;
	quint32 n = g_ItemList.size();
	qint32 ID_start = 200000 + m_ShopType * 1000;
	qint32 ID_stop = 200000 + (m_ShopType + 1) * 1000;
	foreach(const Info_Item &item, g_ItemList)
	{
		//不显示元宝购买的物品。
		if (item.sale != 1 || item.ID < ID_start || item.ID >= ID_stop)
		{
			continue;
		}
		ui.tableWidget->setRowCount(i + 1);
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(item.ID)));
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(item.name));
		ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(item.level)));
		ui.tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(item.coin)));
		ui.tableWidget->setItem(i, 4, new QTableWidgetItem(item.descr));
		ui.tableWidget->setItem(i, 5, new QTableWidgetItem(QStringLiteral("购买")));

		++i;
	}

	return true;
}

void shop::cellClicked(int row, int column)
{
	//第五列为购买，若此列上单击了，则认为玩家想购买道具
	if (column == 5)
	{
		quint32 nCount, nCost;
		quint32 ID = ui.tableWidget->item(row, 0)->text().toUInt();		//待购买道具的ID
		quint32 price = ui.tableWidget->item(row, 3)->text().toUInt();  //待购买道具的单价
		quint32 nMaxCount = myRole->coin / price;						//玩家当前资金可购买的最大数量。
		if (nMaxCount > 9999)
		{
			nMaxCount = 9999;											//单次最多允许购买9999
		}

		//弹出购买对话框
		dlg_count *dlg = new dlg_count(this, QStringLiteral("购买量"), nMaxCount);
		if (QDialog::Accepted == dlg->exec())
		{
			nCount = dlg->getCount();
			nCost = price * nCount;
			if (nCost > myRole->coin)
			{
				QString message = QStringLiteral("做人不要太贪心，您现有的资金最多只能购买：") + QString::number(nMaxCount);
				QMessageBox::critical(this, QStringLiteral("余额不足"), message);
			}
			else
			{
				if (nCount > 0)
				{
					//防止买'0'个物品。
					myRole->coin -= nCost;
					m_bag_item->insert(ID, m_bag_item->value(ID) + nCount);
				}
			}
		}
		delete dlg;
	}
}