#include <QMessageBox>

#include "item_equipbag.h"

extern QVector<Info_equip> g_EquipList;


item_equipBag::item_equipBag(RoleInfo *info, ListEquip *item, ListEquip *storageItem)
	: myRole(info), m_item(item), m_storageItem(storageItem)
{
	ui.btn_sale->setVisible(true);
//	ui.btn_sort->setVisible(true);
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

	popMenu = new QMenu();
	action_use = new QAction(QStringLiteral("装备"), this);
	action_storage = new QAction(QStringLiteral("入库"), this);
	action_sale = new QAction(QStringLiteral("销售"), this);
	popMenu->addAction(action_use);
	popMenu->addAction(action_storage);
	popMenu->addAction(action_sale);

	connect(ui.btn_sale, SIGNAL(clicked()), this, SLOT(on_btn_sale_clicked()));
	connect(ui.tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(ShowItemInfo(int, int)));
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
	quint32 ID, nCount;
	QString Name;

	//必须先清除背包显示，否则当前装备数量小于之前数量时会在最尾显示原装备的假像。
	ui.tableWidget->clear();
	for (ListEquip::const_iterator iter = m_item->begin(); iter != m_item->end(); iter ++)
	{
		const Info_equip *itemItem = FindItem_Equip(*iter);
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

		if (row_cur >= row_Count)
		{
			//添加到第二页。
			break;	//暂不处理
		}
	}
}

void item_equipBag::ShowItemInfo(int row, int column)
{
	ShowItemInfo_equip(row, column, m_item, myRole);
}

void item_equipBag::ShowContextMenu(QPoint pos)
{
	popMenu->exec(ui.tableWidget->mapToGlobal(pos));
}

void item_equipBag::on_action_use(bool checked)
{
	int row = ui.tableWidget->currentRow();
	int col = ui.tableWidget->currentColumn();
	quint32 ID = GetItemID(row, col, m_item);
	quint32 index = row * ui.tableWidget->columnCount() + col;

	const Info_equip *equip_new = Item_Base::FindItem_Equip(ID);
	if (equip_new == NULL)
	{
		return;		//unknown equipment ID！
	}
	//获取待佩带装备的类别
	int Type = (ID % 100000) / 1000;

	//查询角色当前属性是否符合佩带需要。
	bool bSatisfy = false;
	switch (equip_new->need)
	{
	case 0: bSatisfy = (myRole->level >= equip_new->needLvl); break;
	case 1: bSatisfy = (myRole->dc2 > equip_new->needLvl); break;
	case 2: bSatisfy = (myRole->mc2 > equip_new->needLvl); break;
	case 3: bSatisfy = (myRole->sc2 > equip_new->needLvl); break;
	default:
		break;
	}
	if (Type == 2 || Type == 3)
	{
		//当前装备为衣服，需判断性别。
		bSatisfy = bSatisfy && (myRole->gender == (Type - 1));
	}

	if (!bSatisfy)
	{
		QString message = QStringLiteral("你未达到穿戴此装备的最低要求！");
		QMessageBox::critical(this, QStringLiteral("提示"), message);
		return;
	}

	emit wearEquip(ID, index);
}
void item_equipBag::on_action_storage(bool checked)
{
	int row = ui.tableWidget->currentRow();
	int col = ui.tableWidget->currentColumn();
	quint32 ID = GetItemID(row, col, m_item);
	quint32 index = row * ui.tableWidget->columnCount() + col;

	m_item->removeAt(index);
	m_storageItem->append(ID);
	
	emit UpdateEquipInfoSignals();
}
void item_equipBag::on_action_sale(bool checked)
{
	int row = ui.tableWidget->currentRow();
	int col = ui.tableWidget->currentColumn();
	quint32 ID = GetItemID(row, col, m_item);
	quint32 index = row * ui.tableWidget->columnCount() + col;

	const Info_equip *equip = FindItem_Equip(ID);
	if (equip != NULL)
	{
		myRole->coin += equip->price >> 1;		//一半价格卖出
		emit UpdatePlayerInfoSignals();

		m_item->removeAt(index);
		updateInfo();
	}
}

void item_equipBag::on_btn_sale_clicked()
{
	QString message = QStringLiteral("点击确认将售出背包内所有装备，是否确认？");
	QMessageBox msgBox(QMessageBox::Information, QStringLiteral("一键销售"), message);
	QPushButton *YsBtn = msgBox.addButton(QStringLiteral(" 确认 "), QMessageBox::AcceptRole);
	QPushButton *NoBtn = msgBox.addButton(QStringLiteral(" 取消 "), QMessageBox::RejectRole);
	msgBox.exec();
	if (msgBox.clickedButton() == YsBtn)
	{
		for (ListEquip::const_iterator iter = m_item->begin(); iter != m_item->end(); iter++)
		{
			const Info_equip *equip = FindItem_Equip(*iter);
			if (equip != NULL)
			{
				myRole->coin += equip->price >> 2;		//一键销售只有1/4价格
			}
		}
		m_item->clear();
		
		emit UpdatePlayerInfoSignals();
	}
}
void item_equipBag::on_btn_sort_clicked()
{
	//找一个快速排序算法
}