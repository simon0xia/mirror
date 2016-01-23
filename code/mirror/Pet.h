#pragma once
#include "Organisms.h"

class CPet :
	public COrganisms
{
public:
	CPet();
	~CPet();

	bool ReplaceSoul(const QString &edtName, qint32 summonID, int32_t skillLv, int32_t playerLv, int32_t playerDamage);

	int32_t get_LvExp(void) { return LvExp; }

	void LevelUp();

private:
	void updateParameter();

private:
	Info_SkillSummon ss;

	int32_t standardLv, m_SkillLv, m_playerDamage;
	int32_t LvExp;	
};

