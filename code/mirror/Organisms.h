#pragma once

#include <QLabel>
#include <QProgressBar>
#include <QList>
#include <QMap>

#include "def_DataType.h"
#include "def_skill.h"


const qint32 MaxBuffCount = 4;

enum BufferEffect
{
	be_DamageEnhance = 1,
	be_DamageSave,
	be_hp,
	be_rhp,
	be_ac,
	be_mac,
	be_speed,
};

struct realBuff
{
	quint32 id;
	QString name;
	QPixmap icon;
	qint32 time;
	BufferEffect et;
	qint32 value;
};

class COrganisms
{
public:
	COrganisms(const char *w_name, int32_t w_level);
	~COrganisms();

	void set_camps(int32_t n) { camps = n; }
	int32_t get_camps(void) const { return camps; }

	void bindWidget(QLabel *buffs[], int32_t buffsCount, QProgressBar *pbHP, QProgressBar *pbMP);
	void freeWidget(void);
	void ResetSkillCD(void);

	MapSkillStudy &get_skill_study(void) { return skill_study; }

	int32_t get_skill_fight_size(void) const { return skill_fight.size(); }
	const SkillFight &get_skill_fight_cur(void)  {return skill_fight.at(nFightSkillIndex);}
	void ResetSkillCD(int32_t n) { skill_fight[n].cd_c = skill_fight[n].cd; }
	int32_t MoveToNextFightSkill(void)
	{
		int32_t nTmp = nFightSkillIndex;
		nFightSkillIndex++;
		if (nFightSkillIndex >= skill_fight.size())
		{
			nFightSkillIndex = 0;
		}
		return nTmp;
	}

	const QString &get_name(void) const { return name; }
	const QImage &get_head(void) const { return Head; }
	Vocation get_voc(void) const { return vocation; }
	int32_t get_gender(void) const { return gender; }
	int32_t get_lv(void) const { return lv ^ xorkey; }
	int32_t get_exp(void) const { return exp ^ xorkey; }

	int32_t get_live(void) const { return live; }	
	int32_t get_intervel(void) const { return (intervel^xorkey) - buff_spd; }
	int32_t get_luck(void) const { return (luck^xorkey); }

	int32_t get_hp_max(void) const { return (m_hp^xorkey) + buff_hp; }
	int32_t get_mp_max(void) const { return (m_mp^xorkey) + buff_mp; }
	int32_t get_hp_c(void) const { return (c_hp^xorkey); }
	int32_t get_mp_c(void) const { return (c_mp^xorkey); }
	int32_t get_rhp(void) const { return (rhp^xorkey) + buff_rhp; }
	int32_t get_rmp(void) const { return (rmp^xorkey); }
	int32_t get_dc1(void) const { return (dc1^xorkey) + buff_dc; }
	int32_t get_dc2(void) const { return (dc2^xorkey) + buff_dc; }
	int32_t get_mc1(void) const { return (mc1^xorkey) + buff_mc; }
	int32_t get_mc2(void) const { return (mc2^xorkey) + buff_mc; }
	int32_t get_sc1(void) const { return (sc1^xorkey) + buff_sc; }
	int32_t get_sc2(void) const { return (sc2^xorkey) + buff_sc; }
	int32_t get_ac(void) const {
		qint32 n = (ac^xorkey) + buff_ac;
		return  n > 0 ? n : 0;
	}
	int32_t get_mac(void) const {
		qint32 n = (mac^xorkey) + buff_mac;
		return  n > 0 ? n : 0; 
	}
	int32_t get_DamageEchance(void) const { return buff_damageEchance; }
	int32_t get_DamageSave(void) const { return buff_DamageSave; }

	int32_t GetAttack(int32_t type, bool &bLuck) ;

	void reset_live(int32_t n) { live = n; }
	void InitFightSkill(void);

	void set_BasicInfo(const QString &n, int32_t g, Vocation v) { name = n, gender = g, vocation = v;}
	void set_levelInfo(int32_t l, int32_t e) { lv = l ^ xorkey, exp = e ^ xorkey; }
	void set_head(QImage h) { Head = h; }
	void set_hp_m(int32_t n) 
	{
		m_hp = n^xorkey; set_hp_c(n);
		if (progressBar_hp != nullptr) {
			progressBar_hp->setMaximum(n);
		}
	}
	void set_mp_m(int32_t n) 
	{ 
		m_mp = n^xorkey; set_mp_c(n);
		if (progressBar_mp != nullptr) {
			progressBar_mp->setMaximum(n);
		}
	}
	void set_hp_c(int32_t n) 
	{ 
		int hpm = get_hp_max();
		int nTmp = (n < hpm) ? ((n < 0 ? 0 : n)) : hpm;
		c_hp = nTmp ^ xorkey; 
		if (progressBar_hp != nullptr) {
			progressBar_hp->setValue(nTmp);
		}
	}
	void set_mp_c(int32_t n)
	{ 
		int mpm = get_mp_max();
		int nTmp = (n < mpm) ? ((n < 0 ? 0 : n)) : mpm;
		c_mp = nTmp ^ xorkey; 
		if (progressBar_mp != nullptr) {
			progressBar_mp->setValue(nTmp);
		}
	}
	void set_rhp(int32_t n) { rhp = n ^ xorkey; }
	void set_rmp(int32_t n) { rmp = n ^ xorkey; }

	void set_intervel(int32_t n) { intervel = n^xorkey; }
	void set_luck(int32_t n) { luck = n^xorkey; }
	void set_dc(int32_t n1, int32_t n2) { dc1 = n1^xorkey, dc2 = (n1 > n2) ? dc1 : n2^xorkey; }
	void set_mc(int32_t n1, int32_t n2) { mc1 = n1^xorkey, mc2 = (n1 > n2) ? mc1 : n2^xorkey; }
	void set_sc(int32_t n1, int32_t n2) { sc1 = n1^xorkey, sc2 = (n1 > n2) ? sc1 : n2^xorkey; }
	void set_ac(int32_t n1) { ac = n1^xorkey; }
	void set_mac(int32_t n1) { mac = n1^xorkey; }

	bool wasDead(void) const { return get_hp_c() <= 0; }

	void attack(COrganisms *const other, qint32 damageId, qint32 skillLv, bool &bLuck, QList<qint32> *const ListDamage);

	void update_beforeAction(void);
	void update_LifeStatus(void);

	void appendBuff(const realBuff &readbuff);
	void ClearBuff(void);

private:
	void update_live(void) { live += get_intervel();}
	void updateBuffInfo(void);
	void update_skillCD(void);
	void ShowBuffStatus(void);

private:
	int32_t xorkey; 
	QString name;			//名字
	QImage Head;			//头像
	Vocation vocation;		//职业
	int32_t gender;			//性别
	int32_t lv;				//等级
	int32_t exp;			//当前经验值	
	int32_t live;			//存活时间

	//动态设定值
	int32_t intervel, luck;								//攻击间隔、幸运			
	int32_t rhp, rmp;									//每回合恢复生命、体力值
	int32_t m_hp, m_mp, c_hp, c_mp;						//生命最大值、体力最大值、当前生命值、当前体力值
	int32_t ac, mac;									//防御，魔法防御
	int32_t dc1, dc2, mc1, mc2, sc1, sc2;				//物理攻击力、魔法攻击力、精神攻击力

	int32_t buff_hp, buff_mp, buff_rhp;
	int32_t buff_DamageSave, buff_ac, buff_mac;
	int32_t buff_damageEchance, buff_dc, buff_mc, buff_sc;
	int32_t buff_spd;

	//运行时的临时变量
	QList<realBuff> buff;

	MapSkillStudy skill_study;
	QList<SkillFight> skill_fight;
	int32_t nFightSkillIndex;
	int32_t camps;

	QLabel *lbl_buffs[4];
	QProgressBar *progressBar_hp, *progressBar_mp;
};

