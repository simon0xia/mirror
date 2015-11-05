#include "Monster.h"


CMonster::CMonster()
	:COrganisms("unKnow", 0)
{

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
	set_ac(info.AC);
	set_mac(info.MAC);
	set_intervel(info.interval);

	set_rhp(info.hp >> 7);

	QString str[10] = { QStringLiteral("ÆË»÷"), QStringLiteral("³å×²"), QStringLiteral("¿ÖÏÅ"), QStringLiteral("·É»÷"), QStringLiteral("Ó°»÷"),
		QStringLiteral("¶¾Êõ"), QStringLiteral("´ÎÉù²¨"), QStringLiteral("¼«ËÙ"), QStringLiteral("¾ŞÁ¦"), QStringLiteral("¼ùÌ¤") };
	VirtulSkillName = str[qrand() % 10];
	return true;
}

void CMonster::attack(COrganisms *const other, bool &bLuck, QList<qint32> *const ListDamage)
{
	qint32 nDamage1, nDamage2, nDamage, nTmp;
	double dTmp;

	//»ìãç¹¥»÷¡£
	nTmp = GetAttack(1, bLuck);
	nDamage1 = (nTmp - other->get_ac());
	dTmp = nDamage1 > 0 ? pow(1.0 * nDamage1 / nTmp + 0.25, 4) : 0;
	nDamage1 = nTmp * dTmp;

	nTmp = GetAttack(2, bLuck);
	nDamage2 = (nTmp - other->get_mac());
	dTmp = nDamage2 > 0 ? pow(1.0 * nDamage2 / nTmp + 0.25, 4) : 0;
	nDamage2 = nTmp * dTmp;

	nDamage1 = (nDamage1 > 0) ? nDamage1 : 0;
	nDamage2 = (nDamage2 > 0) ? nDamage2 : 0;

	nDamage = nDamage1 + nDamage2;
	nDamage = (nDamage > 1) ? nDamage : 1;
	ListDamage->append(nDamage);

	other->set_hp_c(other->get_hp_c() - nDamage);
}