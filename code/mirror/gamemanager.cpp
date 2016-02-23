#include "gamemanager.h"


void GameManager::Init(int32_t w_MaxMap, int32_t w_keepSign, uint32_t w_preSignTime, int32_t count_daysTask0, int32_t count_daysTask1, int32_t count_daysTask2)
{
	if (bWasInit)
	{
		return;
	}
	xorkey = qrand();
	MaxMap = w_MaxMap ^ xorkey;
	keepSign = w_keepSign ^ xorkey;
	preSignTime = w_preSignTime ^ xorkey;

	fis = { 0 };

	taskMgr.Init();
	taskMgr.Set_RemainCount(count_daysTask0, count_daysTask1, count_daysTask2);

	bWasInit = true;
}

