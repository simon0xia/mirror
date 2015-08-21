#include "Pet.h"
#include "MonsterDefine.h"

extern QVector<MonsterInfo> g_MonsterNormal_List;

CPet::CPet()
	:COrganisms("pet", 1)
{
	skill.type = 0;
	skill.damage = 100;
	skill.basic = 0;
	skill.times = 1;
	skill.name = QStringLiteral("¹¥»÷");

	set_hp_c(-1);

	LvExp = 10;
}


CPet::~CPet()
{
}

bool CPet::ReplaceSoul(monsterID Summoner, int32_t skillLv, int32_t t, int32_t playerLv)
{
	set_Lv(playerLv);
	m_playerLv = playerLv;
	m_SkillLv = skillLv;
	skill.type = t;
	pt = static_cast<PetType>(t);

	for (int i = 0; i < g_MonsterNormal_List.size(); i++)
	{
		if (Summoner == g_MonsterNormal_List[i].ID)
		{
			set_head(g_MonsterNormal_List[i].Head);
			name = g_MonsterNormal_List[i].name;
			break;
		}
	}

	set_exp(0);
	updateParameter();
	return true;
}

void CPet::LevelUp()
{
	set_Lv(get_lv() + 1);
	set_exp(0);
	updateParameter();

	LvExp = pow((get_lv() - m_playerLv), 2) * 10;
}

void CPet::updateParameter()
{
	int32_t lv = get_lv();
	int32_t hp, dc1, dc2, mc1, mc2, ac1, ac2, mac1, mac2;
	hp = lv * 10;
	dc1 = dc2 = mc1 = mc2 = lv * 0.1;
	ac1 = ac2 = mac1 = mac2 = lv * 0.65;
	switch (pt)
	{
	case  pt_undefine:
		//nothing;
		break;
	case pt_dc:
		dc1 = lv * (1.0 + m_SkillLv / 10.0); 
		dc2 = lv * (1.2 + m_SkillLv / 10.0);
		ac1 = ac2 = lv * (0.5 + m_SkillLv / 15.0);
		break;
	case pt_mc:
		mc1 = lv * (1.1 + m_SkillLv / 10.0);
		mc2 = lv * (1.3 + m_SkillLv / 10.0);
		mac1 = mac2 = lv * (0.5 + m_SkillLv / 15.0);
		break;
	case pt_defense:
		ac1 = mac1 = lv * (0.7 + m_SkillLv / 12.0);
		ac2 = mac2 = lv * (0.9 + m_SkillLv / 12.0);
		break;
	case pt_life:
		hp = lv * (60 + m_SkillLv * 20);
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