#include "city_auction.h"
#include <QMouseEvent>
#include <QListWidgetItem>
#include <QMessageBox>

extern QMap<itemID, Info_Item> g_ItemList;

city_auction::city_auction(QWidget *w_parent, CPlayer *const w_player)
	: QWidget(w_parent), parrent(w_parent), player(w_player)
{
	ui.setupUi(this);
	ui.listWidget->setMovement(QListView::Static);

	m_bag_item = player->get_bag_item();

	if (InitSales())
	{
		DisplayItemList();
	}

	connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(itemClicked(QListWidgetItem *)));
	connect(ui.listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(itemDoubleClicked(QListWidgetItem *)));
}

city_auction::~city_auction()
{

}
bool city_auction::InitSales(void)
{
	const int count = 7;
	itemID item[count] = { 203008, 203012, 203014, 220042, 220043, 220044 };
	uint32_t price[count] = { 100, 10000, 100000, 50000, 50000, 50000 };

	for (auto i = 0; i < count; i++)
	{
		sales.insert(item[i], price[i]);
	}

	return true;
}

bool city_auction::DisplayItemList(void)
{
	QListWidgetItem *ListItem;

	for (auto iter = sales.constBegin(); iter != sales.constEnd(); iter++)
	{
		const Info_Item *item = Item_Base::FindItem_Item(iter.key());
		if (item != nullptr)
		{
			ListItem = new QListWidgetItem(item->icon, item->name);
			ListItem->setWhatsThis(QString::number(item->ID));
			ui.listWidget->addItem(ListItem);
		}
	}
	return true;
}

void city_auction::mousePressEvent(QMouseEvent *ev)
{
	mouseMovePos = ev->globalPos();
}
void city_auction::mouseMoveEvent(QMouseEvent * ev)
{
	if (mouseMovePos != QPoint(0, 0))
	{
		move(this->pos() + ev->globalPos() - mouseMovePos);
		mouseMovePos = ev->globalPos();
	}
}
void city_auction::mouseReleaseEvent(QMouseEvent *ev)
{
	mouseMovePos = QPoint(0, 0);
}

void city_auction::on_btn_close_clicked(void)
{
	close();
}

void city_auction::itemClicked(QListWidgetItem * item)
{
	itemID id = item->whatsThis().toUInt();
	ui.lbl_soul_count->setText(QString::number(sales.value(id)));
}

void city_auction::itemDoubleClicked(QListWidgetItem * item)
{
	itemID ID = item->whatsThis().toUInt();
	quint32 price = sales.value(ID);

	if (player->get_soul() < price)
	{
		QString message = QStringLiteral("你当前仅拥有灵魂点:%1，不足以兑换此物品").arg(player->get_soul());
		QMessageBox::information(this, QStringLiteral("兑换"), message);
		return;
	}

	QMessageBox *msgBox = new QMessageBox;
	msgBox->setText(QStringLiteral("你正在兑换：%1，确认?").arg(item->text()));
	msgBox->setWindowTitle(QStringLiteral("兑换"));
	msgBox->addButton(QStringLiteral(" 确认 "), QMessageBox::AcceptRole);
	msgBox->addButton(QStringLiteral(" 取消 "), QMessageBox::RejectRole);

	if (QMessageBox::AcceptRole == msgBox->exec())
	{
		player->sub_soul(price);
		m_bag_item->insert(ID, m_bag_item->value(ID) + 1);
	}
	delete msgBox;
}
