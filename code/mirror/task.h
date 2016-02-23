#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>
#include <QVector>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class task : public QObject
{
	Q_OBJECT

public:
	enum Status {
		ts_Error,
		ts_Complete,
		ts_Doing,
		ts_NotAccepted
	};

	enum TargetType
	{
		tt_HoldRound,
		tt_KillMonster,
		tt_Item,
		tt_Level,
		tt_Coin,
		tt_Gold,
		tt_Rep
	};

	enum RewardType
	{
		rt_Item,
		rt_Exp,
		rt_Coin,
		rt_Gold,
		rt_Rep,
		rt_YuanLi
	};

	struct taskItem {
		QString name;		//任务名
		Status ts;			//任务状态
		TargetType tType;	//目标类型
		qint32 tID;			//当tType=tt_HoldRound,此为MapID;当tType=tt_KillMonster，此为怪物id;当tType=tt_Item，此为道具id;tType其他取值，则无意义
		qint32 tCount;		//目标数量
		RewardType rType;	//奖励类型
		qint32 rID;			//当rType=rt_Item，此为道具id。rType为其他取值时无意义。
		qint32 rCount;		//奖励数量
	};

public:
	task() {  };
	~task() {  };

	void Init(void);

	void Set_RemainCount(qint32 count_daysTask0, qint32 count_daysTask1, qint32 count_daysTask2) {
		count_days[0] = count_daysTask0 ^ xorkey;
		count_days[1] = count_daysTask1 ^ xorkey;
		count_days[2] = count_daysTask2 ^ xorkey;
	}
	qint32 get_RemainDaysCount(qint32 index) const{
		return count_days[index] ^ xorkey;
	}

	qint32 DisplayTasks(QTreeWidget *widget) const;
	const taskItem &Get_taskItem(qint32 id) const;

	bool acceptTask(qint32 id);
	bool completeTask(qint32 id);
	bool refreshTask(qint32 id);

private:
	void Init_daysTask(void);
	void Init_GudidTask(void);
	void Init_branchTask(void);

	void Init_days(qint32 index);
	void Init_days_ProtectMine(taskItem &which);
	void Init_days_KillMonster(taskItem &which);
	void Init_days_Item(taskItem &which);

	void Display_daysTask(QTreeWidget *widget) const;
	void Display_GuideTask(QTreeWidget *widget) const;
	void Display_branchTask(QTreeWidget *widget) const;

	void GiveReward(RewardType rType, qint32 rID, qint32 rCount);

	void sub_RemainDaysCount(qint32 index) {
		qint32 nTmp = count_days[index] ^ xorkey;
		--nTmp;
		count_days[index] = nTmp ^ xorkey;
	}
private:
	qint32 xorkey;

	QVector<QVector<taskItem>> tasks;
	qint32 count_days[3];
	taskItem errorTask;
};

#endif // TASKMANAGER_H
