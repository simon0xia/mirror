#ifndef _PLAYER_H
#define _PLAYER_H

#include "Human.h"

#define mir_min(a,b) (((a) < (b)) ? (a) : (b))

class CPlayer
{
private:
	const int32_t maxPoints = 200000000;
	
public:
	static CPlayer& getInstance()
	{
		static CPlayer Instance;
		return Instance;
	}

	void Init(void);
	void Set_BasicInfo(int32_t w_ID_H, int32_t w_ID_L, int32_t w_level, int32_t w_exp, int32_t w_coin, int32_t w_gold, int32_t w_rep, int32_t w_soul);
	void Set_ExtraInfo(int32_t w_FightEdtIndex, int32_t MaxMap);

	MapItem& get_bag_item(void)  { return bag_item; }
	ListEquip& get_bag_equip(void) { return bag_equip; }
	ListEquip& get_storage_equip(void) { return storage_equip; }
	CHuman& get_edt_role(void) { return embodiment[0]; }				//获取本尊
	CHuman& get_edt_warrior(void) { return embodiment[1]; }				//获取分身-战
	CHuman& get_edt_magic(void) { return embodiment[2]; }				//获取分身-法
	CHuman& get_edt_taoshi(void) { return embodiment[3]; }				//获取分身-道
	CHuman& get_edt_current(void) { return embodiment[CurEdtIndex]; }	//获取当前显示的分身(或本尊)
	CHuman& get_edt_Fight(void)	{ return embodiment[FightEdtIndex]; }	//获取参战的分身
	int32_t get_edt_Fight_index(void) const { return FightEdtIndex; }

	void set_edt_current(int32_t nIndex) { CurEdtIndex = nIndex; }
	void set_edt_fight(int32_t nIndex) { FightEdtIndex = nIndex;}

	void add_coin(int32_t no) {
		int32_t n = get_coin() + no;
		set_coin(mir_min(maxPoints, n));
	}
	void add_gold(int32_t no) {
		int32_t n = get_gold() + no;
		set_gold(mir_min(maxPoints, n));
	}
	void add_rep(int32_t no) {
		int32_t n = get_rep() + no;
		set_rep(mir_min(maxPoints, n));
	}
	void add_soul(int32_t no) {
		int32_t n = get_soul() + no;
		set_soul(mir_min(maxPoints, n));
	}

	void sub_coin(int32_t no) { set_coin(get_coin() - no); }
	void sub_gold(int32_t no) { set_gold(get_gold() - no); }
	void sub_rep(int32_t no) { set_rep(get_rep() - no); }
	void sub_soul(int32_t no) { set_soul(get_soul() - no); }

	int32_t get_id_H(void) const { return accountId_H; }
	int32_t get_id_L(void) const { return accountId_L; }
	int32_t get_lv(void) const  { return level ^ xorkey; }
	int32_t get_exp(void) const { return exp ^ xorkey; }
	int32_t get_coin(void) const{ return coin ^ xorkey; }
	int32_t get_gold(void) const{ return gold ^ xorkey; }
	int32_t get_rep(void) const { return reputation ^ xorkey; }
	int32_t get_soul(void)const { return soul ^ xorkey; }
	int32_t get_maxMapID(void) const { return MaxMap; }

	void set_Lv(int32_t n) { level = n ^ xorkey; }
	void set_exp(int32_t no) { exp = no ^ xorkey; }
	void set_coin(int32_t no) { coin = no ^ xorkey; }
	void set_gold(int32_t no) { gold = no ^ xorkey; }
	void set_rep(int32_t no) { reputation = no ^ xorkey; }
	void set_soul(int32_t no) { soul = no ^ xorkey; }
	void Set_maxMapID(int32_t no) { MaxMap = no; }

private:
	CPlayer() { ; };
	CPlayer(const CPlayer &);
	CPlayer & operator= (const CPlayer &);

private:
	int32_t xorkey;
	int32_t accountId_H;	//帐号ID高64位
	int32_t accountId_L;	//帐号ID低64位
	int32_t level;			//帐号等级
	int32_t exp;			//帐号经验
	int32_t coin;			//金币
	int32_t gold;			//元宝
	int32_t reputation;		//声望
	int32_t soul;			//灵魂点，获得途径：击败BOSS

	int32_t MaxMap;			//角色已通关的最高地图

	MapItem bag_item;			//道具背包
	ListEquip bag_equip;		//装备背包
	ListEquip storage_equip;	//装备仓库

	//本尊、分身-战、分身-法，分身-道
	CHuman embodiment[4];
	int32_t CurEdtIndex, FightEdtIndex;
};

#define PlayerIns CPlayer::getInstance()

#endif	//#ifndef _PLAYER_H