#include "Monster.h"


CMonster::CMonster()
	:COrganisms("unKnow", 0)
{
	skill.type = 0;
	skill.damage = 100;
	skill.basic = 0;
	skill.times = 1;
	skill.name = QStringLiteral("³èÎï¹¥»÷");
}


CMonster::~CMonster()
{
}

bool CMonster::ReplaceSoul(const MonsterInfo &info, bool boss)
{
	bBoss = boss;
	id = info.ID;
	name = info.name;
	set_head(info.Head);
	set_Lv(info.level);
	set_exp(info.exp);
	set_hp_m(info.hp);
	set_mp_m(info.mp);
	set_dc(info.DC1, info.DC2);
	set_mc(info.MC1, info.MC2);
	set_sc(0, 0);
	set_ac(info.AC, info.AC);
	set_mac(info.MAC, info.MAC);
	set_intervel(info.interval);

	QString str[10] = { QStringLiteral("ÆË»÷"), QStringLiteral("³å×²"), QStringLiteral("¿ÖÏÅ"), QStringLiteral("·É»÷"), QStringLiteral("Ó°»÷"),
		QStringLiteral("¶¾Êõ"), QStringLiteral("´ÎÉù²¨"), QStringLiteral("¼«ËÙ"), QStringLiteral("¾ÞÁ¦"), QStringLiteral("¼ùÌ¤") };
	skill.name = str[qrand() % 10];
	return true;
}
