#include "fight_fight.h"
#include <QFile>
#include <QMessageBox>
#include <time.h>
#include <QSettings>

#include "Item_Base.h"
#include "def_System_para.h"
#include "mirrorlog.h"
#include "BasicMath.h"

#include <assert.h>

extern QVector<Info_jobAdd> g_JobAddSet;
extern QMap<skillID, Info_SkillBasic> g_SkillBasic;
extern QMap<qint32, Info_SkillDamage> g_SkillDamage;
extern QMap<qint32, Info_SkillBuff> g_SkillBuff;
extern QMap<qint32, Info_SkillSummon> g_SkillSummon;
extern QMap<qint32, Info_SkillTreat> g_SkillTreat;
extern QMap<itemID, Info_Item> g_ItemList;
extern QMap<itemID, Info_basic_equip> g_EquipList;
extern QMap<mapID, Info_Distribute> g_MonsterDistribute;
extern QMap<monsterID, MonsterInfo> g_MonsterInfo;
extern mapDrop	g_mapDropSet;

double GetMaxExtraValue(EquipExtraType eet, int32_t EquipLevel);
void CreateEquip(bool bBoss, itemID id, Info_Equip &equip);

fight_fight::fight_fight(QWidget* parent, const Info_Distribute &w_dis, CPlayer *const w_player)
	: QDialog(parent), m_MainFrame(parent), dis(w_dis), player(w_player)
{
	ui.setupUi(this);
	InitUI();
	InitSystemConfigure();

	m_bag_item = player->get_bag_item();
	m_bag_equip = player->get_bag_equip();

	DisplayRoleinfo();
	DisplayRoleParameter();
	
	//AssignMonster(g_MonsterInfo, g_MonsterDistribute);

	nRound = 0;
	time_findMonster = 0;
	rt = RT_Rest;

	nTimeOutTime = 2 * 60 * 1000;


	//为装备栏控件安装事件过滤机制，使得QLabel控件可响应clicked()之类的事件。
	OrganismsHead.append(ui.lbl_head_monster1);
	OrganismsHead.append(ui.lbl_head_monster2);
	OrganismsHead.append(ui.lbl_head_monster3);
	OrganismsHead.append(ui.lbl_head_monster4);
	foreach(QLabel *lbl, OrganismsHead)
	{
		lbl->installEventFilter(this);
	}
	dlg_orgInfo = new fight_OrgInfo(this);
	dlg_orgInfo->hide();

	nFightTimer = startTimer(nFightInterval);
	ct_start = QDateTime::currentDateTime();
	nCount_exp = nCount_coin = nCount_items = 0;
	nCount_victory = nCount_totalWar = 0;

	nXSpeedTimer = startTimer(nXSpeedInvterval);
	xSpeedTime.start();
	nXSpeedCount = 0;
}

fight_fight::~fight_fight()
{

}

void fight_fight::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape)
	{
		on_btn_quit_clicked();
	}
}
void fight_fight::on_btn_quit_clicked(void)
{
	close();
}

void fight_fight::InitUI()
{
	monsterCount = dis.monsterCount;
	monster_normal_count = dis.normal.size();
	monster_boss_count = dis.boss.size();

	ui.lbl_statistics_map->setText(dis.name);
	ui.lbl_round_info->setText("");
	ui.lbl_round_time->setText("");

	QString strFont = loadFontFamilyFromTTF_ygy();
	ui.lbl_round_info->setFont(QFont(strFont, 24));

	QLabel *buffs_player[MaxBuffCount] = { ui.lbl_buff_1_role, ui.lbl_buff_2_role, ui.lbl_buff_3_role, ui.lbl_buff_4_role };
	player->bingWidget(ui.lbl_name_role, ui.lbl_level_role, ui.lbl_head_role, buffs_player, MaxBuffCount, ui.progressBar_hp_role, ui.progressBar_mp_role);
	
	QLabel *buffs_pet[MaxBuffCount] = { ui.lbl_buff_1_pet, ui.lbl_buff_2_pet, ui.lbl_buff_3_pet, ui.lbl_buff_4_pet };
	pet.bingWidget(ui.lbl_name_pet, ui.lbl_level_pet, ui.lbl_head_pet, buffs_pet, MaxBuffCount, ui.progressBar_hp_pet, ui.progressBar_mp_pet);
	
	setPetVisible(false);
	ui.lbl_deadflag_pet->setVisible(false);
	
	for (int i = 0; i < Max_MonsterLive; i++)
	{
		monsterRemainderIndex[i] = -1;
	}
	for (int i = 0; i < monsterCount; i++)
	{
		monster[i] = new CMonster;
	}
	setVisible_monster4(false);
	setVisible_monster3(false);
	setVisible_monster2(false);
	ui.lbl_deadflag_monster1->setVisible(false);
	ui.lbl_deadflag_monster2->setVisible(false);
	ui.lbl_deadflag_monster3->setVisible(false);
	ui.lbl_deadflag_monster4->setVisible(false);

	QLabel *buffs_mon[MaxBuffCount] = { nullptr };
	switch (monsterCount)
	{
	case 4:
		buffs_mon[0] = ui.lbl_buff_1_monster4; buffs_mon[1] = ui.lbl_buff_2_monster4;
		buffs_mon[2] = ui.lbl_buff_3_monster4; buffs_mon[3] = ui.lbl_buff_4_monster4;
		monster[3]->bingWidget(ui.lbl_name_monster4, ui.lbl_level_monster4, ui.lbl_head_monster4, buffs_mon, MaxBuffCount, ui.progressBar_hp_monster4, ui.progressBar_mp_monster4);
		setVisible_monster4(true);
		//继续向下执行
	case 3:
		buffs_mon[0] = ui.lbl_buff_1_monster3; buffs_mon[1] = ui.lbl_buff_2_monster3;
		buffs_mon[2] = ui.lbl_buff_3_monster3; buffs_mon[3] = ui.lbl_buff_4_monster3;
		monster[2]->bingWidget(ui.lbl_name_monster3, ui.lbl_level_monster3, ui.lbl_head_monster3, buffs_mon, MaxBuffCount, ui.progressBar_hp_monster3, ui.progressBar_mp_monster3);
		setVisible_monster3(true);
		//继续向下执行
	case 2:
		buffs_mon[0] = ui.lbl_buff_1_monster2; buffs_mon[1] = ui.lbl_buff_2_monster2;
		buffs_mon[2] = ui.lbl_buff_3_monster2; buffs_mon[3] = ui.lbl_buff_4_monster2;
		monster[1]->bingWidget(ui.lbl_name_monster2, ui.lbl_level_monster2, ui.lbl_head_monster2, buffs_mon, MaxBuffCount, ui.progressBar_hp_monster2, ui.progressBar_mp_monster2);
		setVisible_monster2(true);
		//继续向下执行
	default:
		buffs_mon[0] = ui.lbl_buff_1_monster1; buffs_mon[1] = ui.lbl_buff_2_monster1;
		buffs_mon[2] = ui.lbl_buff_3_monster1; buffs_mon[3] = ui.lbl_buff_4_monster1;
		monster[0]->bingWidget(ui.lbl_name_monster1, ui.lbl_level_monster1, ui.lbl_head_monster1, buffs_mon, MaxBuffCount, ui.progressBar_hp_monster1, ui.progressBar_mp_monster1);
		break;
	}
}

void fight_fight::DisplayRoleinfo()
{
	ui.progressBar_hp_role->setMaximum(player->get_hp_max());
	ui.progressBar_hp_role->setValue(player->get_hp_c());
	ui.progressBar_mp_role->setMaximum(player->get_mp_max());
	ui.progressBar_mp_role->setValue(player->get_mp_c());

	ui.lbl_name_role->setText(player->get_name());

	QString VocImg = QString(":/mirror/Resources/ui/f_0_%1.png").arg(player->get_voc() + 1);
	ui.lbl_vocation_role->setPixmap(QPixmap(VocImg));

	qint32 headNo = ((player->get_voc() - 1) * 2 + player->get_gender());
	QString headImg = QString(":/mirror/Resources/head/%1.png").arg(headNo);
	ui.lbl_head_role->setPixmap(QPixmap(headImg));

	MapRoleSkill *m_skill = player->get_skill();
	for (auto iter = m_skill->constBegin(); iter != m_skill->constEnd(); iter++)
	{
		const Info_SkillBasic &sb = g_SkillBasic.value(iter->id);
		if (sb.type == 4 && iter->usdIndex != 0)
		{
			SummonPet(sb.no, iter->level);
			setPetVisible(true);
			break;
		}
	}

	//从整个技能列表中单独提取出挂机技能(除召唤类)，以节约后续调用的效率
	QMap<int, roleSkill> mapTemp;
	for (auto iter = m_skill->constBegin(); iter != m_skill->constEnd(); iter++)
	{
		if (iter->usdIndex != 0)
		{
			mapTemp[iter->usdIndex] = *iter;
		}	
	}
	
	for (auto iterRole = mapTemp.constBegin(); iterRole != mapTemp.constEnd(); iterRole++)
	{
		const Info_SkillBasic &sb = g_SkillBasic.value(iterRole->id);
		if (sb.type != 4)
		{
			fightingSkill.append(skill_fight(sb, iterRole->level));
		}
	}

	if (fightingSkill.size() == 0)
	{
		fightingSkill.append(skill_fight(g_SkillBasic.first(), 1));
	}

	ResetSkillCD();
}

const Info_Item* fight_fight::FindItem(itemID id)
{
	if (g_ItemList.contains(id))
		return &g_ItemList[id];
	else
		return nullptr;
}
const Info_Item* fight_fight::FindItem(const QString &name)
{
	foreach(const Info_Item &item, g_ItemList)
	{
		if (item.name == name)
		{
			return &item;
		}
	}
	return nullptr;
}

void fight_fight::ShowMonsterInfo(const CMonster *mon, QLabel *lbl_name, QLabel *lbl_level, QLabel *lbl_head, QProgressBar *pbHP)
{
	lbl_name->setText(mon->get_name());
	lbl_level->setText(QString::number(mon->get_lv()));
	lbl_head->setPixmap(QPixmap::fromImage(mon->get_head()));

//	pbHP->setMaximum(mon->get_hp_max());
//	pbHP->setValue(mon->get_hp_c());
}

inline QString fight_fight::Generate_ItemComboBox_Text(const QString &name, const QString &type, quint32 value, quint32 count)
{
	QString strSplit = QStringLiteral("%1 %2:%3 剩:%4").arg(name).arg(type).arg(value).arg(count);
	return strSplit;
}
inline QString fight_fight::Generate_Display_LineText(const QString &str1, const QString &skill, const QString &str2, bool bLuck, bool bep, QList<qint32> listDamage)
{
	QString strTmp = QStringLiteral("%1使用<font color=gray>%2</font>，").arg(str1).arg(skill);
	if (bLuck)
		strTmp += QStringLiteral("获得战神祝福, ");

	strTmp += QStringLiteral("对") + str2;
	
	if (bep)
		strTmp += QStringLiteral("造成<font color = red>致命</font>伤害:<font color = magenta>");
	else
		strTmp += QStringLiteral("造成伤害:<font color = magenta>");

	if (listDamage.size() == 0)
	{
		strTmp += "0";
	}
	for (qint32 i = 0; i < listDamage.size(); i++)
	{
		strTmp += QString::number(listDamage.at(i)) + " ";
	}

	strTmp += QStringLiteral("</font>");
	return strTmp;
}

QString fight_fight::Generate_Display_buffInfo(bool bLuck, const QString &targetName, const QString &SkillName, const realBuff &real)
{
	QString strTmp = QStringLiteral("<font color=DarkCyan>你</font>对<font color=DarkCyan>%1</font>使用:<font color=gray>%2</font>")
		.arg(targetName).arg(SkillName);
	if (bLuck) {
		strTmp += QStringLiteral(", 获得幸运女神赐福");
	}
	if (real.DamageEnhance) {
		strTmp += QStringLiteral(", 伤害增强%1%").arg(real.DamageEnhance);
	}
	if (real.DamageSave) {
		strTmp += QStringLiteral(", 伤害减免%1%").arg(real.DamageSave);
	}
	if (real.ac) {
		strTmp += QStringLiteral(", 防御提升%1点").arg(real.ac);
	}
	if (real.mac) {
		strTmp += QStringLiteral(", 魔御提升%1点").arg(real.mac);
	}
	if (real.speed) {
		strTmp += QStringLiteral(", 攻击间隔减少%1").arg(real.speed);
	}
	strTmp += QStringLiteral(", 效果持续<font color=magenta>%1</font>回合。").arg(real.time);

	return strTmp;
}

void fight_fight::Step_role_Skill(void)
{
	bool bUsedSkill = false;
	qint32 nTmp;

	for (qint32 i = 0; i < fightingSkill.size(); i++)
	{
		nTmp = nSkillIndex;
		const skill_fight &skill = fightingSkill.at(nSkillIndex++);
		if (nSkillIndex >= fightingSkill.size())
		{
			nSkillIndex = 0;
		}

		if (skill.cd_c <= 0)
		{
			switch (skill.type)
			{
			case 1: bUsedSkill = MStep_role_Attack(skill); break;
			case 2: bUsedSkill = MStep_role_Buff(skill); break;
			case 3: bUsedSkill = MStep_role_Debuff(skill); break;
			case 4: bUsedSkill = false; break;	//this case can not catch
			case 5: bUsedSkill = MStep_role_Treat(skill); break;
			default: bUsedSkill = true; break;
			}
		}
		if (bUsedSkill)
		{
			fightingSkill[nTmp].cd_c = fightingSkill[nTmp].cd;
			break;
		}
	}
	if (!bUsedSkill)
	{
		ui.display_Fighting->append(QStringLiteral("无可用技能"));
	}
}
bool fight_fight::MStep_role_Treat(const skill_fight &skill)
{
	qint32 targets, rhp;
	double dTmp1, dTmp2;
	bool bUsedSKill = false;
	COrganisms *orgForLeastHp = player;
	
	const Info_SkillTreat st = g_SkillTreat[skill.no];
	targets = (st.targets == -1 ? nPlayerMember : st.targets);			//玩家侧最多只有4名人员。
	QVector<COrganisms *> orgs = { player, &pet };

	while (targets--)
	{
		//找出血量最少的那个玩家同盟成员
		foreach(COrganisms *org, orgs)
		{
			dTmp1 = 1.0 * orgForLeastHp->get_hp_c() / orgForLeastHp->get_hp_max();
			dTmp2 = 1.0 * org->get_hp_c() / org->get_hp_max();
			if (dTmp2 < dTmp1 && !org->wasDead())
			{
				orgForLeastHp = org;
			}
		}

		if (orgForLeastHp->get_hp_c() > orgForLeastHp->get_hp_max() * 0.5)
		{
			//如果血量最少的玩家同盟成员仍然大于50%血量，则放弃使用治疗类技能
			break;
		}

		rhp = orgForLeastHp->get_hp_max() * (st.hpr_basic + st.hpr_add * skill.level) / 100;
		orgForLeastHp->set_hp_c(orgForLeastHp->get_hp_c() + rhp);
		bUsedSKill = true;

		QString strTmp = 
			QStringLiteral("<font color=DarkCyan>你</font>使用:<font color=gray>%1</font>为<font color=magenta>%2</font>恢复HP：<font color=magenta>%3</font>")
			.arg(skill.name).arg(orgForLeastHp->get_name()).arg(rhp);
		ui.display_Fighting->append(strTmp);
	}
	
	return bUsedSKill;
}
bool fight_fight::MStep_role_Buff(const skill_fight &skill)
{
	realBuff real;
	bool bLuck = false;

	if (!Init_realBuff(skill, bLuck, real))
	{
		//has error, can't use the skill
		LogIns.append(LEVEL_ERROR, __FUNCTION__, mirErr_para);
		return false;
	}

	const Info_SkillBuff &sb = g_SkillBuff.value(skill.no);
	int32_t targets = sb.targets;
	if (targets == -1)
	{
		player->appendBuff(real);
		pet.appendBuff(real);

		ui.display_Fighting->append(Generate_Display_buffInfo(bLuck, QStringLiteral("我方全体"), skill.name, real));
	}
	else if (targets == 0)
	{
		player->appendBuff(real);

		ui.display_Fighting->append(Generate_Display_buffInfo(bLuck, QStringLiteral("自身"), skill.name, real));
	}
	else
	{
		//暂时不可能出现此情况
		LogIns.append(LEVEL_ERROR, __FUNCTION__, mirErr_para);
		ui.display_Fighting->append(QStringLiteral("buffs error, ID:%1 No:%2 Targets:%3").arg(skill.id).arg(sb.id).arg(sb.targets));
		return false;
	}

	return true;
}

bool fight_fight::MStep_role_Debuff(const skill_fight &skill)
{
	//先关闭debuff
	ui.display_Fighting->append(QStringLiteral("暂时不支持debuff类技能"));
	return false;
// 	quint32 nTmp, nTmp1;
// 	realBuff real;
// 	bool bLuck = false;
// 
// 	if (monster.HasBuff(skill.no))
// 	{
// 		return false;	//no need to used skill
// 	}
// 
// 	if (!Init_realBuff(skill, bLuck, real))
// 	{
// 		//has error, can't use the skill
// 		LogIns.append(LEVEL_ERROR, __FUNCTION__, mirErr_para);
// 		return false;
// 	}
// 
// 	monster.appendBuff(real);
// 
// 	ui.display_Fighting->append(Generate_Display_buffInfo(bLuck, skill.name, real));
 	return true;
}

bool fight_fight::Init_realBuff(const skill_fight &skill, bool &bLuck, realBuff &real)
{
	if (!g_SkillBuff.contains(skill.no))
	{
		return false;
	}

	qint32 nTmp;

	int32_t flag;
	if (skill.type == 2) {
		flag = 1;
	} else {
		flag = -1;
	}

	real = { 0 };
	const Info_SkillBuff &sb = g_SkillBuff.value(skill.no);
	switch (sb.et)
	{
	case et_DamageEnhance:real.DamageEnhance = flag * sb.basic + skill.level * sb.add; break;
	case et_DamageSave:real.DamageSave = flag * sb.basic + skill.level * sb.add; break;
	case et_ac_fixed:real.ac = flag * (sb.basic + skill.level * sb.add) * player->GetAttack(3, bLuck) / 100; break;
	case et_mac_fixed:real.mac = flag * (sb.basic + skill.level * sb.add) * player->GetAttack(3, bLuck) / 100; break;
	case et_ac_percent:real.ac = flag * (sb.basic + skill.level * sb.add) * player->get_ac() / 100; break;
	case et_mac_percent:real.ac = flag * (sb.basic + skill.level * sb.add) * player->get_mac() / 100; break;
	case et_speed:real.speed = flag * (sb.basic + skill.level * sb.add) * player->get_intervel() / 100; break;
	default:
		break;
	}

	real.id = skill.no;
	real.name = skill.name;
	real.icon = skill.icon;
	real.time = sb.time;

	return true;
}

bool fight_fight::MStep_role_Attack(const skill_fight &skill)
{
	bool bLuck = false, bep = false;
	qint32 damageId = g_SkillBasic.value(skill.id).no;
	qint32 targets = qMin(g_SkillDamage.value(damageId).targets, monsterRemainderCount);

	//随机选择。
	qint32 whichMonster = 1.0 * monsterRemainderCount * qrand() / RAND_MAX;
	for (int i = 0; i < targets; i++)
	{	
		QList<qint32> ListDamage;

		CMonster *mon = monster[monsterRemainderIndex[whichMonster]];
		player->attack(mon, damageId, skill.level, bLuck, &ListDamage);
		ui.display_Fighting->append(Generate_Display_LineText(
			QStringLiteral("<font color=DarkCyan>你</font>"),
			skill.name,
			QStringLiteral("<font color=DarkRed>%1</font>").arg(mon->get_name()),
			bLuck, bep, ListDamage));

		if (mon->wasDead())
		{
			MonsterDead(mon, whichMonster);

			if (monsterRemainderIndex[whichMonster] == -1)
			{
				//说明是最后一个位置的怪物死亡，故需要回到开头。
				whichMonster = 0;
			}
		}
		else
		{
			//减1是因为数组下标从0开始。
			++whichMonster;
			if (whichMonster > monsterRemainderCount - 1)
			{
				whichMonster = 0;
			}
		}
	} 

	return true;
}

void fight_fight::CalcDropItemsAndDisplay()
{
	if (!g_mapDropSet.contains(dis.ID))	{
		return;
	}

	Info_Equip DropEquip;
	qint32 nTmp;
	itemID itemitem;
	QString strTmp;
	QStringList List_Pick, List_Drop;

	bool bBoss = monster[0]->isBoss();
	qint32 howmany = (bBoss ? 2 : 1);
	for (int i = 0; i < howmany; i++)
	{
		const _tagDrop &dropSet = g_mapDropSet.value(dis.ID);
		if (1 < 1.0 * dropSet.chance * qrand() / RAND_MAX)
		{
			continue;		//未达到掉落机率
		}
		
		nTmp = dropSet.list.size() * qrand() / RAND_MAX;
		itemitem = dropSet.list.at(nTmp);
		if (itemitem > g_itemID_start_equip && itemitem <= g_itemID_stop_equip)
		{
			//掉落装备,大于拾取过滤则拾取，否取丢弃。
			CreateEquip(bBoss, itemitem, DropEquip);
			const Info_basic_equip *equip = Item_Base::GetEquipBasicInfo(DropEquip.ID);
			
			if (m_bag_equip->size() < g_bag_maxSize && (pickFilter[DropEquip.extraAmount] == 1))
			{
				List_Pick.append(equip->name);
				m_bag_equip->append(DropEquip);
			}
			else
			{
				List_Drop.append(equip->name);
			}
		}
		else
		{
			//掉落道具
			const Info_Item *item = Item_Base::FindItem_Item(itemitem);
			List_Pick.append(item->name);
			m_bag_item->insert(itemitem, m_bag_item->value(itemitem) + 1);
		}
		++nCount_items;
	}

	if (List_Pick.size() > 0)
	{
		strTmp.clear();
		foreach(const QString &s, List_Pick)
		{
			strTmp += s + ", ";
		}
		ui.display_Fighting->append(QStringLiteral("<font color=white>获得 %1实力又强了几分。</font>").arg(strTmp));
	}
	if (List_Drop.size() > 0)
	{
		strTmp.clear();
		foreach(const QString &s, List_Drop)
		{
			strTmp += s + ", ";
		}
		ui.display_Fighting->append(QStringLiteral("<font color=white>地上散落着 %1无人拾取。</font>").arg(strTmp));
	}
}

void fight_fight::Action_role(void)
{
	player->update_beforeAction();

	Step_role_Skill();

	updateSkillCD();
}
void fight_fight::Action_monster(CMonster *monster)
{	
	monster->update_beforeAction();

	bool bLuck, bAttackPlayer;
	QList<int32_t> ListDamage;
	QString strTmp;

	if (pet.wasDead()) 	{
		bAttackPlayer = true;
	} 	else {
		bAttackPlayer = 0.65 < (1.0 * qrand() / RAND_MAX);
	}
	
	if (bAttackPlayer)
	{
		monster->attack(player, bLuck, &ListDamage);
		ui.display_Fighting->append(Generate_Display_LineText(
			QStringLiteral("<font color = darkRed>%1</font>").arg(monster->get_name()),
			monster->getVirtualSkillName(),
			QStringLiteral("<font color=DarkCyan>%1</font>").arg(player->get_name()),
			bLuck, false, ListDamage));

		if (player->wasDead())
		{
			FightFinish(FR_fail);
		}
	}
	else
	{
		monster->attack(&pet, bLuck, &ListDamage);
		ui.display_Fighting->append(Generate_Display_LineText(
			QStringLiteral("<font color = darkRed>%1</font>").arg(monster->get_name()),
			monster->getVirtualSkillName(),
			QStringLiteral("<font color=DarkCyan>%1</font>").arg(pet.get_name()),
			bLuck, false, ListDamage));

		if (pet.wasDead())
		{
			ui.lbl_deadflag_pet->setVisible(true);
		}
	}
}

void fight_fight::Action_pet(void)
{
	pet.update_beforeAction();

	bool bLuck, bAttackPlayer;
	QList<int32_t> ListDamage;
	QString strTmp;

	//确定要打哪个monster。
	qint32 whichMonster = 1.0 * monsterRemainderCount * qrand() / RAND_MAX;
	CMonster *mon = monster[monsterRemainderIndex[whichMonster]];

	pet.M_attack(mon, bLuck, &ListDamage);
	ui.display_Fighting->append(Generate_Display_LineText(
		QStringLiteral("<font color=DarkCyan>%1</font>").arg(pet.get_name()),
		pet.get_skill().name,
		QStringLiteral("<font color=DarkRed>%1</font>").arg(mon->get_name()),
		bLuck, false, ListDamage));

	if (mon->wasDead())
	{
		MonsterDead(mon, whichMonster);
	}
}

bool fight_fight::GenerateMonster()
{
	qint32 n;
	QString strTmp = "";

	if (EncounterBoss())
	{
		n = qrand() % monster_boss_count;
		monster[0]->ReplaceSoul(g_MonsterInfo.value(dis.boss[n]), true);	
	}
	else
	{
		n = qrand() % monster_normal_count;
		monster[0]->ReplaceSoul(g_MonsterInfo.value(dis.normal[n]), false);
	}
	ui.lbl_deadflag_monster1->setVisible(false);
	ShowMonsterInfo(monster[0], ui.lbl_name_monster1, ui.lbl_level_monster1, ui.lbl_head_monster1, ui.progressBar_hp_monster1);

	switch (monsterCount)
	{
	case 4:
		ui.lbl_deadflag_monster4->setVisible(false);
		n = qrand() % monster_normal_count;
		monster[3]->ReplaceSoul(g_MonsterInfo.value(dis.normal[n]), false);
		ShowMonsterInfo(monster[3], ui.lbl_name_monster4, ui.lbl_level_monster4, ui.lbl_head_monster4, ui.progressBar_hp_monster4);
		//继续向下执行
	case 3:
		ui.lbl_deadflag_monster3->setVisible(false);
		n = qrand() % monster_normal_count;
		monster[2]->ReplaceSoul(g_MonsterInfo.value(dis.normal[n]), false);
		ShowMonsterInfo(monster[2], ui.lbl_name_monster3, ui.lbl_level_monster3, ui.lbl_head_monster3, ui.progressBar_hp_monster3);
		//继续向下执行
	case 2:
		ui.lbl_deadflag_monster2->setVisible(false);
		n = qrand() % monster_normal_count;
		monster[1]->ReplaceSoul(g_MonsterInfo.value(dis.normal[n]), false);
		ShowMonsterInfo(monster[1], ui.lbl_name_monster2, ui.lbl_level_monster2, ui.lbl_head_monster2, ui.progressBar_hp_monster2);
		break;
	default:
		//上面已创建，无需动作。
		break;
	}
	
	for (int i = 0; i < monsterCount; i++)
	{
		monster[i]->reset_live(time_remain);
		monster[i]->ClearBuff();
	}

	monsterRemainderCount = monsterCount;
	for (int i = 0; i < monsterRemainderCount; i++)
	{
		monsterRemainderIndex[i] = i;
	}
	
	return true;
}

bool fight_fight::EncounterBoss()
{
	static qint32 nElapse_pre_boss = 0;
	bool bBoss = false;

	if (monster_boss_count > 0 && nCount_totalWar > 5)
	{
		++nElapse_pre_boss;
		if (nElapse_pre_boss > 100) {
			bBoss = true;
		}
		else {
			bBoss = (1.0 * qrand() / RAND_MAX) > g_fight_boss_probability;
		}
	}

	if (bBoss)
	{
		nElapse_pre_boss = 0;
	}
	return bBoss;
}

void fight_fight::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == nXSpeedTimer)
	{
		//检测是否加速
		if (xSpeedTime.elapsed() - nXSpeedInvterval < 0)
		{
			++nXSpeedCount;
			if (nXSpeedCount > 20)	{
				LogIns.append(LEVEL_ERROR, __FUNCTION__, mirErr_XSpeed);
				exit(0);
			}
		} else	{
			--nXSpeedCount;
			if (nXSpeedCount < 0)	{
				nXSpeedCount = 0;
			}
		}
		xSpeedTime.restart();
	}
	else if (event->timerId() == nFightTimer)
	{
		++nRound;
		switch (rt)
		{
		case RT_FindMonster: round_FindMonster(); break;
		case RT_Fighting: round_Fighting(); break;
		case RT_Rest: round_Rest(); break;
		default:
			//标准情况下不可能跑到default。如出现此情况，则修正为RT_FindMonster。
			rt = RT_FindMonster;
			break;
		}
	}
}


bool fight_fight::eventFilter(QObject *obj, QEvent *ev)
{
	if (ev->type() == QEvent::Enter)
	{
		for (quint32 i = 0; i < monsterCount; i++)
		{
			if (obj == OrganismsHead[i])
			{
				dlg_orgInfo->DisplayMonsterInfo(this->mapFromGlobal(QCursor::pos()), monster[i]->get_id());
				dlg_orgInfo->show();
				return  true;
			}
		}
	}
	else if (ev->type() == QEvent::Leave)
	{
		dlg_orgInfo->hide();
	}

	// pass the event on to the parent class
	return QWidget::eventFilter(obj, ev);
}

void fight_fight::DisplayStatistics()
{
	qint32 nTmp;
	QString strTmp = "";
	qint32 totalMinutes = ct_start.secsTo(QDateTime::currentDateTime()) / 60 + 1;		//防止除0
	qint32 hour = totalMinutes / 60;
	qint32 minute = totalMinutes % 60;

	if (hour > 0) {
		strTmp = QStringLiteral("%1小时").arg(hour);
	}
	strTmp += QStringLiteral("%1分钟").arg(minute);
	ui.lbl_statistics_time->setText(strTmp);

	ui.lbl_statistics_times->setText(QString::number(nCount_totalWar));

	strTmp = QStringLiteral("%1秒/场").arg(totalMinutes * 60 / nCount_totalWar);
	ui.lbl_statistics_warSpend->setText(strTmp);

	strTmp = QStringLiteral("%1%").arg(100 * nCount_victory / nCount_totalWar);
	ui.lbl_statistics_winRate->setText(strTmp);

	strTmp = QStringLiteral("%1/小时").arg(nCount_exp * 60 / totalMinutes);
	ui.lbl_statistics_exp->setText(strTmp);

	strTmp = QStringLiteral("%1/小时").arg(nCount_coin * 60 / totalMinutes);
	ui.lbl_statistics_coin->setText(strTmp);

	strTmp = QStringLiteral("%1/小时").arg(nCount_items * 60 / totalMinutes);
	ui.lbl_statistics_item->setText(strTmp);

	int32_t level = player->get_lv();
	quint64 lvExp = g_JobAddSet[level].exp - player->get_exp();
	QString strTmp2;
	if (lvExp >= 100000)
	{
		strTmp2 = QStringLiteral("%1万").arg(lvExp / 10000);
	}
	else
	{
		strTmp2 = QString::number(lvExp);
	}

	QString strTmp3;
	qint32 GainExpPerMinute = nCount_exp / totalMinutes;
	if (GainExpPerMinute < 1)
	{
		strTmp3 = QStringLiteral("未知时间");
	}
	else
	{
		nTmp = lvExp / GainExpPerMinute;
		strTmp3 = QStringLiteral("%1小时%2分钟").arg(nTmp / 60).arg(nTmp % 60);
	}
	
	strTmp = QStringLiteral("距离%1级还需要%2经验，预计升级还需要%3").arg(level + 1).arg(strTmp2).arg(strTmp3);
	ui.lbl_statistics_info->setText(strTmp);
}

void fight_fight::updateSkillCD()
{
	for (int i = 0; i < fightingSkill.size(); i++)
	{
		--fightingSkill[i].cd_c;
	}
}

void fight_fight::ResetSkillCD()
{
	nSkillIndex = 0;
	for (int i = 0; i < fightingSkill.size(); i++)
	{
		fightingSkill[i].cd_c = 0;
	}
}

void fight_fight::DisplayRoleParameter(void)
{
	ui.lbl_level_role->setText(QStringLiteral("Lv:") + QString::number(player->get_lv()));

	ui.progressBar_exp_role->setMaximum(g_JobAddSet[player->get_lv()].exp);
	ui.progressBar_exp_role->setValue(player->get_exp());
}

void fight_fight::setPetVisible(bool Visible)
{
	ui.lbl_info_pet->setVisible(Visible);
	ui.lbl_name_pet->setVisible(Visible);
	ui.lbl_head_pet->setVisible(Visible);
	ui.progressBar_hp_pet->setVisible(Visible);
	ui.progressBar_mp_pet->setVisible(Visible);
	ui.lbl_level_pet->setVisible(Visible);
	ui.lbl_vocation_pet->setVisible(Visible);

	ui.lbl_buff_1_pet->setVisible(Visible);
	ui.lbl_buff_2_pet->setVisible(Visible);
	ui.lbl_buff_3_pet->setVisible(Visible);
	ui.lbl_buff_4_pet->setVisible(Visible);
}

bool fight_fight::SummonPet(qint32 summonID, int32_t skillLv)
{
	qint32 nDamage = (player->get_sc1() + player->get_sc2()) / 2;
	pet.ReplaceSoul(summonID, skillLv, player->get_lv(), nDamage);

	ui.lbl_name_pet->setText(pet.get_name());
	ui.lbl_head_pet->setPixmap(QPixmap::fromImage(pet.get_head()));
	ui.lbl_level_pet->setText(QStringLiteral("Lv:") + QString::number(pet.get_lv()));

	return true;
}

void fight_fight::MonsterDead(const CMonster *const mon, qint32 whichMonster)
{
	switch (monsterRemainderIndex[whichMonster])
	{
	case 0:ui.lbl_deadflag_monster1->setVisible(true); break;
	case 1:ui.lbl_deadflag_monster2->setVisible(true); break;
	case 2:ui.lbl_deadflag_monster3->setVisible(true); break;
	case 3:ui.lbl_deadflag_monster4->setVisible(true); break;
	default:
		//nothing
		break;
	}

	monsterRemainderIndex[whichMonster] = monsterRemainderIndex[monsterRemainderCount - 1];
	monsterRemainderIndex[monsterRemainderCount - 1] = -1;
	--monsterRemainderCount;

	if (monsterRemainderCount <= 0)
	{
		FightFinish(FR_victory);
	}

}

inline void fight_fight::CalcDropBasicAndDisplay()
{
	QString strTmp;
	double dTmp;
	quint32 nTmp, nDropExp, nDropCoin, nRoleLevel, nDropRep, nDropSoul;
	bool bHaveBoss = false;

	//怪物死掉，角色增加经验及金币。若是BOSS，再增加声望。
	//必须先乘1.0转化为double，否则等级相减运算将提升到uint层次从而得到一个无穷大。
	nDropExp = nDropCoin = nDropRep = nDropSoul = 0;
	for (int i = 0; i < monsterCount; i++)
	{
		if (player->get_lv() < MaxLevel)
		{
			dTmp = atan(0.3 * (1.0 * monster[i]->get_lv() - player->get_lv()));
			nTmp = monster[i]->get_exp() * ((dTmp + 1.58) / 2);
			nDropExp += nTmp;
		}
		nDropCoin += monster[i]->get_exp() * 0.11;
		if (monster[i]->isBoss())
		{
			nDropRep += nTmp * 0.01;
//			nDropSoul += monster[i]->get_lv();

			bHaveBoss = true;
		}
	}

	player->add_exp(nDropExp);
	quint64 lvExp = g_JobAddSet[player->get_lv()].exp;
	if (player->get_exp() > lvExp)
	{
		player->levelUp();
		DisplayRoleParameter();
		ui.display_Fighting->append(QStringLiteral("<font color=white>升级了. </font>"));
	}
	ui.progressBar_exp_role->setValue(player->get_exp());

	player->add_coin(nDropCoin);
	player->add_rep(nDropRep);
//	player->add_soul(nDropSoul);

	//统计信息
	nCount_exp += nDropExp;
	nCount_coin += nDropCoin;

	ui.display_Fighting->append("");
	strTmp = QStringLiteral("你率领小伙伴们击退了<font color=DarkRed>%1</font>率领的小分队").arg(monster[0]->get_name());
	strTmp += QStringLiteral("，获得\t经验: <font color=green>%1</font>, 金币: <font color=#808000>%2</font>").arg(nDropExp).arg(nDropCoin);
	
	if (bHaveBoss)
	{
		strTmp += QStringLiteral("，声望: <font color=DarkMagenta>%1</font>").arg(nDropRep);
//		strTmp += QStringLiteral("，灵魂点: <font color=DarkMagenta>%1</font>").arg(nDropSoul);
	}
	
	ui.display_Fighting->append(strTmp);
}

void fight_fight::round_FindMonster()
{
	QString strTmp;
	if (nRound % 10 == 0)
	{
		ui.lbl_round_time->setText(QString::number((time_findMonster - nRound) / 10 + 1));
		ui.lbl_round_info->setText(QStringLiteral("寻找怪物中..."));
	}

	if (nRound >= time_findMonster)
	{
		//空字符串就看不到了。
		ui.lbl_round_time->setText("");
		ui.lbl_round_info->setText("");

		//生成一个怪物，并显示怪物信息。
		 GenerateMonster();
		strTmp = QStringLiteral("<font color= white>遭遇 %1 率领的怪物小分队</font>").arg(monster[0]->get_name());
		ui.display_Fighting->setText(strTmp);

		nRound = 0;
		rt = RT_Fighting;
	}
}
void fight_fight::round_Fighting()
{
	//回合时间已用完，判定战斗超时。
	if (time_remain > nTimeOutTime)
	{
		FightFinish(FR_draw);	
		return;
	}

	//若回合时间大于角色时间，则角色活动一回合。再判断，若回合时间小于怪物时间，则怪物活动一回合。
	if (!pet.wasDead() && time_remain > pet.get_live())
	{
		Action_pet();
	}
	else if (time_remain > player->get_live())
	{
		Action_role();
	}
	else
	{
		for (int i = 0; i < monsterRemainderCount; i++)
		{
			CMonster *mon = monster[monsterRemainderIndex[i]];
			if (time_remain > mon->get_live())
			{
				Action_monster(mon);
				break;
			}
		}
	}

	//战斗记时
	time_remain += nFightInterval;
}
void fight_fight::round_Rest()
{
	if (nRound % 10 == 0)
	{
		//先计算已失去的血量，然后除以每秒回复血量，即得出剩余休息时间。
		double rhp = 0.1;
		qint32 nTmp = (player->get_hp_max() - player->get_hp_c()) * 100 / player->get_hp_max();
		nTmp = nTmp * rhp + 1;
		ui.lbl_round_time->setText(QString::number(nTmp));
		ui.lbl_round_info->setText(QStringLiteral("休息中..."));

		player->set_hp_c(player->get_hp_c() + player->get_hp_max() * rhp);
		pet.set_hp_c(pet.get_hp_c() + pet.get_hp_max() * rhp);
	}
	
	if (player->get_hp_c() >= player->get_hp_max() &&
		pet.get_hp_c() >= pet.get_hp_max())
	{
		time_remain = 0;

		player->reset_live(time_remain);
		pet.reset_live(time_remain);

		nRound = 0;
		rt = RT_FindMonster;
	}
}

void fight_fight::FightFinish(FightResult fr)
{
	++nCount_totalWar;
	if (fr == FR_victory)
	{
		++nCount_victory;

		CalcDropBasicAndDisplay();
		CalcDropItemsAndDisplay();
	}
	else if (fr == FR_fail)
	{
		ui.display_Fighting->append(QStringLiteral("<font color=red>战斗失败。</font>"));
	}
	else
	{
		ui.display_Fighting->append(QStringLiteral("<font color=white>战斗超时。</font>"));
	}
	ui.lbl_deadflag_pet->setVisible(false);

	time_findMonster = 40 * qrand() / RAND_MAX + 40;
	nRound = 0;

	ResetSkillCD();
	DisplayStatistics();

	rt = RT_Rest;
}

double GetMaxExtraValue(EquipExtraType eet, int32_t EquipLevel)
{
	double Tmp;
	switch (eet)
	{
	case eet_fixed_hp:
	case eet_fixed_mp:
		Tmp = 4 * EquipLevel * EquipLevel;
		break;

	case eet_fixed_hpr:
	case eet_fixed_mpr:
		Tmp = 3 * EquipLevel;
		break;

	case eet_fixed_dc:
	case eet_fixed_mc:
	case eet_fixed_sc:
		Tmp = EquipLevel < 4 ? 4 : (EquipLevel < 7 ? 7 : (1 + (EquipLevel - 7)*0.08) * EquipLevel);
		break;

	case eet_fixed_ac:
	case eet_fixed_mac:
		Tmp = EquipLevel < 4 ? 4 : (EquipLevel < 7 ? 7 : EquipLevel);
		break;

	case eet_fixed_spd:
		Tmp = 10 * EquipLevel;
		break;

	case eet_fixed_luck:
		Tmp = 3;
		break;

	case eet_percent_dc:
	case eet_percent_mc:
	case eet_percent_sc:
	case eet_percent_ac:
	case eet_percent_mac:
		Tmp = EquipLevel < 4 ? 3 : 5;
		break;

	default:
		Tmp = EquipLevel;
		break;
	}
	return Tmp;
}

void CreateEquip(bool bBoss, itemID id, Info_Equip &DropEquip)
{
	double dTmp;
	int nArr[20];
	//装备品质：普通、魔法、英雄、传说。
	int32_t nTmp = 0;

	dTmp = 1.0 * qrand() / RAND_MAX;
	if (dTmp > 0.98) {
		nTmp = 3;
	}
	else if (dTmp > 0.90) {
		nTmp = 2;
	}
	else if (dTmp > 0.75) {
		nTmp = 1;
	}
	else {
		nTmp = 0;
	}

	//初始化
	DropEquip = { 0 };
	DropEquip.ID = id;
	DropEquip.extraAmount = nTmp;

	QVector<EquipExtraType> eet_attack = { eet_fixed_dc, eet_fixed_mc, eet_fixed_sc, eet_percent_dc, eet_percent_mc, eet_percent_sc };
	QVector<EquipExtraType> eet_defense = { eet_fixed_ac, eet_fixed_mac, eet_percent_ac, eet_percent_mac };

	//获取装备的类别
	int Type = (id - g_itemID_start_equip) / 1000;
	switch (Type)
	{
	case g_equipType_weapon:
		eet_attack.append(eet_fixed_spd);
		eet_attack.append(eet_fixed_luck);
		break;

	case g_equipType_necklace:
		eet_attack.append(eet_fixed_luck);
		break;

	case  g_equipType_ring:
		eet_attack.append(eet_fixed_spd);
		break;

	case g_equipType_clothes_m:
	case g_equipType_clothes_f:
	case g_equipType_belt:
		eet_defense.append(eet_fixed_hp);
		eet_defense.append(eet_fixed_hpr);
		break;

	default:
		//nothing
		break;
	}

	if (Type == g_equipType_weapon || Type == g_equipType_necklace || Type == g_equipType_bracelet ||
		Type == g_equipType_ring || Type == g_equipType_medal || Type == g_equipType_gem) {
		GetRandomSequence(nArr, eet_attack.size());
	}
	else {
		GetRandomSequence(nArr, eet_defense.size());
	}

	for (int i = 0; i < DropEquip.extraAmount; i++)
	{
		EquipExtra2 *extra = &DropEquip.extra[i];
		if (Type == g_equipType_weapon || Type == g_equipType_necklace || Type == g_equipType_bracelet ||
			Type == g_equipType_ring || Type == g_equipType_medal || Type == g_equipType_gem) {
			extra->eet = eet_attack[nArr[i]];
		}
		else {
			extra->eet = eet_defense[nArr[i]];
		}

		dTmp = GetMaxExtraValue(extra->eet, g_EquipList.value(id).lv);
		if (dTmp > 10) {
			dTmp = dTmp * 0.4 * qrand() / RAND_MAX + dTmp * 0.6;
		} else {
			dTmp = dTmp * qrand() / RAND_MAX;
		}
		if (dTmp < 0.5) {
			extra->value = 1;
		} else {
			extra->value = dTmp + 0.55;
		}
	}
}


void fight_fight::setVisible_monster2(bool Visible)
{
	ui.lbl_info_monster2->setVisible(Visible);
	ui.lbl_name_monster2->setVisible(Visible);
	ui.lbl_head_monster2->setVisible(Visible);
	ui.progressBar_hp_monster2->setVisible(Visible);
	ui.progressBar_mp_monster2->setVisible(Visible);
	ui.lbl_level_monster2->setVisible(Visible);

	ui.lbl_buff_1_monster2->setVisible(Visible);
	ui.lbl_buff_2_monster2->setVisible(Visible);
	ui.lbl_buff_3_monster2->setVisible(Visible);
	ui.lbl_buff_4_monster2->setVisible(Visible);
}

void fight_fight::setVisible_monster3(bool Visible)
{
	ui.lbl_info_monster3->setVisible(Visible);
	ui.lbl_name_monster3->setVisible(Visible);
	ui.lbl_head_monster3->setVisible(Visible);
	ui.progressBar_hp_monster3->setVisible(Visible);
	ui.progressBar_mp_monster3->setVisible(Visible);
	ui.lbl_level_monster3->setVisible(Visible);

	ui.lbl_buff_1_monster3->setVisible(Visible);
	ui.lbl_buff_2_monster3->setVisible(Visible);
	ui.lbl_buff_3_monster3->setVisible(Visible);
	ui.lbl_buff_4_monster3->setVisible(Visible);
}

void fight_fight::setVisible_monster4(bool Visible)
{
	ui.lbl_info_monster4->setVisible(Visible);
	ui.lbl_name_monster4->setVisible(Visible);
	ui.lbl_head_monster4->setVisible(Visible);
	ui.progressBar_hp_monster4->setVisible(Visible);
	ui.progressBar_mp_monster4->setVisible(Visible);
	ui.lbl_level_monster4->setVisible(Visible);

	ui.lbl_buff_1_monster4->setVisible(Visible);
	ui.lbl_buff_2_monster4->setVisible(Visible);
	ui.lbl_buff_3_monster4->setVisible(Visible);
	ui.lbl_buff_4_monster4->setVisible(Visible);
}


void fight_fight::InitSystemConfigure(void)
{
	qint32 nTmp, i = 0;
	QSettings settings("setting.ini", QSettings::IniFormat);
	nTmp = settings.value("filter/extraAmount", 0x0F).toInt();

	for (auto &i:pickFilter)
	{
		i = 0;
	}

	do 
	{
		pickFilter[i++] = nTmp & 0x01;
		nTmp = nTmp >> 1;
	} while (nTmp);
}