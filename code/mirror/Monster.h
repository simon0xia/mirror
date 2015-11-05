#pragma once
#include "Organisms.h"
#include "MonsterDefine.h"

class CMonster :
	public COrganisms
{
public:
	CMonster();
	~CMonster();

	const QString &get_name(void) const { return name; }


	bool ReplaceSoul(const MonsterInfo &info, bool boss);
	bool isBoss(void) { return bBoss; }

	const QString &getVirtualSkillName() const { return VirtulSkillName; }

	monsterID get_id(void) { return id; }
	
	void attack(COrganisms *const other, bool &bLuck, QList<qint32> *const ListDamage);

private:
	monsterID id;
	QString name;
	bool bBoss;

	QString VirtulSkillName;
};

