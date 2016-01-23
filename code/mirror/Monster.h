#pragma once
#include "Organisms.h"
#include "MonsterDefine.h"

class CMonster :
	public COrganisms
{
public:
	CMonster();
	~CMonster();

	bool ReplaceSoul(const MonsterInfo &info, bool boss);
	bool isBoss(void) const { return bBoss; }

	monsterID get_id(void) const { return id; }
	
private:
	monsterID id;
	bool bBoss;
};

