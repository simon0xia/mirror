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

	const QString &getVirtualSkillName() const { return VirtulSkillName; }

	monsterID get_id(void) const { return id; }
	
	void attack(COrganisms *const other, bool &bLuck, QList<qint32> *const ListDamage);

private:
	monsterID id;
	bool bBoss;

	QString VirtulSkillName;
};

