#include "Organisms.h"


#include <stdlib.h>
#include <string.h>

extern QMap<skillID, Info_SkillBasic> g_SkillBasic;
extern QMap<qint32, Info_SkillDamage> g_SkillDamage;
extern QMap<qint32, Info_SkillBuff> g_SkillBuff;

COrganisms::COrganisms(const char *w_name, int32_t w_level)
	:lv(w_level)
{
	memcpy_s(name, sizeof(name), w_name, 128);

	live = 0;
	intervel = 2000;
	luck = 0;
	m_hp = m_mp = c_hp = c_mp = rhp = rmp = 0;
	ac = mac = dc1 = dc2 = mc1 = mc2 = sc1 = sc2 = 0;

	buff_hp = buff_mp = buff_rhp = buff_rmp = 0;
	buff_ac = buff_mac = buff_dc = buff_mc = buff_sc = 0;
	buff_spd = 0;
}

COrganisms::~COrganisms()
{
}

void COrganisms::bingWidget(QLabel *name, QLabel *level, QLabel *head, QLabel *buffs[], int32_t buffsCount, QProgressBar *pbHP, QProgressBar *pbMP)
{
	lbl_name = name;
	lbl_level = level;
	lbl_head = head;
	progressBar_hp = pbHP;
	progressBar_mp = pbMP;

	for (int i = 0; i < buffsCount && i < MaxBuffCount; i++)
	{
		lbl_buffs[i] = buffs[i];
	}
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

	nA = Min + 1.0 * qrand() / RAND_MAX * (Max - Min + 1);

	//发挥幸运
	dTmp = 20.0 * qrand() / RAND_MAX;
	if (dTmp < luck)
	{
		nA = Max;
		bLuck = true;
	}
	else
	{
		bLuck = false;
	}
	return nA;
}

bool COrganisms::HasBuff(qint32 buffNo)
{
	bool res = false;

	foreach(const realBuff &real, buff)
	{
		if (real.id == buffNo)
		{
			res = true;
		}
	}

	return res;
}

void COrganisms::attack(COrganisms *const other, qint32 skillId, qint32 skillLv, bool &bLuck, QList<qint32> *const ListDamage)
{
	qint32 nDamage, nTmp;
	double dTmp;

	qint32 damageId = g_SkillBasic.value(skillId).no;
	const Info_SkillDamage &sd = g_SkillDamage.value(damageId);

	for (qint32 i = 0; i < sd.times; i++)
	{
		if (sd.type == 1)
		{
			//物理攻击。
			nTmp = GetAttack(sd.type, bLuck) * (sd.basic + sd.add * skillLv) / 100 + sd.extra;
			nDamage = (nTmp - other->get_ac());
		}
		else if (sd.type == 2 || sd.type == 3)
		{	//魔法攻击、精神攻击
			nTmp = GetAttack(sd.type, bLuck) * (sd.basic + sd.add * skillLv) / 100 + sd.extra;
			nDamage = (nTmp - other->get_mac());
		}
		nDamage = (nDamage > 0) ? nDamage : 0;
		ListDamage->append(nDamage);
		
		other->set_hp_c(other->get_hp_c() - nDamage);
	}
}

void COrganisms::update_beforeAction(void)
{
	update_live();
	update_LifeStatus();
	updateBuffInfo();
}

inline void COrganisms::update_LifeStatus(void)
{
	//回血、回蓝
	set_hp_c(get_hp_c() + get_rhp());
	set_mp_c(get_mp_c() + get_rmp());
}

void COrganisms::updateBuffInfo(void)
{
	qint32 i, b_rhp, b_damage, b_defense, b_speed;

	b_rhp = b_damage = b_defense = b_speed = 0;

	for (auto iter = buff.begin(); iter != buff.end();)
	{
		--iter->time;
		if (iter->time <= 0)
		{
			iter = buff.erase(iter);
		}
		else
		{
			b_rhp += iter->rhp;
			b_damage += iter->damage;
			b_defense += iter->defense;
			b_speed += iter->speed;

			iter++;
		}
	}

	buff_rhp = b_rhp;
	buff_ac = b_defense;
	buff_mac = b_defense;

	buff_dc = b_damage;
	buff_mc = b_damage;
	buff_sc = b_damage;

	buff_spd = b_speed;

	ShowBuffStatus();
}

void COrganisms::appendBuff(const realBuff &readbuff)
{
	if (!HasBuff(readbuff.id))
	{
		buff.append(readbuff);

		ShowBuffStatus();
	}
}

void COrganisms::ShowBuffStatus(void)
{
	int i = 0;
	int32_t nTmp = qMin(buff.size(), MaxBuffCount);

	for (; i < nTmp; i++)
	{
		lbl_buffs[i]->setPixmap(buff[i].icon);
		lbl_buffs[i]->setToolTip(buff[i].name);
	}
	for (; i < MaxBuffCount; i++)
	{
		lbl_buffs[i]->setPixmap(QPixmap(""));
	}
}

void COrganisms::ClearBuff()
{
	buff.clear();
	updateBuffInfo();
}