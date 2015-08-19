#include "Organisms.h"


#include <stdlib.h>
#include <string.h>


COrganisms::COrganisms(const char *w_name, int32_t w_level)
	:lv(w_level)
{
	memcpy_s(name, sizeof(name), w_name, 128);

	rhp = rmp = buff_hp = buff_mp = buff_rhp = buff_rmp = 0;
	acc = agi = luck = sacred = Ignore_ac = Ignore_mac = 0;

	buff_ac1 = buff_ac2 = buff_mac1 = buff_mac2 = 0;
	buff_dc1 = buff_dc2 = buff_mc1 = buff_mc2 = buff_sc1 = buff_sc2 = 0;
}

COrganisms::~COrganisms()
{
}

int32_t COrganisms::GetAttack(int32_t type, bool &bLuck)
{
	int32_t nA, Min, Max;
	uint32_t nTmp;
	double dTmp;
	errno_t         err;

	Min = 0;
	Max = 1;
	if (type == 1)
	{
		Min = get_dc1();
		Max = get_dc2();
	}
	else if (type == 2)
	{
		Min = get_mc1();
		Max = get_mc2();
	}
	else if (type == 3)
	{
		Min = get_sc1();
		Max = get_sc2();
	}

	err = rand_s(&nTmp);
	nA = Min + nTmp % (Max - Min + 1);

	//∑¢ª”–“‘À
	err = rand_s(&nTmp);
	dTmp = 20.0 * nTmp / RAND_MAX;
	if (dTmp < luck)
	{
		nA = Max;
		bLuck = true;
	}
	return nA;
}

void COrganisms::attack(COrganisms *const other, const skill_fight &skill, bool &bLuck, QList<qint32> *const ListDamage)
{
	qint32 nDamage1, nDamage2, nDamage, nTmp, ac, mac;
	double dTmp;

	for (qint32 i = 0; i < skill.times; i++)
	{
		if (skill.type == 1)
		{
			//ŒÔ¿Ìπ•ª˜°£
			nTmp = GetAttack(skill.type, bLuck) * skill.damage / 100 + skill.basic;
			ac = qMax(0, other->get_ac2() - Ignore_ac);
			nDamage1 = (nTmp - ac);
			nDamage2 = 0;
		}
		else if (skill.type == 2 || skill.type == 3)
		{	//ƒß∑®π•ª˜°¢æ´…Òπ•ª˜
			nTmp = GetAttack(skill.type, bLuck) * skill.damage / 100 + skill.basic;
			mac = qMax(0, other->get_mac2() - Ignore_mac);
			nDamage1 = (nTmp - mac);
			nDamage2 = 0;
		}
		else
		{
			//ªÏ„Áπ•ª˜°£
			nTmp = GetAttack(1, bLuck) * skill.damage / 100 + skill.basic;
			ac = qMax(0, other->get_ac2() - Ignore_ac);
			nDamage1 = (nTmp - ac);
			dTmp = nDamage1 > 0 ? pow(1.0 * nDamage1 / nTmp + 0.25, 4) : 0;
			nDamage1 = nTmp * dTmp;

			nTmp = GetAttack(2, bLuck) * skill.damage / 100 + skill.basic;
			mac = qMax(0, other->get_mac2() - Ignore_mac);
			nDamage2 = (nTmp - mac);
			dTmp = nDamage2 > 0 ? pow(1.0 * nDamage2 / nTmp + 0.25, 4) : 0;
			nDamage2 = nTmp * dTmp;
		}
		nDamage1 = (nDamage1 > 0) ? nDamage1 : 0;
		nDamage2 = (nDamage2 > 0) ? nDamage2 : 0;

		nDamage = nDamage1 + nDamage2;
		nDamage = (nDamage > 1) ? nDamage : 1;
		ListDamage->append(nDamage);
		
		other->set_hp_c(other->get_hp_c() - nDamage);
	}
}