#pragma once

#define _CRT_RAND_S
#include "def_DataType.h"
#include "def_skill.h"

class COrganisms
{
public:
	COrganisms(const char *w_name, int32_t w_level);
	~COrganisms();

	void add_exp(int32_t no) { exp += no; }
	void sub_exp(int32_t no) { exp -= no; }

	const char *get_name(void) { return name; }
	const QImage &get_head(void) { return Head; }
	int32_t get_lv(void) { return lv; }
	int32_t get_intervel(void) { return intervel; }
	int32_t get_acc(void) { return acc; }
	int32_t get_agi(void) { return agi; }
	int32_t get_luck(void) { return luck; }
	int32_t get_sacred(void) { return sacred; }
	int32_t set_Ignore_ac(void) { return Ignore_ac; }
	int32_t set_Ignore_mac(void) { return Ignore_mac; }

	int32_t get_hp_max(void) { return m_hp + buff_hp; }
	int32_t get_mp_max(void) { return m_mp + buff_mp; }
	uint64_t get_exp(void) { return exp; }

	int32_t get_hp_c(void) { return c_hp; }
	int32_t get_mp_c(void) { return c_mp; }
	int32_t get_rhp(void) { return rhp + buff_rhp; }
	int32_t get_rmp(void) { return rmp + buff_rmp; }
	int32_t get_dc1(void) { return dc1 + buff_dc1; }
	int32_t get_dc2(void) { return dc2 + buff_dc2; }
	int32_t get_mc1(void) { return mc1 + buff_mc1; }
	int32_t get_mc2(void) { return mc2 + buff_mc2; }
	int32_t get_sc1(void) { return sc1 + buff_sc1; }
	int32_t get_sc2(void) { return sc2 + buff_sc2; }
	int32_t get_ac1(void) { return ac1 + buff_ac1; }
	int32_t get_ac2(void) { return ac2 + buff_ac2; }
	int32_t get_mac1(void) { return mac1 + buff_mac1; }
	int32_t get_mac2(void) { return mac2 + buff_mac2; }

	int32_t GetAttack(int32_t type, bool &bLuck);

	void set_head(QImage h) { Head = h; }
	void set_hp_m(int32_t n) { m_hp = c_hp = n; }
	void set_mp_m(int32_t n) { m_mp = c_mp = n; }
	void set_hp_c(int32_t n) { c_hp = (n < m_hp) ? ((n < 0 ? 0 : n)) : m_hp; }
	void set_mp_c(int32_t n) { c_mp = (n < m_mp) ? ((n < 0 ? 0 : n)) : m_mp; }
	void set_rhp(int32_t n) { rhp = n; }
	void set_rmp(int32_t n) { rmp = n; }

	void set_exp(uint64_t no) { exp = no; }

	void set_Lv(int32_t n) { lv = n; }
	void set_intervel(int32_t n) { intervel = n; }
	void set_acc(int32_t n) { acc = n; }
	void set_agi(int32_t n) { agi = n; }
	void set_luck(int32_t n) { luck = n; }
	void set_sacred(int32_t n) { sacred = n; }
	void set_Ignore_ac(int32_t n) { Ignore_ac = n; }
	void set_Ignore_mac(int32_t n) { Ignore_mac = n; }
	void set_dc(int32_t n1, int32_t n2) { dc1 = n1, dc2 = (n1 > n2) ? n1 : n2; }
	void set_mc(int32_t n1, int32_t n2) { mc1 = n1, mc2 = (n1 > n2) ? n1 : n2; }
	void set_sc(int32_t n1, int32_t n2) { sc1 = n1, sc2 = (n1 > n2) ? n1 : n2; }
	void set_ac(int32_t n1, int32_t n2) { ac1 = n1, ac2 = (n1 > n2) ? n1 : n2; }
	void set_mac(int32_t n1, int32_t n2) { mac1 = n1, mac2 = (n1 > n2) ? n1 : n2; }

	void set_buff_hp(int32_t n) { buff_hp = n; }
	void set_buff_mp(int32_t n) { buff_mp = n; }
	void set_buff_rhp(int32_t n) { buff_rhp = n; }
	void set_buff_rmp(int32_t n) { buff_rmp = n; }
	void set_buff_dc(int32_t n1, int32_t n2) { buff_dc1 = n1, buff_dc2 = n2; }
	void set_buff_mc(int32_t n1, int32_t n2) { buff_mc1 = n1, buff_mc2 = n2; }
	void set_buff_sc(int32_t n1, int32_t n2) { buff_sc1 = n1, buff_sc2 = n2; }
	void set_buff_ac(int32_t n1, int32_t n2) { buff_ac1 = n1, buff_ac2 = n2; }
	void set_buff_mac(int32_t n1, int32_t n2) { buff_mac1 = n1, buff_mac2 = n2; }

	//bool isLive(void) { return  c_hp > 0; }
	bool wasDead(void) { return c_hp <= 0; }

	void attack(COrganisms *const other, const skill_fight &skill, bool &bLuck, QList<qint32> *const ListDamage);

private:
	char name[128];										//名字
	QImage Head;										//头像
	uint64_t exp;										//当前经验值
	int32_t lv;											//等级
	int32_t intervel;									//攻击间隔
	int32_t acc, agi;									//准确、敏捷
	int32_t luck, sacred;								//幸运、神圣
	int32_t rhp, rmp;									//每回合恢复生命、体力值
	int32_t Ignore_ac, Ignore_mac;						//无视防御、魔御

	//设定值
	int32_t m_hp, m_mp;									//生命、体力
	int32_t c_hp, c_mp;
	int32_t ac1, ac2, mac1, mac2;						//防御，魔法防御
	int32_t dc1, dc2, mc1, mc2, sc1, sc2;				//物理攻击力、魔法攻击力、精神攻击力

	int32_t buff_hp, buff_mp, buff_rhp, buff_rmp;
	int32_t buff_ac1, buff_ac2, buff_mac1, buff_mac2;
	int32_t buff_dc1, buff_dc2, buff_mc1, buff_mc2, buff_sc1, buff_sc2;
};

