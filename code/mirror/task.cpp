#include "task.h"

task::task(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	int nTaskID;
	QString str[5] = { QStringLiteral("主线"), QStringLiteral("支线"), QStringLiteral("日常"), QStringLiteral("循环"), QStringLiteral("悬赏") };
	for (int i = 0; i < 5; i++)
	{
		nTaskID = 500000 + i * 1000;
		QTreeWidgetItem *propType = new QTreeWidgetItem(ui.treeWidget);
		propType->setText(0, str[i]);
		propType->setText(1, QString::number(nTaskID));
		for (int j = 1; j < 5; j++)
		{
			QTreeWidgetItem *propItem = new QTreeWidgetItem(propType);
			propItem->setText(0, QStringLiteral("等待添加"));
			propItem->setText(1, QString::number(nTaskID + j));
		}
	}
	ui.treeWidget->expandAll();
	ui.treeWidget->hideColumn(1);

	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, qint32)), this, SLOT(itemClicked(QTreeWidgetItem *, qint32)));
}

task::~task()
{

}

void task::itemClicked(QTreeWidgetItem * item, int column)
{
	ui.textEdit->setText(item->text(1));

// 	QString title = QStringLiteral("任务提示");
// 	if (g_roleAddition.taskStep >= g_task_main_list.size())
// 	{
// 		QMessageBox::information(this, title, QStringLiteral("没有可用任务"));
// 		return;
// 	}
// 	const info_task &task = g_task_main_list[g_roleAddition.taskStep];
// 
// 	QMessageBox *msgBox = new QMessageBox;
// 	QString strTmp = task.msg;
// 	QString strGiveItem;
// 	msgBox->setText(strTmp);
// 	msgBox->setWindowTitle(title);
// 
// 	strTmp = QStringLiteral("任务奖励:");
// 	strGiveItem = "";
// 	foreach(const itemID id, task.giveItem)
// 	{
// 		strGiveItem += " ";
// 		if (id > g_itemID_start_equip && id <= g_itemID_stop_equip)
// 		{
// 			const Info_basic_equip *EquipBasicInfo = Item_Base::GetEquipBasicInfo(id);
// 			if (EquipBasicInfo != nullptr)
// 			{
// 				strGiveItem += EquipBasicInfo->name + QStringLiteral(" 数量: ") + QString::number(task.giveCount);
// 			}
// 		}
// 		else if (id > g_itemID_start_item && id <= g_itemID_stop_item)
// 		{
// 			const Info_Item *itemInfo = Item_Base::FindItem_Item(id);
// 			if (itemInfo != nullptr)
// 			{
// 				strGiveItem += itemInfo->name + QStringLiteral(" 数量: ") + QString::number(task.giveCount);
// 			}
// 		}
// 		else
// 		{
// 			strGiveItem += QStringLiteral("未知道具 数量：1");
// 		}
// 	}
// 	msgBox->setInformativeText(strTmp + strGiveItem);
// 
// 	QPushButton *YsBtn = msgBox->addButton(QStringLiteral(" 我已经带过来了 "), QMessageBox::AcceptRole);
// 	QPushButton *NoBtn = msgBox->addButton(QStringLiteral(" 我这就去 "), QMessageBox::RejectRole);
// 	msgBox->setDefaultButton(NoBtn);
// 	msgBox->exec();
// 	bool bReject = (msgBox->clickedButton() == NoBtn);
// 	delete msgBox;
// 
// 	bool bTaskFinish = false;
// 
// 	if (bReject)
// 	{
// 		return;
// 	}
// 
// 	if (task.requireItem > g_itemID_start_equip && task.requireItem <= g_itemID_stop_equip)
// 	{
// 		for (quint32 i = 0; i < m_bag_equip->size(); i++)
// 		{
// 			if (m_bag_equip->at(i).ID == task.requireItem)
// 			{
// 				m_bag_equip->removeAt(i);
// 				bTaskFinish = true;
// 				break;
// 			}
// 		}
// 	}
// 	else if (task.requireItem > g_itemID_start_item && task.requireItem <= g_itemID_stop_item)
// 	{
// 		if (m_bag_item->value(task.requireItem) > task.requireCount)
// 		{
// 			m_bag_item->insert(task.requireItem, m_bag_item->value(task.requireItem) - task.requireCount);
// 			bTaskFinish = true;
// 		}
// 		else if (m_bag_item->value(task.requireItem) == task.requireCount)
// 		{
// 			m_bag_item->remove(task.requireItem);
// 			bTaskFinish = true;
// 		}
// 	}
// 	else
// 	{
// 		//nothing
// 	}
// 
// 
// 	if (bTaskFinish)
// 	{
// 		foreach(const itemID id, task.giveItem)
// 		{
// 			if (id > g_itemID_start_equip && id <= g_itemID_stop_equip)
// 			{
// 				Info_Equip equip = { 0 };
// 				equip.ID = id;
// 				m_bag_equip->append(equip);
// 
// 				m_tab_equipBag.updateInfo();
// 			}
// 			else if (id > g_itemID_start_item && id <= g_itemID_stop_item)
// 			{
// 				m_bag_item->insert(id, m_bag_item->value(id) + task.giveCount);
// 
// 				m_tab_itemBag.updateInfo();
// 			}
// 			else
// 			{
// 				//nothing;
// 			}
// 		}
// 
// 		QMessageBox *finishTaskBox = new QMessageBox;
// 		finishTaskBox->setText(QStringLiteral("获得物品： ") + strGiveItem);
// 		finishTaskBox->setWindowTitle(title);
// 		finishTaskBox->exec();
// 		delete finishTaskBox;
// 
// 		g_roleAddition.taskStep++;
// 	}
// 	else
// 	{
// 		QMessageBox::critical(this, title, QStringLiteral("小子，不要欺骗我老人家。"));
// 	}
}