#pragma once
#include "Organisms.h"

enum PetType
{
	pt_undefine,
	pt_dc,
	pt_mc,
	pt_defense,
	pt_life
};

class CPet :
	public COrganisms
{
public:
	CPet();
	~CPet();

	bool ReplaceSoul(monsterID Summoner, int32_t skillLv, int32_t t, int32_t playerLv);

	const QString &get_name(void) { return name; }
	skill_fight *get_skill(void) { return &skill; }

	int32_t get_LvExp(void) { return LvExp; }

	void M_attack(COrganisms *const other, bool &bLuck, QList<qint32> *const ListDamage)
	{
		attack(other, skill, bLuck, ListDamage);
	}

	void LevelUp();

private:
	void updateParameter();

private:
	QString name;
	skill_fight skill;
	PetType pt;

	int32_t m_playerLv, m_SkillLv;
	uint64_t LvExp;	
};

