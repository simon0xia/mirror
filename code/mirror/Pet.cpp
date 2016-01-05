#include "Pet.h"
#include "MonsterDefine.h"

extern QMap<monsterID, MonsterInfo> g_MonsterInfo;
extern QMap<qint32, Info_SkillSummon> g_SkillSummon;

CPet::CPet()
	:COrganisms("pet", 1)
{
	skill.id = 220000;
	skill.level = 1;
	skill.name = QStringLiteral("攻击");

	LvExp = 10;
}

CPet::~CPet()
{
}

bool CPet::ReplaceSoul(qint32 summonID, int32_t skillLv, int32_t playerLv, int32_t playerDamage)
{
	standardLv = playerLv + skillLv;
	m_SkillLv = skillLv;
	m_playerDamage = playerDamage;
	set_Lv(standardLv);
	
	ss = g_SkillSummon.value(summonID);
	const MonsterInfo &mi = g_MonsterInfo.value(ss.photo);

	if (ss.type == 1) {
		skill.no = 3;
	} else {
		skill.no = 4;
	}

	set_name(mi.name);
	set_head(mi.Head);
	set_exp(0);
	updateParameter();
	return true;
}

void CPet::LevelUp()
{
	set_Lv(get_lv() + 1);
	set_exp(0);
	updateParameter();

	LvExp = pow((get_lv() - standardLv), 2) * 10;
}

void CPet::updateParameter()
{
	int32_t lv = get_lv();
	int32_t hp, dc1, dc2, sc1, sc2, ac, mac;

	hp = lv * ss.hp;
	dc1 = sc1 = m_playerDamage * ss.damage1 / 100 + lv * m_SkillLv * 0.5;
	dc2 = sc2 = m_playerDamage * ss.damage2 / 100 + lv * m_SkillLv * 0.5;
	ac = mac = m_playerDamage * ss.defense / 100 + lv / 10;

	set_hp_m(hp);
	set_mp_m(lv);		//宠物不放技能，mp设置为多少都无所谓
	set_dc(dc1, dc2);
	set_mc(0, 0);
	set_sc(sc1, sc2);
	set_ac(ac);
	set_mac(mac);
	set_intervel(qMax(500, 2000 - (get_lv() - standardLv) * 100));
}