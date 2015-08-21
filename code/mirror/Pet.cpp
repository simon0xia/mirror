#include "Pet.h"
#include "MonsterDefine.h"

extern QVector<MonsterInfo> g_MonsterNormal_List;

CPet::CPet(int32_t lv)
	:COrganisms("pet", lv)
{
	skill.type = 0;
	skill.damage = 100;
	skill.basic = 0;
	skill.times = 1;

	set_hp_c(-1);
}


CPet::~CPet()
{
}

bool CPet::ReplaceSoul(monsterID Summoner, int32_t skillLv, int32_t t)
{
	pt = static_cast<PetType>(t-1);

	for (int i = 0; i < g_MonsterNormal_List.size(); i++)
	{
		if (Summoner == g_MonsterNormal_List[i].ID)
		{
			set_head(g_MonsterNormal_List[i].Head);
			name = g_MonsterNormal_List[i].name;
			break;
		}
	}

	updateParameter();
	return true;
}

void CPet::updateParameter()
{
	int32_t lv = get_lv();
	int32_t hp, dc1, dc2, mc1, mc2, ac1, ac2, mac1, mac2;
	hp = lv * 10;
	dc1 = dc2 = mc1 = mc2 = lv * 0.1;
	ac1 = ac2 = mac1 = mac2 = lv * 0.5;
	switch (pt)
	{
	case pt_dc:
		dc1 = lv * 1.2; dc2 = lv * 1.5;
		break;
	case pt_mc:
		mc1 = lv * 1.25; mc2 = lv * 1.6;
		break;
	case pt_defense:
		ac1 = mac1 = lv * 0.65; ac2 = mac2 = lv * 0.8;
		break;
	case pt_life:
		hp = lv * 100;
		break;
	default:
		break;
	}

	set_hp_m(hp);
	set_mp_m(get_lv() * 1);
	set_dc(dc1, dc2);
	set_mc(mc1, mc2);
	set_sc(0, 0);
	set_ac(ac1, ac2);
	set_mac(mac1, mac2);
	set_intervel(1500);
}