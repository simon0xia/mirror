#pragma once

#include <QLabel>
#include <QProgressBar>
#include <QList>
#include <QMap>

#include "def_DataType.h"
#include "def_skill.h"

const qint32 MaxBuffCount = 4;


struct realBuff
{
	quint32 id;
	QString name;
	QPixmap icon;
	qint32 time;
	qint32 rhp;
	qint32 damage;
	qint32 defense;
	qint32 speed;
};

class COrganisms
{
public:
	COrganisms(const char *w_name, int32_t w_level);
	~COrganisms();

	void bingWidget(QLabel *name, QLabel *level, QLabel *head, QLabel *buffs[], int32_t buffsCount, QProgressBar *pbHP, QProgressBar *pbMP);

	void add_exp(int32_t no) { exp += no; }
	void sub_exp(int32_t no) { exp -= no; }

	const QString &get_name(void) const { return name; }
	const QImage &get_head(void) const { return Head; }
	int32_t get_live(void) const { return live; }
	int32_t get_lv(void) const { return lv; }
	int32_t get_intervel(void) const { return intervel - buff_spd; }
	int32_t get_luck(void) const { return luck; }

	int32_t get_hp_max(void) const { return m_hp + buff_hp; }
	int32_t get_mp_max(void) const { return m_mp + buff_mp; }
	uint64_t get_exp(void) const { return exp; }

	int32_t get_hp_c(void) const { return c_hp; }
	int32_t get_mp_c(void) const { return c_mp; }
	int32_t get_rhp(void) const { return (buff_rhp >= 0 ? rhp + buff_rhp : buff_rhp); }
	int32_t get_rmp(void) const { return (buff_rmp >= 0 ? rmp + buff_rmp : buff_rmp); }
	int32_t get_dc1(void) const { return dc1 + buff_dc; }
	int32_t get_dc2(void) const { return dc2 + buff_dc; }
	int32_t get_mc1(void) const { return mc1 + buff_mc; }
	int32_t get_mc2(void) const { return mc2 + buff_mc; }
	int32_t get_sc1(void) const { return sc1 + buff_sc; }
	int32_t get_sc2(void) const { return sc2 + buff_sc; }
	int32_t get_ac(void) const { return (ac + buff_ac) > 0 ? (ac + buff_ac) : 0; }
	int32_t get_mac(void) const { return (mac + buff_mac) > 0 ? (mac + buff_mac) : 0; }

	int32_t GetAttack(int32_t type, bool &bLuck);

	void reset_live(int32_t n) { live = n; }

	void set_name(const QString &n) { name = n; }
	void set_head(QImage h) { Head = h; }
	void set_hp_m(int32_t n) { m_hp = n; set_hp_c(n); progressBar_hp->setMaximum(n); }
	void set_mp_m(int32_t n) { m_mp = n; set_mp_c(n); progressBar_mp->setMaximum(n); }
	void set_hp_c(int32_t n) { c_hp = (n < m_hp) ? ((n < 0 ? 0 : n)) : m_hp; progressBar_hp->setValue(c_hp); }
	void set_mp_c(int32_t n) { c_mp = (n < m_mp) ? ((n < 0 ? 0 : n)) : m_mp; progressBar_mp->setValue(c_mp); }
	void set_rhp(int32_t n) { rhp = n; }
	void set_rmp(int32_t n) { rmp = n; }

	void set_exp(uint64_t no) { exp = no; }

	void set_Lv(int32_t n) { lv = n; }
	void set_intervel(int32_t n) { intervel = n; }
	void set_luck(int32_t n) { luck = n; }
	void set_dc(int32_t n1, int32_t n2) { dc1 = n1, dc2 = (n1 > n2) ? n1 : n2; }
	void set_mc(int32_t n1, int32_t n2) { mc1 = n1, mc2 = (n1 > n2) ? n1 : n2; }
	void set_sc(int32_t n1, int32_t n2) { sc1 = n1, sc2 = (n1 > n2) ? n1 : n2; }
	void set_ac(int32_t n1) { ac = n1; }
	void set_mac(int32_t n1) { mac = n1; }

	void set_buff_hp(int32_t n) { buff_hp = n; }
	void set_buff_mp(int32_t n) { buff_mp = n; }
	void set_buff_rhp(int32_t n) { buff_rhp = n; }
	void set_buff_rmp(int32_t n) { buff_rmp = n; }
	void set_buff_dc(int32_t n) { buff_dc = n; }
	void set_buff_mc(int32_t n) { buff_mc = n; }
	void set_buff_sc(int32_t n) { buff_sc = n; }
	void set_buff_ac(int32_t n) { buff_ac = n; }
	void set_buff_mac(int32_t n) { buff_mac = n; }
	void set_buff_spd(int32_t n) { buff_spd = n; }

	bool wasDead(void) { return c_hp <= 0; }

	void attack(COrganisms *const other, qint32 damageId, qint32 skillLv, bool &bLuck, QList<qint32> *const ListDamage);

	void update_beforeAction(void);
	void update_LifeStatus(void);

	bool HasBuff(qint32 buffNo);
	void appendBuff(const realBuff &readbuff);
	void ClearBuff(void);

private:
	void update_live(void) { live += get_intervel();}
	void updateBuffInfo(void);
	void ShowBuffStatus(void);

private:
	QString name;										//名字
	int32_t live;										//存活时间
	QImage Head;										//头像
	int64_t exp;										//当前经验值
	int32_t lv;											//等级

	//动态设定值
	int32_t intervel, luck;								//攻击间隔、幸运			
	int32_t rhp, rmp;									//每回合恢复生命、体力值
	int32_t m_hp, m_mp, c_hp, c_mp;						//生命最大值、体力最大值、当前生命值、当前体力值
	int32_t ac, mac;									//防御，魔法防御
	int32_t dc1, dc2, mc1, mc2, sc1, sc2;				//物理攻击力、魔法攻击力、精神攻击力

	int32_t buff_hp, buff_mp, buff_rhp, buff_rmp;
	int32_t buff_ac, buff_mac;
	int32_t buff_dc, buff_mc, buff_sc;
	int32_t buff_spd;

	QList<realBuff> buff;

	QLabel *lbl_name, *lbl_level, *lbl_head, *lbl_buffs[4];
	QProgressBar *progressBar_hp, *progressBar_mp;
};

