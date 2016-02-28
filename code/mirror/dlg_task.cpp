#include "dlg_task.h"
#include "def_System_para.h"
#include "MonsterDefine.h"
#include "ItemDefine.h"
#include "def_item_equip.h"
#include "Player.h"

extern QMap<mapID, Info_Distribute> g_MonsterDistribute;
extern QMap<monsterID, MonsterInfo> g_MonsterInfo;
extern QMap<itemID, Info_Item> g_ItemList;
extern QMap<itemID, Info_basic_equip> g_EquipList;

dlg_task::dlg_task(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	initUI();

	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, qint32)), this, SLOT(itemClicked(QTreeWidgetItem *, qint32)));
}

void dlg_task::initUI()
{
	GameMgrIns.DisplayTasks(ui.treeWidget);

	ui.btn_accpet->setEnabled(false);
	ui.btn_refresh->setEnabled(false);

	ui.treeWidget->expandAll();
	ui.treeWidget->hideColumn(1);

	init_currentTask();
}

void dlg_task::init_currentTask()
{
	QTreeWidgetItem *top = ui.treeWidget->topLevelItem(2);
	if (top != nullptr)
	{
		QTreeWidgetItem *item = top->child(0);
		if (item != nullptr)
		{
			ui.treeWidget->setCurrentItem(item);
			itemClicked(item, 0);
		}
	}
}

dlg_task::~dlg_task()
{

}

void dlg_task::itemClicked(QTreeWidgetItem * item, int column)
{
	QTreeWidgetItem *parent = item->parent();
	if (parent == nullptr)
	{
		return;
	}
	QString str = "";
	qint32 nTmp;
	qint32 id = item->text(1).toInt();

	qint32 taskType = id / 1000;
	qint32 taskIndex = id % 1000;
	if (taskType == 2) 	{
		nTmp = GameMgrIns.get_RemainDaysTaskCount(taskIndex);
		if (nTmp == 0) 	{
			str = QStringLiteral("(剩余<font color = darkRed>%1</font>次)").arg(nTmp);
		} else {
			str = QStringLiteral("(剩余<font color = green>%1</font>次)").arg(nTmp);
		}
	} 
	ui.lbl_name->setText(item->text(0) + str);
		
	GeneralTaskInfo(id, str);
	ui.lbl_info->setText(str);

	GeneralReward(id, str);
	ui.lbl_reward->setText(str);

	checkTaskStatus(id);
}

qint32 dlg_task::GeneralTaskInfo(qint32 id, QString &str)
{
	const task::taskItem& item = GameMgrIns.Get_taskItem(id);

	switch (item.tType)
	{
	case task::tt_HoldRound:GeneralTaskInfo_HoldRound(item, str); break;
	case task::tt_KillMonster: GeneralTaskInfo_KillMonster(item, str); break;
	case task::tt_Item:GeneralTaskInfo_Item(item.tID, item.tCount, str); break;
	case task::tt_Level:str = QStringLiteral("等级达到%1级").arg(item.tCount); break;
	case task::tt_Coin:str = QStringLiteral("金币达到%1").arg(item.tCount); break;
	case task::tt_Gold:str = QStringLiteral("元宝达到%1").arg(item.tCount); break;
	case task::tt_Rep:str = QStringLiteral("声望达到%1").arg(item.tCount); break;
		break;
	default:
		str = QStringLiteral("错误的任务目标"); break;
		break;
	}
	return 0;
}

void dlg_task::GeneralTaskInfo_HoldRound(const task::taskItem& item, QString &str)
{
	if (!g_MonsterDistribute.contains(item.tMap))
	{
		str = QStringLiteral("错误的地图ID：%1").arg(item.tMap);
		return;
	}
	const Info_Distribute &dis = g_MonsterDistribute.value(item.tMap);

	const FightInfoStatistics& fisc = GameMgrIns.get_FIS();
	qint32 count = (fisc.whatsMap == item.tMap ? fisc.nCount_StraightVictory : 0);

	str = QStringLiteral("在<font color = cyan>%1</font>坚守<font color = cyan>%2</font>回合").arg(dis.name).arg(item.tCount);
	if (item.ts == task::ts_Doing)
	{
		if (count >= item.tCount) {
			str += QStringLiteral("（<font color = green>已完成</font>）");
		}
		else {
			str += QStringLiteral("（<font color = darkRed>最高记录:%1</font>）").arg(count);
		}
	}	
}
void dlg_task::GeneralTaskInfo_KillMonster(const task::taskItem& item, QString &str)
{
	if (!g_MonsterInfo.contains(item.tID))
	{
		str = QStringLiteral("错误的怪物ID：%1").arg(item.tID);
		return;
	}
	if (!g_MonsterDistribute.contains(item.tMap))
	{
		str = QStringLiteral("错误的地图ID：%1").arg(item.tMap);
		return;
	}

	const Info_Distribute &dis = g_MonsterDistribute.value(item.tMap);
	const MonsterInfo &info = g_MonsterInfo.value(item.tID);
	const FightInfoStatistics& fisc = GameMgrIns.get_FIS();
	qint32 kill = fisc.killMonster.value(item.tID);

	str = QStringLiteral("前往<font color = cyan>%1</font>击杀<font color = cyan>%2</font>").arg(dis.name).arg(info.name);
	if (item.ts == task::ts_Doing)
	{
		if (kill >= item.tCount) {
			str += QStringLiteral("（<font color = green>已完成</font>）");
		}
		else {
			str += QStringLiteral("（<font color = darkRed>%1/%2</font>）").arg(kill).arg(item.tCount);
		}
	}
}

void dlg_task::GeneralTaskInfo_Item(qint32 tID, qint32 tCount, QString &str)
{
	QString itemName;
	qint32 nCount = 0;
	if (g_EquipList.contains(tID))
	{
		itemName = g_EquipList.value(tID).name;
	}
	else if(g_ItemList.contains(tID))
	{
		itemName = g_ItemList.value(tID).name;

		nCount = PlayerIns.get_bag_item().value(tID);
	}
	else
	{
		str = QStringLiteral("错误的道具ID：%1").arg(tID);
		return;
	}

	str = QStringLiteral("收集<font color = cyan>%1</font>，共%2/%3").arg(itemName).arg(nCount).arg(tCount);
}

qint32 dlg_task::GeneralReward(qint32 id, QString &str)
{
	const task::taskItem& item = GameMgrIns.Get_taskItem(id);
	QString strTmp;
	if (item.rType == task::rt_Item)
	{
		if (item.rID > g_itemID_start_item && item.rID <= g_itemID_stop_item)
		{
			strTmp = g_ItemList.value(item.rID).name;
		}
		else if (item.rID > g_itemID_start_equip && item.rID <= g_itemID_stop_equip)
		{
			strTmp = g_EquipList.value(item.rID).name;
		}
	}
	switch (item.rType)
	{
	case task::rt_Item:str = QStringLiteral("%1 %2件").arg(strTmp).arg(item.rCount); break;
	case task::rt_Exp: str = QStringLiteral("经验：%1").arg(item.rCount); break;
	case task::rt_Coin:str = QStringLiteral("金币：%1").arg(item.rCount); break;
	case task::rt_Gold:str = QStringLiteral("元宝：%1").arg(item.rCount); break;
	case task::rt_Rep:str = QStringLiteral("声望：%1").arg(item.rCount); break;
	case task::rt_YuanLi:str = QStringLiteral("元力值：%1").arg(item.rCount); break;
		break;
	default:
		str = QStringLiteral("错误的任务目标"); break;
		break;
	}
	return 0;
}

void dlg_task::checkTaskStatus(qint32 id)
{
	const task::taskItem& item = GameMgrIns.Get_taskItem(id);
	if (item.ts == task::ts_NotAccepted && PlayerIns.get_coin() >= 20000) {
		ui.btn_refresh->setEnabled(true);
	} else {
		ui.btn_refresh->setEnabled(false);
	}

	bool bComplete = false;
	bool btn_accpet_status = false;

	if (item.ts == task::ts_NotAccepted)
	{
		qint32 taskType = id / 1000;
		qint32 taskIndex = id % 1000;
		if (taskType == 2)
		{
			if (GameMgrIns.get_RemainDaysTaskCount(taskIndex) > 0)
			{
				btn_accpet_status = true;
			}
		}
		ui.btn_accpet->setText(QStringLiteral("接受任务"));	
	}
	else if (item.ts == task::ts_Doing)
	{
		bComplete = wasComplete(item);
		btn_accpet_status = bComplete;

		if (bComplete) 	{
			ui.btn_accpet->setText(QStringLiteral("领取奖励"));
		} else {
			ui.btn_accpet->setText(QStringLiteral("已接受"));
		}
	}
	else if (item.ts == task::ts_Complete) 
	{
		ui.btn_accpet->setText(QStringLiteral("已完成"));
	}
	ui.btn_accpet->setEnabled(btn_accpet_status);
	ui.lbl_status->setVisible(bComplete);
}

bool dlg_task::wasComplete(const task::taskItem &item)
{
	bool bComplete = false;
	const FightInfoStatistics& fis = GameMgrIns.get_FIS();

	switch (item.tType)
	{
	case task::tt_HoldRound:bComplete = ((fis.whatsMap == item.tMap) && (fis.nCount_StraightVictory >= item.tCount)); break;
	case task::tt_KillMonster:bComplete = (fis.killMonster.value(item.tID) >= item.tCount);	break;
	case task::tt_Item:
		//暂未完成，以后研究
		bComplete = false;
		break;
	case task::tt_Level:bComplete = (PlayerIns.get_edt_role().get_lv() >= item.tCount); break;
	case task::tt_Coin:bComplete = (PlayerIns.get_coin() >= item.tCount); break;
	case task::tt_Gold:bComplete = (PlayerIns.get_gold() >= item.tCount); break;
	case task::tt_Rep:bComplete = (PlayerIns.get_rep() >= item.tCount); break;
		break;
	default:
		//nothing
		break;
	}
	return bComplete;
}

void dlg_task::on_btn_accpet_clicked(void)
{
	QTreeWidgetItem *it = ui.treeWidget->currentItem();
	qint32 id = it->text(1).toInt();

	//为防止他人使用“灰暗钮点击”等插件，此处必须再次判断。
	const task::taskItem& item = GameMgrIns.Get_taskItem(id);
	if (item.ts == task::ts_NotAccepted) 
	{
		qint32 taskType = id / 1000;
		qint32 taskIndex = id % 1000;
		if (taskType == 2 && GameMgrIns.get_RemainDaysTaskCount(taskIndex) > 0)
		{
			GameMgrIns.acceptTask(id);
		}
	} 
	else if(item.ts == task::ts_Doing)
	{
		if (wasComplete(item))
		{
			if (GameMgrIns.completeTask(id))
			{
				//更新信息
			}
		}
	}
	else
	{
		//error status or complete, nothing to do.
	}
	itemClicked(it, 0);
}
void dlg_task::on_btn_refresh_clicked(void)
{
	QTreeWidgetItem *it = ui.treeWidget->currentItem();
	qint32 id = it->text(1).toInt();

	PlayerIns.sub_coin(20000);
	GameMgrIns.refreshTask(id);

	itemClicked(it, 0);
}
