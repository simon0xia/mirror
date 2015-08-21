#pragma once
#include "Organisms.h"
#include "def_DataType.h"
#include "RoleDefine.h"
#include "ItemDefine.h"

class CPlayer :
	public COrganisms
{
public:
	CPlayer(const char *w_name, RoleVoc w_voc, int32_t w_level, uint32_t w_gender, uint64_t w_coin, uint64_t w_gold, uint64_t w_rep);
	~CPlayer();

	void bind_skill(MapRoleSkill *w_skill_study) { skill_study = w_skill_study; }
	void bind_bag(MapItem *w_bag_item, ListEquip *w_bag_equip) { bag_item = w_bag_item; bag_equip = w_bag_equip; }
	void bind_storage(MapItem *w_storage_item, ListEquip *w_storage_equip) { storage_item = w_storage_item; storage_equip = w_storage_equip; }

	int32_t wearEquip(uint32_t index);
	int32_t takeoffEquip(uint32_t location);
	Info_Equip *get_onBodyEquip_point(void) { return onWearEquip; }

	void add_coin(int32_t no) { coin += no; }
	void add_gold(int32_t no) { gold += no; }
	void add_rep(int32_t no) { reputation += no; }

	void sub_coin(int32_t no) { coin -= no; }
	void sub_gold(int32_t no) { gold -= no; }
	void sub_rep(int32_t no) { reputation -= no; }
	
	RoleVoc get_voc(void) { return vocation; }
	uint32_t get_gender(void) { return gender; }
	uint64_t get_coin(void) { return coin; }
	uint64_t get_gold(void) { return gold; }
	uint64_t get_rep(void) { return reputation; }


	MapRoleSkill *get_skill(void) { return skill_study; }
	MapItem *get_bag_item(void) { return bag_item; }
	MapItem *get_storage_item(void) { return storage_item; }
	ListEquip *get_bag_equip(void) { return bag_equip; }
	ListEquip *get_storage_equip(void) { return storage_equip; }

	void set_coin(uint64_t no) { coin = no; }
	void set_gold(uint64_t no) { gold = no; }
	void set_rep(uint64_t no) { reputation = no; }

	void set_equip_secret(info_equip_secret e) { equip_secret = e; }
	void set_equip_add(Info_basic_equip e) { equip_add = e; }

	void levelUp();

	void updateEquipInfo();
	void updateParameter();

private:
	RoleVoc vocation;		//职业
	uint32_t gender;		//性别
	uint64_t coin;			//金币
	uint64_t gold;			//元宝
	uint64_t reputation;	//声望

	MapRoleSkill *skill_study;	//角色已学会的技能
	MapItem *bag_item;			//道具背包
	MapItem *storage_item;		//道具仓库
	ListEquip *bag_equip;		//装备背包
	ListEquip *storage_equip;	//装备仓库

	Info_Equip onWearEquip[MaxEquipCountForRole];

	info_equip_secret equip_secret;
	Info_basic_equip equip_add;
};

