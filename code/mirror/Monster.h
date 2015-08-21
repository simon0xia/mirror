#pragma once
#include "Organisms.h"
#include "MonsterDefine.h"

class CMonster :
	public COrganisms
{
public:
	CMonster();
	~CMonster();

	const QString &get_name(void) { return name; }


	bool ReplaceSoul(const MonsterInfo &info, bool boss);
	bool isBoss(void) { return bBoss; }

	monsterID get_id(void) { return id; }
	
	int32_t get_exp(void) { return exp; }

	skill_fight *get_skill(void) { return &skill; }

	void M_attack(COrganisms *const other, bool &bLuck, QList<qint32> *const ListDamage)
	{
		attack(other, skill, bLuck, ListDamage);
	}

private:
	monsterID id;
	QString name;
	int32_t exp;
	bool bBoss;

	skill_fight skill;
};

