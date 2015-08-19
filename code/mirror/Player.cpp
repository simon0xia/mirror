#include "Player.h"
#include "Item_Base.h"

extern QVector<Info_jobAdd> g_JobAddSet;

CPlayer::CPlayer(const char *w_name, RoleVoc w_voc, int32_t w_level, uint32_t w_gender, uint64_t w_coin, uint64_t w_gold, uint64_t w_rep, uint64_t w_exp)
	:COrganisms(w_name, w_level)
	, vocation(w_voc), gender(w_gender), coin(w_coin), gold(w_gold), reputation(w_rep), exp(w_exp)
{

}


CPlayer::~CPlayer()
{
}


int32_t CPlayer::wearEquip(uint32_t index)
{
	const Info_Equip &equip_new = get_bag_equip()->at(index);

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
		bag_equip->append(onWearEquip[locationA]);
	}
	//将背包装备从背包中取出来（删除）,然后佩带在当前部位。
	onWearEquip[locationA] = equip_new;
	bag_equip->removeAt(index);

	updateEquipInfo();
	updateParameter();
	return locationA;
}

int32_t CPlayer::takeoffEquip(uint32_t location)
{
	bag_equip->append(onWearEquip[location]);
	onWearEquip[location] = { 0 };

	updateEquipInfo();
	updateParameter();
	return location;
}

void CPlayer::updateEquipInfo()
{
	quint32 nTmp, nSBV, nSGV;
	equip_add = { 0 };
	equip_secret = { 0 };

	for (int32_t i = 0; i < MaxEquipCountForRole; i++)
	{
		const Info_basic_equip *equip = Item_Base::GetEquipBasicInfo(onWearEquip[i].ID);
		if (equip == nullptr)
		{
			continue;
		}
		const EquipExtra *extra = &onWearEquip[i].extra;

		equip_add.luck += equip->luck + extra->luck;
		equip_add.acc += equip->acc + extra->acc;
		equip_add.sacred += equip->sacred;
		equip_add.ep += equip->ep;
		equip_add.ed += equip->ed;
		equip_add.ac1 += equip->ac1;
		equip_add.ac2 += equip->ac2 + extra->ac;
		equip_add.mac1 += equip->mac1;
		equip_add.mac2 += equip->mac2 + extra->mac;
		equip_add.dc1 += equip->dc1;
		equip_add.dc2 += equip->dc2 + extra->dc;
		equip_add.mc1 += equip->mc1;
		equip_add.mc2 += equip->mc2 + extra->mc;
		equip_add.sc1 += equip->sc1;
		equip_add.sc2 += equip->sc2 + extra->sc;

		nSBV = equip->sbv;
		nSGV = equip->sgv;
		switch (equip->st)
		{
		case st_hp: equip_secret.hp += nSBV; equip_secret.ghp += nSGV;  break;
		case st_hpr: equip_secret.hpr += nSBV; equip_secret.ghpr += nSGV;  break;
		case st_hpd: equip_secret.hpd += nSBV; equip_secret.ghpd += nSGV;  break;
		case st_mp: equip_secret.mp += nSBV; equip_secret.gmp += nSGV;  break;
		case st_mpr: equip_secret.mpr += nSBV; equip_secret.gmpr += nSGV;  break;
		case st_mpd: equip_secret.mpd += nSBV; equip_secret.gmpd += nSGV;  break;
		case st_acc: equip_secret.acc += nSBV;  break;
		case st_macc: equip_secret.macc += nSBV;  break;
		case st_luck: equip_secret.luck += nSBV;  break;
		case st_speed: equip_secret.speed += nSBV;  break;
		default:
			break;
		}
	}
}

void CPlayer::updateParameter()
{
	int32_t nTmp, nTmp1, nTmp2;

	nTmp = qMax(1000, 1500 - equip_secret.speed);
	set_intervel(nTmp);

	const Info_jobAdd &jobAdd = g_JobAddSet[get_lv() - 1];

	nTmp1 = jobAdd.dc1 + equip_add.dc1;
	nTmp2 = jobAdd.dc2 + equip_add.dc2;
	set_dc(nTmp1, nTmp2);

	nTmp1 = jobAdd.mc1 + equip_add.mc1;
	nTmp2 = jobAdd.mc2 + equip_add.mc2;
	set_mc(nTmp1, nTmp2);

	nTmp1 = jobAdd.sc1 + equip_add.sc1;
	nTmp2 = jobAdd.sc2 + equip_add.sc2;
	set_sc(nTmp1, nTmp2);

	nTmp1 = jobAdd.ac1 + equip_add.ac1;
	nTmp2 = jobAdd.ac2 + equip_add.ac2;
	set_ac(nTmp1, nTmp2);

	nTmp1 = jobAdd.mac1 + equip_add.mac1;
	nTmp2 = jobAdd.mac2 + equip_add.mac2;
	set_mac(nTmp1, nTmp2);

	nTmp = equip_add.luck + equip_secret.luck;
	set_luck(nTmp);

	nTmp = equip_add.acc;
	set_acc(nTmp);

	nTmp = equip_add.sacred;
	set_sacred(nTmp);

	nTmp = 0;
	set_agi(nTmp);

	nTmp = equip_secret.acc;
	set_Ignore_ac(nTmp);

	nTmp = equip_secret.macc;
	set_Ignore_mac(nTmp);

	nTmp1 = jobAdd.hp + equip_secret.hp + get_lv() * equip_secret.ghp / 100;
	set_hp_m(nTmp1);

	nTmp1 = jobAdd.mp + equip_secret.mp + get_lv() * equip_secret.gmp / 100;
	set_mp_m(nTmp1);
}

void CPlayer::levelUp(void)
{ 
	exp = 0; 
	set_Lv(get_lv() + 1); 
	updateParameter();
}