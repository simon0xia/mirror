#pragma once
#include "Organisms.h"
#include "def_DataType.h"
#include "RoleDefine.h"
#include "ItemDefine.h"

class CHuman :
	public COrganisms
{
public:
	CHuman();
	~CHuman();

	//佩戴装备，并返回佩戴位置。
	int32_t wearEquip(uint32_t index);

	//取下指定位置的装备。
	int32_t takeoffEquip(uint32_t location);

	//
	Info_Equip *get_onBodyEquip_point(void) { return onWearEquip; }
	bool HasEquip(qint32 index) {
		if (index >= 0 && index < MaxEquipCountForRole)
			return onWearEquip[index].ID != 0;
		else
			return false;
	}

	void add_exp(int32_t no)
	{ 
		int32_t n = get_exp() + no;
		if (n > lvExp) {
			set_levelInfo(get_lv()+1, 0);
			updateParameter();
		} else {
			set_levelInfo(get_lv(), n);
		}
	}

	void set_Extra(int32_t w_xiulian, int32_t w_yuanli, int32_t r3, int32_t r4, int32_t r5) 
	{ 
		xiulian = w_xiulian ^ xorkey, yuanli = w_yuanli ^ xorkey, resver3 = r3, resver4 = r4, resver5 = r5;
	}
	void set_resver2(int32_t r6, int32_t r7, int32_t r8, int32_t r9, int32_t r10)
	{
		resver6 = r6, resver7 = r7, resver8 = r8, resver9 = r9, resver10 = r10;
	}

	void add_xiulian(void) { set_xiulian(get_xiulian() + 1); }
	void set_xiulian(int32_t n) { xiulian = n ^ xorkey; }
	int32_t get_xiulian(void) const { return xiulian ^ xorkey; }

	void add_yuanli(int32_t n) { set_yuanli(get_yuanli() + n); }
	void sub_yuanli(int32_t n) { set_yuanli(get_yuanli() - n); }
	void set_yuanli(int32_t n) { yuanli = n ^ xorkey; }
	int32_t get_yuanli(void) const { return yuanli ^ xorkey; }

	void updateEquipInfo();
	void updateParameter();

private:
	Info_Equip onWearEquip[MaxEquipCountForRole];	//身上穿戴的装备
	Info_basic_equip equip_basic;					//统计的装备基础属性加成

	int32_t lvExp;
	int32_t xiulian;
	int32_t yuanli;
	
	qint32 resver3, resver4, resver5;
	qint32 resver6, resver7, resver8, resver9, resver10;

	//装备扩展属性加成
	qint32 fixed_hp, fixed_mp, fixed_hpr, fixed_mpr, fixed_dc, fixed_mc, fixed_sc, fixed_ac, fixed_mac, fixed_spd, fixed_luck, fixed_hit, fixed_dodge;
	qint32 percent_hp, percent_mp, percent_hpr, percent_mpr, percent_dc, percent_mc, percent_sc, percent_ac, percent_mac;
};

