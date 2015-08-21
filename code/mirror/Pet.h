#pragma once
#include "Organisms.h"

enum PetType
{
	pt_dc,
	pt_mc,
	pt_defense,
	pt_life
};

class CPet :
	public COrganisms
{
public:
	CPet(int32_t lv);
	~CPet();

	bool ReplaceSoul(monsterID Summoner, int32_t skillLv, int32_t t);

	const QString &get_name(void) { return name; }
	skill_fight *get_skill(void) { return &skill; }

	bool isLive(void) { return get_hp_c() > 0; }
	bool wasDead(void) { return get_hp_c() <= 0; }

private:
	void updateParameter();

private:
	QString name;
	skill_fight skill;
	PetType pt;
	
};

