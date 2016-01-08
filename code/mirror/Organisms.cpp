#include "Organisms.h"


#include <stdlib.h>
#include <string.h>

extern QMap<skillID, Info_SkillBasic> g_SkillBasic;
extern QMap<qint32, Info_SkillDamage> g_SkillDamage;
extern QMap<qint32, Info_SkillBuff> g_SkillBuff;

COrganisms::COrganisms(const char *w_name, int32_t w_level)
	:lv(w_level)
{
	name = w_name;

	live = 0;
	intervel = 2000;
	luck = 0;
	m_hp = m_mp = c_hp = c_mp = rhp = rmp = 0;
	ac = mac = dc1 = dc2 = mc1 = mc2 = sc1 = sc2 = 0;

	buff_hp = buff_mp = 0;
	buff_damageEchance = buff_DamageSave = 0;
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
	dTmp = 10.0 * qrand() / RAND_MAX;
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

void COrganisms::attack(COrganisms *const other, qint32 damageId, qint32 skillLv, bool &bLuck, QList<qint32> *const ListDamage)
{
	qint32 nDamage, nTmp;
	double dTmp;

	const Info_SkillDamage &sd = g_SkillDamage.value(damageId);

	for (qint32 i = 0; i < sd.times; i++)
	{
		nTmp = GetAttack(sd.type, bLuck) * (sd.basic + sd.add * skillLv) / 100 + sd.extra;
		if (sd.type == 1)
		{
			//物理攻击。			
			nDamage = (nTmp - other->get_ac());
		}
		else if (sd.type == 2 || sd.type == 3)
		{	//魔法攻击、精神攻击
			nDamage = (nTmp - other->get_mac());
		}
		nDamage = nDamage * (100 - other->get_DamageSave() + get_DamageEchance()) / 100;
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
//	set_mp_c(get_mp_c() + get_rmp());
}

void COrganisms::updateBuffInfo(void)
{
	qint32 i, DamageEchance, DamageSave, ac, mac, speed;

	DamageEchance = DamageSave = ac = mac = speed = 0;

	for (auto iter = buff.begin(); iter != buff.end();)
	{
		--iter->time;
		if (iter->time <= 0)
		{
			iter = buff.erase(iter);
		}
		else
		{
			DamageEchance += iter->DamageEnhance;
			DamageSave += iter->DamageSave;
			ac += iter->ac;
			mac += iter->mac;
			speed += iter->speed;

			iter++;
		}
	}

	buff_damageEchance = DamageEchance;
	buff_DamageSave = DamageSave;
	buff_ac = ac;
	buff_mac = mac;
	buff_spd = speed;

	ShowBuffStatus();
}

void COrganisms::appendBuff(const realBuff &readbuff)
{
	//如果已存在，则删除buff再重新添加。
	for (auto iter = buff.begin(); iter != buff.end(); iter++)
	{
		if (iter->id == readbuff.id)
		{
			buff.erase(iter);
			break;
		}
	}

	buff.append(readbuff);

	ShowBuffStatus();

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