#include "Player.h"

void CPlayer::Init()
{
	xorkey = qrand();

	embodiment[0].get_skill_study().clear();
	embodiment[0].get_skill_study().clear();
	embodiment[0].get_skill_study().clear();
	embodiment[0].get_skill_study().clear();
	
	bag_item.clear();
	bag_equip.clear();
	storage_equip.clear();
}

void CPlayer::Set_BasicInfo(int32_t w_ID_H, int32_t w_ID_L, int32_t w_level, int32_t w_exp, int32_t w_coin, int32_t w_gold, int32_t w_rep, int32_t w_soul)
{
	accountId_H = w_ID_H;
	accountId_L = w_ID_L;
	level = w_level ^ xorkey;
	exp = w_exp ^ xorkey;
	coin = w_coin ^ xorkey;
	gold = w_gold ^ xorkey;
	reputation = w_rep ^ xorkey;
	soul = w_soul ^ xorkey;
}

void CPlayer::Set_ExtraInfo(int32_t w_FightEdtIndex, int32_t w_MaxMap)
{
	FightEdtIndex = w_FightEdtIndex;
	MaxMap = w_MaxMap;
}
