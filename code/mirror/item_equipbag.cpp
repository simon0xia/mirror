#include <QMessageBox>

#include "item_equipbag.h"
#include "dlg_detail.h"
#include "player.h"

extern QWidget *g_widget;

extern Dlg_Detail *g_dlg_detail;

extern QVector<QImage> g_dat_item;
extern QVector<QImage> g_dat_ui;

item_equipBag::item_equipBag(const LeftWindow& p_lw, QWidget *parent)
	:Item_Base(parent), lw(p_lw)
{
	bag_equip = &PlayerIns.get_bag_equip();

	CurrentPage = 0;

	connect(ui.btn_sale, SIGNAL(clicked()), this, SLOT(on_btn_sale_clicked()));
	connect(ui.btn_clear, SIGNAL(clicked()), this, SLOT(on_btn_clear_clicked()));
	connect(ui.btn_storage, SIGNAL(clicked()), this, SLOT(on_btn_storage_clicked()));
	connect(ui.btn_sort, SIGNAL(clicked()), this, SLOT(on_btn_sort_clicked()));
	connect(ui.btn_pgUp, SIGNAL(clicked()), this, SLOT(on_btn_pgUp_clicked()));
	connect(ui.btn_pgDn, SIGNAL(clicked()), this, SLOT(on_btn_pgDn_clicked()));
	//connect(ui.listView, SIGNAL(cellEntered(int, int)), this, SLOT(ShowItemInfo(int, int)));
	connect(ui.bagView, SIGNAL(entered(QModelIndex)), this, SLOT(ShowItemInfo(QModelIndex)));
	connect(ui.bagView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowContextMenu(QPoint)));

	
}

item_equipBag::~item_equipBag()
{

}

void item_equipBag::updateInfo()
{
	pages = bag_equip->size() / (row_Count * Col_Count) + 1;
	if (CurrentPage >= pages)
	{
		CurrentPage = pages - 1;
	}

	ui.lbl_page->setText(QStringLiteral("%1/%2").arg(CurrentPage+1).arg(pages));
	if (pages == 1)
	{
		ui.btn_pgDn->setEnabled(false);
		ui.btn_pgUp->setEnabled(false);
	}
	else if (CurrentPage == 0)
	{
		ui.btn_pgDn->setEnabled(true);
		ui.btn_pgUp->setEnabled(false);
	}
	else if (CurrentPage+1 >= pages)
	{
		ui.btn_pgDn->setEnabled(false);
		ui.btn_pgUp->setEnabled(true);
	}
	else
	{
		ui.btn_pgDn->setEnabled(true);
		ui.btn_pgUp->setEnabled(true);
	}

	qint32 col_cur, row_cur;
	col_cur = row_cur = 0;
	//必须先清除背包显示，否则当前装备数量小于之前数量时会在最尾显示原装备的假像。
	model->clear();
	auto iter = bag_equip->constBegin();
	for (qint32 i = 0; i < CurrentPage * (row_Count * Col_Count); i++, iter++) { ; }

	for (; iter != bag_equip->constEnd(); iter++)
	{	
		const Info_basic_equip *EquipBasicInfo = GetEquipBasicInfo(iter->ID);
		if (EquipBasicInfo != nullptr)
		{
			MiItem item;
			item.id = EquipBasicInfo->ID;
			item.count = 1;
			item.intensify = iter->lvUp;
			item.image = g_dat_item.at(EquipBasicInfo->icon);
			item.quality = g_dat_ui.at(iter->extraAmount +1);
			
			model->setData(row_cur, col_cur, item);

			++col_cur;
			if (col_cur >= Col_Count)
			{
				++row_cur;
				col_cur = 0;
			}
		}
	}
}

void item_equipBag::on_btn_pgUp_clicked()
{
	if (CurrentPage > 0)
	{
		--CurrentPage;
		updateInfo();
	}
}
void item_equipBag::on_btn_pgDn_clicked()
{
	if (CurrentPage < pages-1)
	{
		++CurrentPage;
		updateInfo();
	}
}

void item_equipBag::ShowItemInfo(const QModelIndex &index)
{
	g_dlg_detail->hide();

	qint32 row = index.row();
	qint32 column = index.column();
	ShowItemInfo_equip(row, column, CurrentPage, bag_equip);
}

void item_equipBag::ShowContextMenu(QPoint pos)
{
	Q_UNUSED(pos);
	g_dlg_detail->hide();

	qint32 index = GetCurrentCellIndex(CurrentPage);
	if (index >= 0 && index < bag_equip->count())
	{
		if (lw == LW_role)
		{
			on_action_use(index);
		}
		else
		{
			on_action_make(index);
		}
	}
}

void item_equipBag::on_action_use(qint32 index)
{
	const Info_Equip &equip = bag_equip->at(index);
	const CHuman &CurEdt = PlayerIns.get_edt_current();
	qint32 nTmp;
	QString message = QStringLiteral("你未达到穿戴此装备的最低要求！");

	const Info_basic_equip *EquipBasicInfo = GetEquipBasicInfo(equip.ID);
	if (EquipBasicInfo->ID != equip.ID)
	{
		return;		//unknown equipment ID！
	}
	//获取待佩带装备的类别
	int Type = (equip.ID - g_itemID_start_equip) / 1000;

	//查询角色当前属性是否符合佩带需要。	
	bool bSatisfy = false;
	switch (EquipBasicInfo->need)
	{
	case 0: 
		bSatisfy = (CurEdt.get_lv() >= EquipBasicInfo->needLvl);
		break;
	case 1: 
		bSatisfy = (CurEdt.get_dc2() >= EquipBasicInfo->needLvl);
		break;
	case 2: 
		bSatisfy = (CurEdt.get_mc2() >= EquipBasicInfo->needLvl);
		break;
	case 3: 
		bSatisfy = (CurEdt.get_sc2() >= EquipBasicInfo->needLvl);
		break;
	default:
		break;
	}
	if (Type == g_equipType_clothes_m || Type == g_equipType_clothes_f)
	{
		//当前装备为衣服，需判断性别。
		bool bTmp = (CurEdt.get_gender() == (Type - 1));
		bSatisfy = bSatisfy && bTmp;
		if (!bTmp)
		{
			QString strTmp = (Type == g_equipType_clothes_m) ? QStringLiteral("男") : QStringLiteral("女");
			message = QStringLiteral("性别不符,此装备为%1性装备").arg(strTmp);
		}	
	}

	if (bSatisfy)	{
		nTmp = PlayerIns.get_edt_current().wearEquip(index);
		updateInfo();
		emit UpdateDisplayEquip(nTmp);
	}	else	{
		QMessageBox::critical(g_widget, QStringLiteral("提示"), message);
	}
}

void item_equipBag::on_action_make(qint32 index)
{
	Info_Equip npc_Eq = PlayerIns.get_onNpcEquip(0);
	
	const Info_Equip &equip = bag_equip->at(index);
	int Type = (equip.ID - g_itemID_start_equip) / 1000;
	if (Type != g_equipType_weapon ||
		GetEquipBasicInfo(equip.ID)->lv < 7)
	{
		return; //只有7阶(含)以上武器才可以强化
	}

	PlayerIns.Set_onNpcEquip(0, equip);
	bag_equip->removeAt(index);

	if (npc_Eq.ID > g_itemID_start_equip && npc_Eq.ID <= g_itemID_stop_equip)
	{
		bag_equip->append(npc_Eq);
	}
	updateInfo();
	emit SmithyEquip();
}

void item_equipBag::on_btn_storage_clicked()
{
	qint32 index = GetCurrentCellIndex(CurrentPage);
	if (index < 0 || index >= bag_equip->count())
	{
		return;
	}

	ListEquip &storage_equip = PlayerIns.get_storage_equip();

	const Info_Equip equip = bag_equip->at(index);

	if (storage_equip.size() >= g_storage_maxSize)
	{
		QString message = QStringLiteral("仓库已满！");
		QMessageBox::critical(g_widget, QStringLiteral("提示"), message);
	}
	else
	{
		storage_equip.append(equip);
		bag_equip->removeAt(index);

		updateInfo();
		emit UpdateBag_StorageEquip();
	}
}
void item_equipBag::on_btn_sale_clicked()
{
	qint32 index = GetCurrentCellIndex(CurrentPage);
	if (index < 0 || index >= bag_equip->count())
	{
		return;
	}

	const Info_Equip equip = bag_equip->at(index);

	const Info_basic_equip *EquipBasicInfo = GetEquipBasicInfo(equip.ID);
	if (EquipBasicInfo != NULL)
	{
		PlayerIns.add_coin(EquipBasicInfo->price >> 1);	//一半价格卖出
		bag_equip->removeAt(index);
		updateInfo();
		emit UpdateCoin();
	}
}

void item_equipBag::on_btn_clear_clicked()
{
	QString message = QStringLiteral("点击确认将售出背包内所有装备(不含神器)，是否确认？");
	QMessageBox msgBox(QMessageBox::Information, QStringLiteral("一键销售"), message);
	QPushButton *YsBtn = msgBox.addButton(QStringLiteral(" 确认 "), QMessageBox::AcceptRole);
	QPushButton *NoBtn = msgBox.addButton(QStringLiteral(" 取消 "), QMessageBox::RejectRole);
	msgBox.exec();
	if (msgBox.clickedButton() != YsBtn)
	{
		return;
	}

	ListEquip tmp;
	qint32 coin = 0;
	for (auto iter = bag_equip->constBegin(); iter != bag_equip->constEnd(); iter++)
	{
		const Info_basic_equip *EquipBasicInfo = GetEquipBasicInfo(iter->ID);
		if (EquipBasicInfo != nullptr)
		{
			if (EquipBasicInfo->lv == 9999)
			{
				tmp.append(*iter);
			}
			else
			{
				coin += EquipBasicInfo->price >> 2; //一键销售只有1/4价格	
			}
		}
	}
	bag_equip->clear();
	PlayerIns.add_coin(coin);	
	for (ListEquip::const_iterator iter = tmp.begin(); iter != tmp.end(); iter++)
	{
		bag_equip->append(*iter);
	}
	updateInfo();
	emit UpdateCoin();
	
}
void item_equipBag::on_btn_sort_clicked()
{
	ListEquip tmp;
	qint32 j, k;
	if (bag_equip->size() <= 0)
	{
		return;
	}
	
	//选择排序，按品质。
	//遍历一次背包，便将同一品质装备加入到临时列表中。故算法速度约为O(C * n) * 2,可直接简写为O(n).
	k = g_specialEquip_MaxExtra;
	j = 0;
	do
	{
		for (auto iter = bag_equip->constBegin(); iter != bag_equip->constEnd(); iter++)
		{
			if (iter->extraAmount == k)
			{
				tmp.append(*iter);
			}
		}
	} while (k--);

	bag_equip->clear();
	for (auto iter = tmp.constBegin(); iter != tmp.constEnd(); iter++)
	{
		bag_equip->append(*iter);
	}
	updateInfo();
}