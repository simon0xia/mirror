#include "city_shop.h"
#include "dlg_count.h"
#include <QMessageBox>
#include <QMouseEvent>
#include "Item_Base.h"

extern QMap<itemID, Info_Item> g_ItemList;
extern RoleInfo_False g_falseRole;

city_shop::city_shop(QWidget *parent, qint32 type, RoleInfo *roleInfo, MapItem *bag_item)
	: QWidget(parent), m_ShopType(type), myRole(roleInfo), m_bag_item(bag_item)
{
	ui.setupUi(this);
	ui.listWidget->setMovement(QListView::Static);
	m_parrent = parent;

	DisplayItemList();
	ui.listWidget->setStyleSheet("QListWidget{ background:transparent} ");

	connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(itemClicked(QListWidgetItem *)));
	connect(ui.listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(itemDoubleClicked(QListWidgetItem *)));
}

city_shop::~city_shop()
{

}

bool city_shop::DisplayItemList(void)
{
	qint32 ID_start = g_itemID_start_item + m_ShopType * 1000;
	qint32 ID_stop = g_itemID_start_item + (m_ShopType + 1) * 1000;

	QListWidgetItem *ListItem;

	foreach(const Info_Item &item, g_ItemList)
	{
		if (item.sale == 1 || item.ID >= ID_start || item.ID < ID_stop)
		{
			ListItem = new QListWidgetItem(item.icon, "");
			ListItem->setWhatsThis(QString::number(item.ID));
			ui.listWidget->addItem(ListItem);
		}	
	}
	return true;
}

void city_shop::itemClicked(QListWidgetItem * item)
{
	itemID id = item->whatsThis().toUInt();
	const Info_Item *itemitem = Item_Base::FindItem_Item(id);
	if (itemitem != nullptr)
	{
		QString strTmp = itemitem->name + QStringLiteral("  等级:") + QString::number(itemitem->level);
		strTmp += QStringLiteral("  单价:") + QString::number(itemitem->coin);
		ui.lbl_ItemName->setText(strTmp);
		ui.lbl_msg->setText(itemitem->descr);
	}
}

void city_shop::itemDoubleClicked(QListWidgetItem * item)
{
	itemID ID = item->whatsThis().toUInt();
	const Info_Item *itemitem = Item_Base::FindItem_Item(ID);
	if (itemitem == nullptr)
	{
		return;
	}

	quint64 role_coin = (myRole->coin >> 1) - 1;
	quint32 nCount, nCost;
	quint32 price = itemitem->coin;							//待购买道具的单价
	quint32 nMaxCount = role_coin / price;					//玩家当前资金可购买的最大数量。
	if (nMaxCount > 9999)
	{
		nMaxCount = 9999;									//单次最多允许购买9999
	}

	//弹出购买对话框
	dlg_count *dlg = new dlg_count(this, QStringLiteral("购买量"), nMaxCount);
	if (QDialog::Accepted == dlg->exec())
	{
		nCount = dlg->getCount();
		nCost = price * nCount;
		if (nCost > role_coin)
		{
			QString message = QStringLiteral("做人不要太贪心，您现有的资金最多只能购买：") + QString::number(nMaxCount);
			QMessageBox::critical(this, QStringLiteral("余额不足"), message);
		}
		else
		{
			if (nCount > 0)
			{
				//防止买'0'个物品。
				myRole->coin -= nCost << 1;
				m_bag_item->insert(ID, m_bag_item->value(ID) + nCount);

				g_falseRole.coin -= nCost;
			}
		}
	}
	delete dlg;
}

void city_shop::on_btn_close_clicked(void)
{
	close();
}

void city_shop::mouseMoveEvent(QMouseEvent * ev)
{
	QPoint point = m_parrent->mapFromGlobal(ev->globalPos()) - QPoint(30,30);
	move(point);
}