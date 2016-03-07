#include "task.h"
#include "Player.h"
#include "MonsterDefine.h"
#include "gamemanager.h"

extern QMap<mapID, Info_Distribute> g_MonsterDistribute;
extern QMap<itemID, Info_basic_equip> g_EquipList;
extern QMap<itemID, Info_Item> g_ItemList;

void task::Init()
{
	xorkey = qrand();

	tasks.resize(3);
	Init_GudidTask();
	Init_branchTask();
	Init_daysTask();

	errorTask = { 0 };
	errorTask.name = QStringLiteral("错误的任务");
	errorTask.ts = ts_Error;
}

void task::Init_GudidTask(void)
{
	//nothing
}
void task::Init_branchTask(void)
{
	//nothing
}
void task::Init_daysTask(void)
{
	QVector<taskItem> &days = tasks[2];
	days.resize(3);

	Init_days(0);
	Init_days(1);
	Init_days(2);
}

void task::Init_days(qint32 index)
{
	taskItem &which = tasks[2][index];

	switch (index)
	{
	case 0:
		Init_days_KillMonster(which);
		break;
	case 1:
		Init_days_ProtectMine(which);
		break;
	case 2:
		Init_days_Item(which);
		break;
	default:
		break;
	}
}

void task::Init_days_ProtectMine(taskItem &which)
{
	which.name = QStringLiteral("坚守矿场");
	which.ts = ts_NotAccepted;
	which.tType = tt_HoldRound;
	which.tMap = 4;
	which.tID = 0;
	which.tCount = 25 + qrand() % 10;
	which.rType = rt_Coin;
	which.rID = 0;
	which.rCount = 100;
}

void task::Init_days_Item(taskItem &which)
{
	//火球、基本、治愈三选一
	qint32 id = 220001 + qrand() % 3;

	which.name = QStringLiteral("物资筹建");
	which.ts = ts_NotAccepted;
	which.tType = tt_Item;
	which.tID = id;
	which.tCount = 5;
	which.rType = rt_Rep;
	which.rID = 0;
	which.rCount = 20;
}
void task::Init_days_KillMonster(taskItem &which)
{
	const qint32 offset = 4;
	qint32 maxMap = GameMgrIns.get_maxMapID();
	qint32 targeMap = maxMap - qrand() % offset;
	if (!g_MonsterDistribute.contains(targeMap))
	{
		targeMap = 1;
	}
	const Info_Distribute &dis = g_MonsterDistribute.value(targeMap);

	which.name = QStringLiteral("降妖伏魔");
	which.ts = ts_NotAccepted;
	which.tType = tt_KillMonster;
	which.tMap = dis.ID;
	which.tID = dis.normal.at(qrand() % dis.normal.count());
	which.tCount = 30;
	which.rType = rt_YuanLi;
	which.rID = 0;
	which.rCount = dis.need_lv * 10;
}

qint32 task::DisplayTasks(QTreeWidget *widget) const
{
	qint32 nTaskID;
	qint32 i = 0;
	QStringList str = { QStringLiteral("主线"), QStringLiteral("支线"), QStringLiteral("日常") };
	for (auto iter = str.constBegin(); iter != str.constEnd(); iter++)
	{
		nTaskID = i * 1000;
		++i;

		QTreeWidgetItem *propType = new QTreeWidgetItem;;
		propType->setTextColor(0, QColor(0, 180, 0));
		propType->setText(0, *iter);
		propType->setText(1, QString::number(nTaskID));
		widget->addTopLevelItem(propType);
	}

	Display_GuideTask(widget);
	Display_branchTask(widget);
	Display_daysTask(widget);
	return 0;
}

void task::Display_GuideTask(QTreeWidget *widget) const
{
	Q_UNUSED(widget);
	return;
}
void task::Display_branchTask(QTreeWidget *widget) const
{
	Q_UNUSED(widget);
	return;
}

void task::Display_daysTask(QTreeWidget *widget) const
{
	QTreeWidgetItem *parent = widget->topLevelItem(2);
	if (parent == nullptr)
	{
		return;
	}

	qint32 i = 0;
	qint32 TaskID_main = parent->text(1).toInt();
	const QVector<taskItem> &days = tasks.value(2);
	for (auto iter = days.constBegin(); iter != days.constEnd(); iter++)
	{
		QTreeWidgetItem *propItem = new QTreeWidgetItem(parent);
		propItem->setTextColor(0, QColor(255, 255, 255));
		propItem->setText(0, iter->name);
		propItem->setText(1, QString::number(TaskID_main + i));

		++i;
	}
}

const task::taskItem &task::Get_taskItem(qint32 id) const
{
	qint32 taskType = id / 1000;
	qint32 taskIndex = id % 1000;

	if (id < 0 || taskType >= tasks.count() || taskIndex >= tasks.value(taskType).count())
	{
		return errorTask;
	}

	return tasks[taskType][taskIndex];
}

qint32 task::get_taskListExceptComplete(QList<task::taskItem> &taskOnDoing)
{
	taskOnDoing.clear();

	for (auto iterType = tasks.constBegin(); iterType != tasks.constEnd(); iterType++)
	{
		for (auto iterTask = iterType->constBegin(); iterTask != iterType->constEnd(); iterTask++)
		{
			if (iterTask->ts == ts_NotAccepted || iterTask->ts == ts_Doing)
			{
				taskOnDoing.append(*iterTask);
			}			
		}
	}
	return 0;
}

bool task::acceptTask(qint32 id)
{
	qint32 taskType = id / 1000;
	qint32 taskIndex = id % 1000;
	
	if (id< 0 || taskType >= tasks.count() || taskIndex >= tasks.value(taskType).count() )
	{
		return false;
	}

	sub_RemainDaysCount(taskIndex);
	taskItem& item = tasks[taskType][taskIndex];
	if (item.ts == ts_NotAccepted)
	{
		item.ts = ts_Doing;
		return true;
	}

	return false;
}
bool task::completeTask(qint32 id)
{
	qint32 taskType = id / 1000;
	qint32 taskIndex = id % 1000;

	if (id < 0 || taskType >= tasks.count() || taskIndex >= tasks.value(taskType).count())
	{
		return false;
	}

	taskItem& item = tasks[taskType][taskIndex];
	if (item.ts == ts_Doing)
	{
		GiveReward(item.rType, item.rID, item.rCount);
		Init_days(taskIndex);
		return true;
	}

	return false;
}

void task::GiveReward(RewardType rType, qint32 rID, qint32 rCount)
{
	switch (rType)
	{
	case task::rt_Item:
		if (g_EquipList.contains(rID))
		{
			ListEquip &bag = PlayerIns.get_bag_equip();
			Info_Equip eq = { 0 };
			eq.ID = rID;
			for (int i = 0; i < rCount; i++)
			{
				bag.append(eq);
			}		
		}
		else if (g_ItemList.contains(rID))
		{
			MapItem &bag = PlayerIns.get_bag_item();
			bag.insert(rID, bag.value(rID) + rCount);
		}
		break;
	case task::rt_Exp: PlayerIns.get_edt_role().add_exp(rCount); break;
	case task::rt_Coin:PlayerIns.add_coin(rCount); break;
	case task::rt_Gold:PlayerIns.add_gold(rCount); break;
	case task::rt_Rep:PlayerIns.add_rep(rCount); break;
	case task::rt_YuanLi:PlayerIns.get_edt_role().add_yuanli(rCount); break;
		break;
	default:
		//error, nothing
		break;
	}
}

bool task::refreshTask(qint32 id)
{
	qint32 taskType = id / 1000;
	qint32 taskIndex = id % 1000;

	//只有日常任务(索引为2)允许刷新
	if (id < 0 || taskType != 2 || taskIndex >= tasks.value(taskType).count())
	{
		return false;
	}
	Init_days(taskIndex);
	return true;
}