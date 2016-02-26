#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QObject>
#include "task.h"
#include "def_DataType.h"

class GameManager : public QObject
{
	Q_OBJECT
		friend class fight_fight;
public:
	static GameManager& getInstance()
	{
		static GameManager Instance;
		return Instance;
	}

	//此函数仅第一次调用有效，后续调用直接返回。
	void Init(void);

	//注册基本信息
	void regInfo(int32_t MaxMap, int32_t w_keepSign, uint32_t w_preSignTime, int32_t count_daysTask0, int32_t count_daysTask1, int32_t count_daysTask2);

	qint32 get_taskListExceptComplete(QList<task::taskItem> &taskOnDoing) {
		return taskMgr.get_taskListExceptComplete(taskOnDoing);
	}

	//
	const task::taskItem &Get_taskItem(qint32 id) const {
		return taskMgr.Get_taskItem(id);
	}

	bool acceptTask(qint32 id) {
		bool bRes = taskMgr.acceptTask(id);
		if (bRes) {
			fis = { 0 };
		}
		return bRes;
	}
	bool completeTask(qint32 id) {
		return taskMgr.completeTask(id);
	}
	bool refreshTask(qint32 id) {
		return taskMgr.refreshTask(id);
	}

	qint32 DisplayTasks(QTreeWidget *widget) const {
		return taskMgr.DisplayTasks(widget);
	}

	void RegFIS(FightInfoStatistics w_fis) { fis = w_fis; }
	const FightInfoStatistics& get_FIS(void) const {
		return fis;
	}

	qint32 get_RemainDaysTaskCount(qint32 index) const {
		return taskMgr.get_RemainDaysCount(index);
	}
	void reset_DaysTaskCount(void){
		qint32 nTmp = 3;
		taskMgr.Set_RemainCount(nTmp, nTmp, nTmp);
	}

	int32_t get_maxMapID(void) const { return MaxMap ^ xorkey; }
	void Set_maxMapID(int32_t no) { MaxMap = no ^ xorkey; }

	void add_keepSign(void) { keepSign = (get_keepSign() + 1) ^ xorkey; }
	void reset_keepSign(void) { keepSign = 1 ^ xorkey; }
	int32_t get_keepSign(void) const { return keepSign ^ xorkey; }

	void set_SignTime(uint32_t t) { preSignTime = t ^ xorkey; }
	uint32_t get_preSignTime(void) const { return preSignTime ^ xorkey; }

private:
	GameManager() { bWasInit = false; }
	GameManager(const GameManager &);
	GameManager & operator= (const GameManager &);

protected:
	qint32 xorkey;

private:
	bool bWasInit;
	int32_t FightEdtIndex, MaxMap, keepSign;
	uint32_t preSignTime;

	task taskMgr;

	FightInfoStatistics fis;		//实时战斗统计信息(仅统计需要统计的数据）
};

#define GameMgrIns GameManager::getInstance()

#endif // GAMEMANAGER_H
