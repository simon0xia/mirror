#include <QMessageBox>

#include "item_equipbag.h"

extern RoleInfo_False g_falseRole;
extern QWidget *g_widget;

extern Dlg_Detail *g_dlg_detail;

item_equipBag::item_equipBag(CPlayer *w_player)
	:player(w_player)
{
	ui.btn_sale->setVisible(true);
	ui.btn_sort->setVisible(true);
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

	m_item = player->get_bag_equip();
	m_storageItem = player->get_storage_equip();

	CurrentPage = 1;
	ui.edit_page_all->setText(QString::number(4));

	popMenu = new QMenu();
	action_use = new QAction(QStringLiteral("装备"), this);
	action_storage = new QAction(QStringLiteral("入库"), this);
	action_sale = new QAction(QStringLiteral("销售"), this);
	popMenu->addAction(action_use);
	popMenu->addAction(action_storage);
	popMenu->addAction(action_sale);

	connect(ui.btn_sale, SIGNAL(clicked()), this, SLOT(on_btn_sale_clicked()));
	connect(ui.btn_sort, SIGNAL(clicked()), this, SLOT(on_btn_sort_clicked()));
	connect(ui.btn_pgUp, SIGNAL(clicked()), this, SLOT(on_btn_pgUp_clicked()));
	connect(ui.btn_pgDn, SIGNAL(clicked()), this, SLOT(on_btn_pgDn_clicked()));
	connect(ui.tableWidget, SIGNAL(cellEntered(int, int)), this, SLOT(ShowItemInfo(int, int)));
	connect(ui.tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowContextMenu(QPoint)));

	connect(action_use, SIGNAL(triggered(bool)), this, SLOT(on_action_use(bool)));
	connect(action_storage, SIGNAL(triggered(bool)), this, SLOT(on_action_storage(bool)));
	connect(action_sale, SIGNAL(triggered(bool)), this, SLOT(on_action_sale(bool)));
}

item_equipBag::~item_equipBag()
{

}

void item_equipBag::updateInfo()
{
	quint32 row_Count = ui.tableWidget->rowCount();
	quint32 Col_Count = ui.tableWidget->columnCount();
	quint32 row_cur = 0;
	quint32 col_cur = 0;

	QString strTmp = "";

	pages = (m_item->size() + row_Count * Col_Count - 1 ) / (row_Count * Col_Count);
	if (pages == 0)
		pages = 1;
	if (CurrentPage > pages)
		CurrentPage = pages;

	ui.edit_page_cur->setText(QString::number(CurrentPage));
	if (pages == 1)
	{
		ui.btn_pgDn->setEnabled(false);
		ui.btn_pgUp->setEnabled(false);
	}
	else if (CurrentPage == 1)
	{
		ui.btn_pgDn->setEnabled(true);
		ui.btn_pgUp->setEnabled(false);
	}
	else if (CurrentPage >= pages)
	{
		ui.btn_pgDn->setEnabled(false);
		ui.btn_pgUp->setEnabled(true);
	}
	else
	{
		ui.btn_pgDn->setEnabled(true);
		ui.btn_pgUp->setEnabled(true);
	}

	//必须先清除背包显示，否则当前装备数量小于之前数量时会在最尾显示原装备的假像。
	ui.tableWidget->clear();
	ListEquip::const_iterator iter = m_item->constBegin();
	for (quint32 i = 0; i < (CurrentPage - 1) * (row_Count * Col_Count); i++, iter++) { ; }

	for (; iter != m_item->constEnd(); iter++)
	{	
		const Info_basic_equip *EquipBasicInfo = GetEquipBasicInfo(iter->ID);
		if (EquipBasicInfo == nullptr)
		{
			continue;
		}
		ui.tableWidget->setItem(row_cur, col_cur++, new QTableWidgetItem(EquipBasicInfo->icon, strTmp));
		if (col_cur >= Col_Count)
		{
			++row_cur;
			col_cur = 0;
		}
	}
}

void item_equipBag::on_btn_pgUp_clicked()
{
	if (CurrentPage > 1)
	{
		--CurrentPage;
		updateInfo();
	}
}
void item_equipBag::on_btn_pgDn_clicked()
{
	if (CurrentPage < pages)
	{
		++CurrentPage;
		updateInfo();
	}
}

void item_equipBag::ShowItemInfo(int row, int column)
{
	ShowItemInfo_equip(row, column, CurrentPage, m_item);
}

void item_equipBag::ShowContextMenu(QPoint pos)
{
	g_dlg_detail->hide();

	//右击非空白单元格，才弹出右键菜单。
	if (m_item->size() > GetCurrentCellIndex(CurrentPage))
	{
		popMenu->exec(ui.tableWidget->mapToGlobal(pos));
	}
}

void item_equipBag::on_action_use(bool checked)
{
	quint32 index = GetCurrentCellIndex(CurrentPage);
	const Info_Equip equip = m_item->at(index);
	quint32 nTmp;
	QString message = QStringLiteral("你未达到穿戴此装备的最低要求！");

	const Info_basic_equip *EquipBasicInfo = Item_Base::GetEquipBasicInfo(equip.ID);
	if (EquipBasicInfo == nullptr)
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
		bSatisfy = (player->get_lv() >= EquipBasicInfo->needLvl);
		break;
	case 1: 
		bSatisfy = (player->get_dc2() >= EquipBasicInfo->needLvl);
		break;
	case 2: 
		bSatisfy = (player->get_mc2() >= EquipBasicInfo->needLvl);
		break;
	case 3: 
		bSatisfy = (player->get_sc2() >= EquipBasicInfo->needLvl);
		break;
	default:
		break;
	}
	if (Type == g_equipType_clothes_m || Type == g_equipType_clothes_f)
	{
		//当前装备为衣服，需判断性别。
		bool bTmp = (player->get_gender() == (Type - 1));
		bSatisfy = bSatisfy && bTmp;
		if (!bTmp)
		{
			QString strTmp = (Type == g_equipType_clothes_m) ? QStringLiteral("男") : QStringLiteral("女");
			message = QStringLiteral("性别不符,此装备为%1性装备").arg(strTmp);
		}	
	}

	if (bSatisfy)	{		
		emit wearEquip(equip.ID, index);		
	}	else	{
		QMessageBox::critical(g_widget, QStringLiteral("提示"), message);
	}
}
void item_equipBag::on_action_storage(bool checked)
{
	quint32 index = GetCurrentCellIndex(CurrentPage);
	const Info_Equip equip = m_item->at(index);

	if (m_storageItem->size() >= g_storage_maxSize)
	{
		QString message = QStringLiteral("仓库已满！");
		QMessageBox::critical(g_widget, QStringLiteral("提示"), message);
	}
	else
	{
		m_storageItem->append(equip);
		m_item->removeAt(index);

		emit UpdateEquipInfoSignals();
	}
}
void item_equipBag::on_action_sale(bool checked)
{
	quint32 index = GetCurrentCellIndex(CurrentPage);
	const Info_Equip equip = m_item->at(index);

	const Info_basic_equip *EquipBasicInfo = GetEquipBasicInfo(equip.ID);
	if (EquipBasicInfo != NULL)
	{
		player->set_coin(player->get_coin() + (EquipBasicInfo->price >> 1));	//一半价格卖出

		g_falseRole.coin += EquipBasicInfo->price >> 1;

		emit UpdatePlayerInfoSignals();

		m_item->removeAt(index);
		updateInfo();
	}
}

void item_equipBag::on_btn_sale_clicked()
{
	QString message = QStringLiteral("点击确认将售出背包内所有装备(不含神器)，是否确认？");
	QMessageBox msgBox(QMessageBox::Information, QStringLiteral("一键销售"), message);
	QPushButton *YsBtn = msgBox.addButton(QStringLiteral(" 确认 "), QMessageBox::AcceptRole);
	QPushButton *NoBtn = msgBox.addButton(QStringLiteral(" 取消 "), QMessageBox::RejectRole);
	msgBox.exec();
	if (msgBox.clickedButton() == YsBtn)
	{
		ListEquip tmp;
		for (ListEquip::const_iterator iter = m_item->begin(); iter != m_item->end(); iter++)
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
					player->set_coin(player->get_coin() + (EquipBasicInfo->price >> 2));		//一键销售只有1/4价格

					g_falseRole.coin += EquipBasicInfo->price >> 2;
				}
			}
		}
		m_item->clear();
		for (ListEquip::const_iterator iter = tmp.begin(); iter != tmp.end(); iter++)
		{
			m_item->append(*iter);
		}
		emit UpdatePlayerInfoSignals();
	}
}
void item_equipBag::on_btn_sort_clicked()
{
	ListEquip tmp;
	quint32 i, j, k;
	
	//选择排序，按品质。
	//遍历一次背包，便将同一品质装备加入到临时列表中。故算法速度约为O(C * n) * 2,可直接简写为O(n).
	k = g_specialEquip_MaxExtra;
	j = 0;
	while (k--)
	{
		for (i = j; i< m_item->size(); i++)
		{
			if (m_item->at(i).extraAmount == k)
			{
				tmp.append(m_item->at(i));
				
				m_item->swap(i, j);
				++j;
			}
		}
	}

	m_item->clear();
	for (ListEquip::const_iterator iter = tmp.begin(); iter != tmp.end(); iter++)
	{
		m_item->append(*iter);
	}
	emit UpdatePlayerInfoSignals();
}