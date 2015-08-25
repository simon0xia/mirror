#include "Pet.h"
#include "MonsterDefine.h"

extern QVector<MonsterInfo> g_MonsterNormal_List;

CPet::CPet()
	:COrganisms("pet", 1)
{
	skill.type = 1;
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

	pt = t;

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

	hp = lv * (100 + 20 * m_SkillLv);
	dc1 = dc2 = mc1 = mc2 = lv * 0.1;
	ac1 = ac2 = mac1 = mac2 = lv * 0.2;
	switch (pt)
	{
	case 1:
		hp = lv * (15 + 2 * m_SkillLv);
		dc1 = lv * (1.0 + m_SkillLv / 10.0) + 25;
		dc2 = lv * (1.2 + m_SkillLv / 10.0) + 30;
		ac1 = mac1 = lv * (0.10 + m_SkillLv / 15.0);
		ac2 = mac2 = lv * (0.25 + m_SkillLv / 15.0);
		skill.type = 1;
		name = QStringLiteral("÷¼÷Ã");
		break;
	case 2:
		hp = lv * (20 + 5 * m_SkillLv);
		mc1 = lv * (1.1 + m_SkillLv / 8.0) + 50;
		mc2 = lv * (1.3 + m_SkillLv / 8.0) + 75;
		ac1 = mac1 = lv * (0.15 + m_SkillLv / 12.0);
		ac2 = mac2 = lv * (0.30 + m_SkillLv / 12.0);
		skill.type = 2;
		name = QStringLiteral("ÉñÊÞ");
		break;
	case 3:
		hp = lv * (30 + 3 * m_SkillLv);
		mc1 = lv * (1.3 + m_SkillLv / 8.0) + 75;
		mc2 = lv * (1.5 + m_SkillLv / 7.0) + 100;
		ac1 = mac1 = lv * (0.10 + m_SkillLv / 12.0);
		ac2 = mac2 = lv * (0.25 + m_SkillLv / 12.0);
		skill.type = 2;
		name = QStringLiteral("Èö·Ç");
		break;
	case 4:
		hp = lv * (40 + 10 * m_SkillLv);
		dc1 = lv * (1.3 + m_SkillLv / 8.0) + 75;
		dc2 = lv * (1.5 + m_SkillLv / 7.0) + 100;
		ac1 = mac1 = lv * (0.15 + m_SkillLv / 10.0);
		ac2 = mac2 = lv * (0.30 + m_SkillLv / 10.0);
		skill.type = 1;
		name = QStringLiteral("°×»¢");
		break;
	default:
		//nothing
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