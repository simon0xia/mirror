#include "Human.h"
#include "Item_Base.h"
#include "Player.h"

extern QVector<QVector<Info_jobAdd>> g_JobAddSet;

CHuman::CHuman()
	:COrganisms("", 1)
{
	set_camps(1);
	lvExp = INT32_MAX;
}

CHuman::~CHuman()
{
}


int32_t CHuman::wearEquip(uint32_t index)
{
	ListEquip &bag_equip = PlayerIns.get_bag_equip();
	const Info_Equip &equip_new = bag_equip.at(index);

	//获取待佩带装备的类别
	int Type = (equip_new.ID - g_itemID_start_equip) / 1000;

	//根据类别映射到穿戴部位
	qint32 locationA, locationB;
	locationA = locationB = -1;
	switch (Type)
	{
	case 1: locationA = 0; break;
	case 2: locationA = 1; break;
	case 3: locationA = 1; break;
	case 4: locationA = 2; break;
	case 5: locationA = 3; break;
	case 6: locationA = 4; locationB = 5; break;
	case 7: locationA = 6; locationB = 7; break;
	case 8: locationA = 8; break;
	case 9: locationA = 9; break;
	case 10: locationA = 10; break;
	case 11: locationA = 11; break;
	case 12: locationA = 12; break;
	case 13: locationA = 13; break;
	default:
		break;
	}

	//此装备可选装备左手/右手
	if (locationB != -1)
	{	//若左手有装备，右手为空，则装备在右手。否则装备在左手
		if (onWearEquip[locationA].ID != 0 && onWearEquip[locationB].ID == 0)
		{
			locationA = locationB;
		}
	}

	//若当前部位有装备，将装备放入背包。
	if (onWearEquip[locationA].ID != 0)
	{
		bag_equip.append(onWearEquip[locationA]);
	}
	//将背包装备从背包中取出来（删除）,然后佩带在当前部位。
	onWearEquip[locationA] = equip_new;
	bag_equip.removeAt(index);

	updateEquipInfo();
	updateParameter();
	return locationA;
}

int32_t CHuman::takeoffEquip(uint32_t location)
{
	ListEquip &bag_equip = PlayerIns.get_bag_equip();
	bag_equip.append(onWearEquip[location]);
	onWearEquip[location] = { 0 };

	updateEquipInfo();
	updateParameter();
	return location;
}

void CHuman::updateEquipInfo()
{
	equip_basic = { 0 };
	fixed_hp = fixed_mp = fixed_hpr = fixed_mpr = fixed_dc = fixed_mc = fixed_sc = fixed_ac = fixed_mac = fixed_spd = fixed_luck = 0;
	percent_hp = percent_mp = percent_hpr = percent_mpr = percent_dc = percent_mc = percent_sc = percent_ac = percent_mac = 0;

	for (qint32 i = 0; i < MaxEquipCountForRole; i++)
	{
		if (!HasEquip(i))
		{
			continue;
		}
		const Info_basic_equip *equip = GetEquipBasicInfo(onWearEquip[i].ID);
		equip_basic.luck += equip->luck;
		equip_basic.spd += equip->spd;
		equip_basic.hp += equip->hp;
		equip_basic.ac += equip->ac;
		equip_basic.mac += equip->mac;
		equip_basic.dc1 += equip->dc1;
		equip_basic.dc2 += equip->dc2;
		equip_basic.mc1 += equip->mc1;
		equip_basic.mc2 += equip->mc2;
		equip_basic.sc1 += equip->sc1;
		equip_basic.sc2 += equip->sc2;

		for (qint32 j = 0; j < onWearEquip[i].extraAmount; j++)
		{
			const EquipExtra2 *extra = &onWearEquip[i].extra[j];

			switch (extra->eet)
			{
				//固定值
			case eet_fixed_hp: fixed_hp += extra->value; break;
			case eet_fixed_mp: fixed_mp += extra->value; break;
			case eet_fixed_hpr: fixed_hpr += extra->value; break;
			case eet_fixed_mpr: fixed_mpr += extra->value; break;
			case eet_fixed_dc: fixed_dc += extra->value; break;
			case eet_fixed_mc: fixed_mc += extra->value; break;
			case eet_fixed_sc: fixed_sc += extra->value; break;
			case eet_fixed_ac: fixed_ac += extra->value; break;
			case eet_fixed_mac: fixed_mac += extra->value; break;
			case eet_fixed_spd: fixed_spd += extra->value; break;
			case eet_fixed_luck: fixed_luck += extra->value; break;
				//百分比
			case eet_percent_dc: percent_dc += extra->value; break;
			case eet_percent_mc: percent_mc += extra->value; break;
			case eet_percent_sc: percent_sc += extra->value; break;
			case eet_percent_ac: percent_ac += extra->value; break;
			case eet_percent_mac: percent_mac += extra->value; break;
			default: 
				//nothing
				break;
			}
		}
	}
}

void CHuman::updateParameter()
{
	int32_t nTmp, nTmp1, nTmp2, nTmp3;

	nTmp = qMax((qint32)1000, 2000 - equip_basic.spd - fixed_spd);
	set_intervel(nTmp);

	const Info_jobAdd &jobAdd = g_JobAddSet[get_voc() - 1].value(get_lv() - 1);
	lvExp = jobAdd.exp;

	nTmp1 = jobAdd.dc1 + equip_basic.dc1;
	nTmp2 = jobAdd.dc2 + equip_basic.dc2;
	nTmp3 = nTmp2 + fixed_dc + nTmp2 * percent_dc / 100;
	set_dc(nTmp1, nTmp3);

	nTmp1 = jobAdd.mc1 + equip_basic.mc1;
	nTmp2 = jobAdd.mc2 + equip_basic.mc2;
	nTmp3 = nTmp2 + fixed_mc + nTmp2 * percent_mc / 100;
	set_mc(nTmp1, nTmp3);

	nTmp1 = jobAdd.sc1 + equip_basic.sc1;
	nTmp2 = jobAdd.sc2 + equip_basic.sc2;
	nTmp3 = nTmp2 + fixed_sc + nTmp2 * percent_sc / 100;
	set_sc(nTmp1, nTmp3);

	nTmp1 = jobAdd.ac + equip_basic.ac;
	nTmp2 = nTmp1 + fixed_ac + nTmp1 * percent_ac / 100;
	set_ac(nTmp2);

	nTmp1 = jobAdd.mac + equip_basic.mac;
	nTmp2 = nTmp1 + fixed_mac + nTmp1 * percent_mac / 100;
	set_mac(nTmp2);

	nTmp = equip_basic.luck + fixed_luck;
	set_luck(nTmp);

	nTmp1 = jobAdd.hp + equip_basic.hp;
	nTmp2 = nTmp1 + fixed_hp + nTmp1 * percent_hp / 100;
	set_hp_m(nTmp2);

	nTmp1 = jobAdd.mp;
	nTmp2 = nTmp1 + fixed_mp + nTmp1 * percent_mp / 100;
	set_mp_m(nTmp2);

	nTmp1 = fixed_hpr + get_hp_max() * percent_hpr / 100;
	set_rhp(nTmp1);

	nTmp1 = fixed_mpr + get_mp_max() * percent_mpr / 100;
	set_rmp(nTmp1);
}

