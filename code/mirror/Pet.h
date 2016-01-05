#pragma once
#include "Organisms.h"

class CPet :
	public COrganisms
{
public:
	CPet();
	~CPet();

	bool ReplaceSoul(qint32 summonID, int32_t skillLv, int32_t playerLv, int32_t playerDamage);

	const skill_fight &get_skill() const { return skill; }

	int32_t get_LvExp(void) { return LvExp; }

	void M_attack(COrganisms *const other, bool &bLuck, QList<qint32> *const ListDamage)
	{
		attack(other, skill.no, skill.level, bLuck, ListDamage);
	}

	void LevelUp();

private:
	void updateParameter();

private:
	skill_fight skill;
	
	Info_SkillSummon ss;

	int32_t standardLv, m_SkillLv, m_playerDamage;
	uint64_t LvExp;	
};

