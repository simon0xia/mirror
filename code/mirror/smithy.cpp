#include "smithy.h"
#include "Player.h"
#include "Item_Base.h"

#include "dlg_detail.h"

extern QMap<itemID, Info_basic_equip> g_EquipList;
extern QVector<QImage> g_dat_item;

extern Dlg_Detail *g_dlg_detail;

smithy::smithy(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	PlayerIns.get_onNpcEquip(0);
	price = INT32_MAX;

	//为装备栏控件安装事件过滤机制，使得QLabel控件可响应clicked()之类的事件。
	ui.lbl_equip->installEventFilter(this);
}

smithy::~smithy()
{

}

void smithy::updateInfo()
{
	ui.listWidget->clear();
	onMakeEquip = PlayerIns.get_onNpcEquip(0);

	bool bSatisfy;
	qint32 equipLv, equipLvUp;
	bool bHasEquip = CheckAndDisplayEquip();

	bSatisfy = bHasEquip;
	if (bHasEquip)
	{
		const Info_basic_equip *eqInfo = GetEquipBasicInfo(onMakeEquip.ID);
		equipLv = eqInfo->lv;
		equipLvUp = onMakeEquip.lvUp;
	}
	else
	{
		equipLv = 0;
		equipLvUp = 0;
	}

	bSatisfy &= CheckAndDisplayPrice(equipLv, equipLvUp);
	bSatisfy &= CheckAndDisplayM1(equipLvUp);

	ui.btn_make->setEnabled(bSatisfy);
}

bool smithy::CheckAndDisplayPrice(qint32 equipLv, qint32 equipLvUp)
{
	bool bSatisfy = false;

	price = qint32(pow(equipLv, 2) * pow((equipLvUp + 2), 2.4)) * 200;

	QString strTmp;
	if (PlayerIns.get_coin() < price) {
		strTmp = QStringLiteral("<font color = red>%1 万金币</font>").arg(price / 10000);
	} else {
		strTmp = QStringLiteral("<font color = white>%1 万金币</font>").arg(price / 10000);
		bSatisfy = true;
	}
	ui.lbl_coin->setText(strTmp);
	return bSatisfy;
}

bool smithy::CheckAndDisplayEquip()
{
	bool hasEquip = false;
	if (onMakeEquip.ID < g_itemID_start_equip || onMakeEquip.ID > g_itemID_stop_equip)
	{
		ui.lbl_equip->setPixmap(QPixmap(""));
		ui.lbl_nameForMake->setText(QStringLiteral("待强化武器"));
	}
	else
	{
		const Info_basic_equip *eqInfo = GetEquipBasicInfo(onMakeEquip.ID);
		if (eqInfo->icon >= g_dat_item.size() || eqInfo->icon < 0) {
			ui.lbl_equip->setPixmap(QPixmap::fromImage(g_dat_item.first()));
		}
		else {
			ui.lbl_equip->setPixmap(QPixmap::fromImage(g_dat_item.at(eqInfo->icon)));
		}
		ui.lbl_nameForMake->setText(eqInfo->name);

		for (int i = 0; i < onMakeEquip.lvUp; i++)
		{
			QIcon pix(":/mirror/Resources/1005.png");
			ui.listWidget->addItem(new QListWidgetItem(pix, ""));
		}
		for (int i = onMakeEquip.lvUp; i < g_equip_MaxLvUp; i++)
		{
			QIcon pix(":/mirror/Resources/1006.png");
			ui.listWidget->addItem(new QListWidgetItem(pix, ""));
		}
		hasEquip = true;
	}
	return hasEquip;
}

bool smithy::CheckAndDisplayM1(qint32 equipLvUp)
{
	bool bSatisfy = false;
	itemID itemitem = PlayerIns.get_onNpcItem(0);
	if (itemitem > g_itemID_stop_item || itemitem <= g_itemID_start_item)
	{
		ui.lbl_icon_m1->setPixmap(QPixmap(""));
		ui.lbl_name_m1->setText(QStringLiteral("<font color = red>矿石</font>"));
	}
	else
	{
		const Info_Item *itemInfo = FindItem_Item(itemitem);
		if (itemInfo->icon >= g_dat_item.size() || itemInfo->icon < 0) {
			ui.lbl_icon_m1->setPixmap(QPixmap::fromImage(g_dat_item.first()).scaled(QSize(32,32)));
		} else {
			ui.lbl_icon_m1->setPixmap(QPixmap::fromImage(g_dat_item.at(itemInfo->icon)).scaled(QSize(32, 32)));
		}

		if (itemInfo->ID > 202100 || itemInfo->ID < 202081 + equipLvUp) {
			ui.lbl_name_m1->setText(QStringLiteral("<font color = red>%1</font>").arg(itemInfo->descr));
		} else{
			ui.lbl_name_m1->setText(QStringLiteral("<font color = green>%1</font>").arg(itemInfo->descr));
			bSatisfy = true;
		}
	}	
	return bSatisfy;
}

void smithy::RemoveEquip()
{
	if (onMakeEquip.ID < g_itemID_start_equip || onMakeEquip.ID > g_itemID_stop_equip)
	{
		//no equip;
		return;
	}
	PlayerIns.Remove_onNpcEquip(0);
	PlayerIns.get_bag_equip().append(onMakeEquip);

	onMakeEquip = { 0 };
	updateInfo();
	
	emit UpdateBag_BagEquip();
}

bool smithy::eventFilter(QObject *obj, QEvent *ev)
{
	if (obj == ui.lbl_equip)
	{
		if (ev->type() == QEvent::Enter)
		{
			const Info_basic_equip *EquipBasicInfo = GetEquipBasicInfo(onMakeEquip.ID);
			if (EquipBasicInfo->ID > g_itemID_start_equip)
			{
				g_dlg_detail->DisplayEquipInfo(this->mapFromGlobal(QCursor::pos()) + QPoint(20, 15), EquipBasicInfo, &onMakeEquip);
				g_dlg_detail->show();
			}
			return  true;
		}
		else if (ev->type() == QEvent::Leave)
		{
			g_dlg_detail->hide();
		}
		else if (ev->type() == QEvent::MouseButtonRelease)
		{
			QMouseEvent *mouseEvent = (QMouseEvent *)(ev);
			if (mouseEvent->button() == Qt::MouseButton::RightButton)
			{
				RemoveEquip();
				return  true;
			}
		}
	}

	// pass the event on to the parent class
	return QWidget::eventFilter(obj, ev);
}

void smithy::on_btn_make_clicked(void)
{
	if (onMakeEquip.lvUp < g_equip_MaxLvUp)
	{
		PlayerIns.sub_coin(price);
		onMakeEquip.lvUp++;

		PlayerIns.Set_onNpcEquip(0, onMakeEquip);
		PlayerIns.Set_onNpcItem(0, 0);
		updateInfo();

		emit UpdateCoin();
	}
}