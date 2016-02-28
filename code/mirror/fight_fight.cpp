#include "fight_fight.h"
#include <QFile>
#include <QMessageBox>
#include <time.h>
#include <QSettings>
#include <QKeyEvent>
#include <assert.h>
#include "mirrorlog.h"
#include "BasicMath.h"
#include "Item_Base.h"
#include "dlg_task.h"
#include "def_System_para.h"

#include "gamemanager.h"

const 	int32_t nTimeOutTime = 2 * 60 * 1000;

extern QVector<QVector<Info_jobAdd>> g_JobAddSet;
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

fight_fight::fight_fight(QWidget* parent, const Info_Distribute &w_dis)
	: QDialog(parent), dis(w_dis)
{
	ui.setupUi(this);
	Init();
	InitUI();
	InitSystemConfigure();

	DisplayRoleinfo(edt_role);
	if (bHasEdt)
	{
		DisplayEmbodimentInfo(edt_edt);
	}

	nFightTimer = startTimer(nFightInterval);
	nXSpeedTimer = startTimer(nXSpeedInvterval);
	xSpeedTime.start();
}

fight_fight::~fight_fight()
{
	//收集统计信息
	fis.minutes = ct_start.secsTo(QDateTime::currentDateTime()) / 60 + 1;		//防止除0
	GameMgrIns.RegFIS(fis);
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
	edt_role->freeWidget();
	edt_edt->freeWidget();

	killTimer(nFightTimer);
	killTimer(nXSpeedTimer);
	for (int i = 0; i < monsterCount; i++)
	{
		delete monster[i];
	}

	delete dlg_orgInfo;
	
	close();
}

void fight_fight::InitUI()
{
	ui.lbl_statistics_map->setText(dis.name);
	ui.lbl_round_info->setText("");
	ui.lbl_round_time->setText("");

	QString strFont = loadFontFamilyFromTTF_ygy();
	ui.lbl_round_info->setFont(QFont(strFont, 24));

	edt_role = &PlayerIns.get_edt_role();
	camps_La.append(edt_role);
	QLabel *buffs_role[MaxBuffCount] = { ui.lbl_buff_1_role, ui.lbl_buff_2_role, ui.lbl_buff_3_role, ui.lbl_buff_4_role };
	edt_role->bindWidget(buffs_role, MaxBuffCount, ui.progressBar_hp_role, ui.progressBar_mp_role);

	edt_edt = &PlayerIns.get_edt_Fight();
	if (PlayerIns.get_edt_Fight_index() > 0) {
		bHasEdt = true;
		camps_La.append(edt_edt);
		QLabel *buffs_edt[MaxBuffCount] = { ui.lbl_buff_1_edt, ui.lbl_buff_2_edt, ui.lbl_buff_3_edt, ui.lbl_buff_4_edt };
		edt_edt->bindWidget(buffs_edt, MaxBuffCount, ui.progressBar_hp_edt, ui.progressBar_mp_edt);
	}else{
		bHasEdt = false;
		setEdtVisible(false);
	}
	
	//因为pet是fight_fight类成员变量，默认为死亡状态，故可以直接加入La阵营，并绑定相关控件。而edt_Fight的指向不可预期，需先判断。
	camps_La.append(&pet);
	setPetVisible(false);
	SetPetPos(bHasEdt);
	QLabel *buffs_pet[MaxBuffCount] = { ui.lbl_buff_1_pet, ui.lbl_buff_2_pet, ui.lbl_buff_3_pet, ui.lbl_buff_4_pet };
	pet.bindWidget(buffs_pet, MaxBuffCount, ui.progressBar_hp_pet, ui.progressBar_mp_pet);
		
	ui.lbl_deadflag_edt->setVisible(false);
	ui.lbl_deadflag_pet->setVisible(false);
	
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
		monster[3]->bindWidget(buffs_mon, MaxBuffCount, ui.progressBar_hp_monster4, ui.progressBar_mp_monster4);
		setVisible_monster4(true);
		//继续向下执行
	case 3:
		buffs_mon[0] = ui.lbl_buff_1_monster3; buffs_mon[1] = ui.lbl_buff_2_monster3;
		buffs_mon[2] = ui.lbl_buff_3_monster3; buffs_mon[3] = ui.lbl_buff_4_monster3;
		monster[2]->bindWidget(buffs_mon, MaxBuffCount, ui.progressBar_hp_monster3, ui.progressBar_mp_monster3);
		setVisible_monster3(true);
		//继续向下执行
	case 2:
		buffs_mon[0] = ui.lbl_buff_1_monster2; buffs_mon[1] = ui.lbl_buff_2_monster2;
		buffs_mon[2] = ui.lbl_buff_3_monster2; buffs_mon[3] = ui.lbl_buff_4_monster2;
		monster[1]->bindWidget(buffs_mon, MaxBuffCount, ui.progressBar_hp_monster2, ui.progressBar_mp_monster2);
		setVisible_monster2(true);
		//继续向下执行
	default:
		buffs_mon[0] = ui.lbl_buff_1_monster1; buffs_mon[1] = ui.lbl_buff_2_monster1;
		buffs_mon[2] = ui.lbl_buff_3_monster1; buffs_mon[3] = ui.lbl_buff_4_monster1;
		monster[0]->bindWidget(buffs_mon, MaxBuffCount, ui.progressBar_hp_monster1, ui.progressBar_mp_monster1);
		break;
	}

	OrganismsHead.append(ui.lbl_head_monster1);
	OrganismsHead.append(ui.lbl_head_monster2);
	OrganismsHead.append(ui.lbl_head_monster3);
	OrganismsHead.append(ui.lbl_head_monster4);
	foreach(QLabel *lbl, OrganismsHead)
	{
		lbl->installEventFilter(this);
	}

	DisplayTaskStatus();
}

void fight_fight::Init()
{
	m_bag_item = &PlayerIns.get_bag_item();
	m_bag_equip = &PlayerIns.get_bag_equip();

	monsterCount = dis.monsterCount;
	monster_normal_count = dis.normal.size();

	if (dis.boss.size() > 0 && dis.boss.first() != 0) {
		monster_boss_count = dis.boss.size();
	} else {
		monster_boss_count = 0;
	}

	for (int i = 0; i < monsterCount; i++)
	{
		monster[i] = new CMonster;
		camps_Rb.append(monster[i]);
	}

	nRound = 0;
	rt = RT_Rest;

	dlg_orgInfo = new fight_OrgInfo(this);
	dlg_orgInfo->hide();

	nCount_exp = nCount_coin = nCount_items = 0;

	GameMgrIns.get_taskListExceptComplete(taskOnDoing);

	fis = { 0 };
	fis.whatsMap = dis.ID;
	fis.killMonster = GameMgrIns.get_FIS().killMonster;

	ct_start = QDateTime::currentDateTime();
}

void fight_fight::DisplayRoleinfo(const CHuman *edt)
{
	ui.progressBar_hp_role->setMaximum(edt->get_hp_max());
	ui.progressBar_hp_role->setValue(edt->get_hp_c());
	ui.progressBar_mp_role->setMaximum(edt->get_mp_max());
	ui.progressBar_mp_role->setValue(edt->get_mp_c());

	ui.lbl_name_role->setText(edt->get_name());

	QString VocImg = QString(":/mirror/Resources/ui/f_0_%1.png").arg(edt->get_voc() + 1);
	ui.lbl_vocation_role->setPixmap(QPixmap(VocImg));

	qint32 headNo = ((edt->get_voc() - 1) * 2 + edt->get_gender());
	QString headImg = QString(":/mirror/Resources/head/%1.png").arg(headNo);
	ui.lbl_head_role->setPixmap(QPixmap(headImg));

	DisplayRoleParameter(edt);
}
void fight_fight::DisplayEmbodimentInfo(const CHuman *edt)
{
	ui.progressBar_hp_edt->setMaximum(edt->get_hp_max());
	ui.progressBar_hp_edt->setValue(edt->get_hp_c());
	ui.progressBar_mp_edt->setMaximum(edt->get_mp_max());
	ui.progressBar_mp_edt->setValue(edt->get_mp_c());

	ui.lbl_name_edt->setText(edt->get_name());

	QString VocImg = QString(":/mirror/Resources/ui/f_0_%1.png").arg(edt->get_voc() + 1);
	ui.lbl_vocation_edt->setPixmap(QPixmap(VocImg));

	qint32 headNo = ((edt->get_voc() - 1) * 2 + edt->get_gender());
	QString headImg = QString(":/mirror/Resources/head/%1.png").arg(headNo);
	ui.lbl_head_edt->setPixmap(QPixmap(headImg));

	DisplayEmbodimentParameter(edt);
}

void fight_fight::DisplayRoleParameter(const CHuman *edt)
{
	ui.lbl_level_role->setText(QStringLiteral("Lv:") + QString::number(edt->get_lv()));

	int32_t lvExp = g_JobAddSet[edt->get_voc() - 1].value(edt->get_lv()-1).exp;
	ui.progressBar_exp_role->setMaximum(lvExp);
	ui.progressBar_exp_role->setValue(edt->get_exp());
}
void fight_fight::DisplayEmbodimentParameter(const CHuman *edt)
{
	ui.lbl_level_edt->setText(QStringLiteral("Lv:") + QString::number(edt->get_lv()));

	int32_t lvExp = g_JobAddSet[edt->get_voc() - 1].value(edt->get_lv()-1).exp;
	ui.progressBar_exp_edt->setMaximum(lvExp);
	ui.progressBar_exp_edt->setValue(edt->get_exp());
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

inline QString fight_fight::Generate_Display_LineText(const QString &str1, const QString &skill, const QString &str2, bool bLuck, bool bep, QList<qint32> listDamage)
{
	QString strTmp = QStringLiteral("%1使用<font color=gray>%2</font>，").arg(str1).arg(skill);
	if (bLuck)
		strTmp += QStringLiteral("获得战神祝福, ");

	if (listDamage.first() <= 0)
	{
		strTmp += QStringLiteral("但是%1敏捷地躲开了。").arg(str2);
	}
	else
	{
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
	}
	return strTmp;
}

QString fight_fight::Generate_Display_DebuffInfo(const QString &name1, bool bLuck, const QString &targetName, const QString &SkillName, const realBuff &real)
{
	QString strTmp = QStringLiteral("%1使用:<font color=gray>%2</font>").arg(name1).arg(SkillName);
	if (bLuck) {
		strTmp += QStringLiteral(", 获得幸运女神赐福");
	}
	strTmp += QStringLiteral(", <font color=darkRed>%1</font>").arg(targetName);
	qint32 nTmp = 0 - real.value;
	switch (real.et)
	{
	case be_DamageEnhance:strTmp += QStringLiteral("伤害降低%1%").arg(nTmp); break;
	case be_DamageSave:strTmp += QStringLiteral("伤害加深%1%").arg(nTmp); break;
	case be_ac:strTmp += QStringLiteral("防御降低%1点").arg(nTmp); break;
	case be_mac:strTmp += QStringLiteral("魔御降低%1点").arg(nTmp); break;
	case be_hp:strTmp += QStringLiteral("生命上限减少%1点").arg(nTmp); break;
	case be_rhp:strTmp += QStringLiteral("每回合受到伤害%1点").arg(nTmp); break;
	case be_speed:strTmp += QStringLiteral("攻击间隔增加%1").arg(nTmp); break;
	default:
		break;
	}

	strTmp += QStringLiteral(", 效果持续<font color=magenta>%1</font>回合。").arg(real.time);
	return strTmp;
}
QString fight_fight::Generate_Display_buffInfo(const QString &name1, bool bLuck, const QString &targetName, const QString &SkillName, const realBuff &real)
{
	QString strTmp = QStringLiteral("%1使用:<font color=gray>%3</font>")
		.arg(name1).arg(SkillName);
	if (bLuck) {
		strTmp += QStringLiteral(", 获得幸运女神赐福");
	}
	strTmp += QStringLiteral(", <font color=DarkCyan>%1</font>").arg(targetName);
	switch (real.et)
	{
	case be_DamageEnhance:strTmp += QStringLiteral("伤害增强%1%").arg(real.value); break;
	case be_DamageSave:strTmp += QStringLiteral("伤害减免%1%").arg(real.value); break;
	case be_ac:strTmp += QStringLiteral("防御提升%1点").arg(real.value); break;
	case be_mac:strTmp += QStringLiteral("魔御提升%1点").arg(real.value); break;
	case be_hp:strTmp += QStringLiteral("生命上限增加%1点").arg(real.value); break;
	case be_rhp:strTmp += QStringLiteral("生命恢复增加%1点").arg(real.value); break;
	case be_speed:strTmp += QStringLiteral("攻击间隔减少%1").arg(real.value); break;
	default:
		break;
	}

	strTmp += QStringLiteral(", 效果持续<font color=magenta>%1</font>回合。").arg(real.time);
	return strTmp;
}

bool fight_fight::Step_Attack(COrganisms *org, const SkillFight &skill)
{
	bool bRes;
	if (org->get_camps() == 1)
	{
		bRes = Step_Attack_La(org, skill);
	}
	else
	{
		bRes = Step_Attack_Rb(org, skill);
	}
	return bRes;
}

bool fight_fight::Step_Summon(COrganisms *org, const SkillFight &skill)
{
	//宠物未死亡，不需要再次召唤。
	if (!pet.wasDead())
	{
		return false;
	}

	pet.set_camps(org->get_camps());
	qint32 nDamage = (org->get_sc1() + org->get_sc2()) / 2;
	pet.ReplaceSoul(org->get_name(), skill.no, skill.level, org->get_lv(), nDamage);
	pet.reset_live(time_remain);

	ui.lbl_name_pet->setText(pet.get_name());
	ui.lbl_head_pet->setPixmap(QPixmap::fromImage(pet.get_head()));
	ui.lbl_level_pet->setText(QStringLiteral("Lv:") + QString::number(pet.get_lv()));
	ui.lbl_deadflag_pet->setVisible(false);
	setPetVisible(true);

	ui.display_Fighting->append(
		QStringLiteral("<font color=DarkCyan>%1</font>使用<font color=gray>%2</font>，召唤宠物帮助战斗")
		.arg(org->get_name()).arg(skill.name));
	return true;
}
bool fight_fight::Step_role_Treat(COrganisms *org, const SkillFight &skill)
{
	qint32 rhp;
	double dTmp1, dTmp2;
	
	const Info_SkillTreat st = g_SkillTreat[skill.no];

	//找出血量最少的成员
	COrganisms *orgForTreat = camps_La[0];
	dTmp2 = 1.0 * orgForTreat->get_hp_c() / orgForTreat->get_hp_max();
	for (int i = 0; i < camps_La.size(); i++)
	{
		if (!camps_La[i]->wasDead())
		{
			dTmp1 = 1.0 * camps_La[i]->get_hp_c() / camps_La[i]->get_hp_max();
			if (dTmp1 < dTmp2)
			{
				orgForTreat = camps_La[i];
				dTmp2 = dTmp1;
			}
		}
	}

	//如果血量最少的成员，其血量仍然超过50%，那么不使用治疗技能。
	if (dTmp2 > 0.5)
	{
		return false;
	}

	if (st.targets == -1)
	{
		//全体补血技能。只要任一目标血量小于50%，都会释放技能。
		for (int i = 0; i < camps_La.size(); i++)
		{
			if (!camps_La[i]->wasDead())
			{
				rhp = camps_La[i]->get_hp_max() * (st.hpr_basic + st.hpr_add * skill.level) / 100;
				camps_La[i]->set_hp_c(camps_La[i]->get_hp_c() + rhp);

				QString strTmp =
					QStringLiteral("<font color=DarkCyan>%1</font>使用:<font color=gray>%2</font>为<font color=magenta>%3</font>恢复HP：<font color=magenta>%4</font>")
					.arg(org->get_name()).arg(skill.name).arg(camps_La[i]->get_name()).arg(rhp);
				ui.display_Fighting->append(strTmp);
			}
		}
	}
	else
	{
		rhp = orgForTreat->get_hp_max() * (st.hpr_basic + st.hpr_add * skill.level) / 100;
		orgForTreat->set_hp_c(orgForTreat->get_hp_c() + rhp);

		QString strTmp =
			QStringLiteral("<font color=DarkCyan>%1</font>使用:<font color=gray>%2</font>为<font color=magenta>%3</font>恢复HP：<font color=magenta>%4</font>")
			.arg(org->get_name()).arg(skill.name).arg(orgForTreat->get_name()).arg(rhp);
		ui.display_Fighting->append(strTmp);
	}
	
	return true;
}
bool fight_fight::MStep_role_Buff(COrganisms *org, const SkillFight &skill)
{
	realBuff real;
	bool bLuck = false;
	
	if (!Init_realBuff(org, skill, bLuck, real))
	{
		//has error, can't use the skill
		LogIns.append(LEVEL_ERROR, __FUNCTION__, mirErr_para);
		return false;
	}

	const Info_SkillBuff &sb = g_SkillBuff.value(skill.no);
	int32_t targets = sb.targets;
	if (targets == -1)
	{
		foreach(COrganisms *member, camps_La) {
			if (!member->wasDead()) {
				member->appendBuff(real);
			}			
		}
		ui.display_Fighting->append(Generate_Display_buffInfo(
			QStringLiteral("<font color=DarkCyan>%1</font>").arg(org->get_name()),
			bLuck, QStringLiteral("我方全体"), skill.name, real));
	}
	else if (targets == 0)
	{
		org->appendBuff(real);
		ui.display_Fighting->append(Generate_Display_buffInfo(
			QStringLiteral("<font color=DarkCyan>%1</font>").arg(org->get_name()),
			bLuck, QStringLiteral("自身"), skill.name, real));
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

bool fight_fight::MStep_role_Debuff(COrganisms *org, const SkillFight &skill)
{
	realBuff real;
	bool bLuck = false;
	
	if (!Init_realBuff(org, skill, bLuck, real))
	{
		//has error, can't use the skill
		LogIns.append(LEVEL_ERROR, __FUNCTION__, mirErr_para);
		return false;
	}

	const Info_SkillBuff &sb = g_SkillBuff.value(skill.no);
	int32_t targets = sb.targets;
	if (targets == -1)
	{
		foreach(COrganisms *mon, camps_Rb) {
			if (!mon->wasDead()) {
				mon->appendBuff(real);
			}
		}
		ui.display_Fighting->append(Generate_Display_DebuffInfo(
			QStringLiteral("<font color=DarkCyan>%1</font>").arg(org->get_name()),
			bLuck, QStringLiteral("敌方全体"), skill.name, real));
	}
	else if (targets == 1)
	{
		//随机一个怪物
		int32_t which = qrand() % camps_Rb.size();
		for (int i = 0; i < camps_Rb.size(); i++)
		{
			//如果当前怪物未死亡，添加debuff。否则向下寻找一个未死亡的目标。
			if (!camps_Rb.at(which)->wasDead())
			{
				camps_Rb[which]->appendBuff(real);
				break;
			}
			else
			{
				++which;
				if (which >= camps_Rb.size()) {
					which = 0;
				}

			}
		}
		
		ui.display_Fighting->append(Generate_Display_DebuffInfo(
			QStringLiteral("<font color=DarkCyan>%1</font>").arg(org->get_name()),
			bLuck, camps_Rb.at(which)->get_name(), skill.name, real));
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

bool fight_fight::Init_realBuff(COrganisms *org, const SkillFight &skill, bool &bLuck, realBuff &real)
{
	if (!g_SkillBuff.contains(skill.no))
	{
		return false;
	}
	
	int32_t flag;
	if (skill.type == 2) {
		flag = 1;
	} else {
		flag = -1;
	}

	qint32 nTmp;
	const Info_SkillBuff &sb = g_SkillBuff.value(skill.no);
	real = { 0 };
	switch (sb.et)
	{
	case et_DamageEnhance:
		real.et = be_DamageEnhance;
		nTmp = sb.basic + skill.level * sb.add; 
		break;
	case et_DamageSave:
		real.et = be_DamageSave;
		nTmp = sb.basic + skill.level * sb.add; 
		break;
	case et_ac_fixed:
		real.et = be_ac;
		nTmp = (sb.basic + skill.level * sb.add) * org->GetAttack(sb.type, bLuck) / 100;
		break;
	case et_mac_fixed:
		real.et = be_mac;
		nTmp = (sb.basic + skill.level * sb.add) * org->GetAttack(sb.type, bLuck) / 100;
		break;
	case et_ac_percent:
		real.et = be_ac;
		nTmp = (sb.basic + skill.level * sb.add) * org->get_ac() / 100; 
		break;
	case et_mac_percent:
		real.et = be_mac;
		nTmp = (sb.basic + skill.level * sb.add) * org->get_mac() / 100; 
		break;
	case et_speed:
		real.et = be_speed;
		nTmp = (sb.basic + skill.level * sb.add) * org->get_intervel() / 100;
		break;
	case et_rhp:
		real.et = be_rhp;
		nTmp = (sb.basic + skill.level * sb.add)*org->GetAttack(sb.type, bLuck) / 100;
		break;
	default:
		break;
	}

	real.value = nTmp * flag;
	real.id = skill.no;
	real.name = skill.name;
	real.icon = skill.icon;
	real.time = sb.time;

	return true;
}

bool fight_fight::Step_Attack_La(COrganisms *attacker, const SkillFight &skill)
{
	bool bLuck = false, bep = false;
	qint32 nTmp;

	qint32 damageId = g_SkillBasic.value(skill.id).no;
	qint32 targets = g_SkillDamage.value(damageId).targets;
	
	int32_t which = qrand() % camps_Rb.size();
	for (int i = 0; i < camps_Rb.size() && targets > 0; i++)
	{
		nTmp = which;
		COrganisms *defender = camps_Rb[which++];
		if (which >= camps_Rb.size()) {
			which = 0;
		}
		if (defender->wasDead())
		{
			continue;
		}
		targets--;
		QList<int32_t> ListDamage;
		attacker->attack(defender, damageId, skill.level, bLuck, &ListDamage);
		ui.display_Fighting->append(Generate_Display_LineText(
			QStringLiteral("<font color=DarkCyan>%1</font>").arg(attacker->get_name()),
			skill.name,
			QStringLiteral("<font color=DarkRed>%1</font>").arg(defender->get_name()),
			bLuck, bep, ListDamage));

		if (defender->wasDead())
		{
			//统计信息
			monsterID id = monster[nTmp]->get_id();
			fis.killMonster.insert(id, fis.killMonster.value(id) + 1);

			//设置死亡状态
			QLabel *deadflag[] = { ui.lbl_deadflag_monster1, ui.lbl_deadflag_monster2, ui.lbl_deadflag_monster3, ui.lbl_deadflag_monster4 };
			deadflag[nTmp]->setVisible(true);
			
			//战斗是否结束？
			--monsterRemainderCount;
			if (monsterRemainderCount <= 0)
			{
				FightFinish(FR_victory);
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
	const _tagDrop &dropSet = g_mapDropSet.value(dis.ID);
	if (!bBoss)
	{
		//如果不是BOSS，则掷骰子。否则为必掉。
		if (0 != (qrand() % dropSet.chance))
		{
			return;		//未达到掉落机率
		}
	}

	nTmp = qrand() % dropSet.list.size();
	itemitem = dropSet.list.at(nTmp);
	if (itemitem > g_itemID_start_equip && itemitem <= g_itemID_stop_equip)
	{
		//掉落装备,大于拾取过滤则拾取，否取丢弃。
		CreateEquip(bBoss, itemitem, DropEquip);
		const Info_basic_equip* equip = GetEquipBasicInfo(DropEquip.ID);
		if (equip != nullptr)
		{
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
	}
	else
	{
		//掉落道具
		const Info_Item *item = FindItem_Item(itemitem);
		if (item != nullptr)
		{
			List_Pick.append(item->name);
			m_bag_item->insert(itemitem, m_bag_item->value(itemitem) + 1);
		}
	}
	++nCount_items;
	

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

void fight_fight::Action(COrganisms *org)
{
	org->update_beforeAction();

	bool bUsedSkill = false;
	qint32 nTmp;

	for (qint32 i = 0; i < org->get_skill_fight_size(); i++)
	{
		const SkillFight &skill = org->get_skill_fight_cur();
		nTmp = org->MoveToNextFightSkill();
		if (skill.cd_c <= 0)
		{
			switch (skill.type)
			{
			case 1: bUsedSkill = Step_Attack(org, skill); break;
			case 2: bUsedSkill = MStep_role_Buff(org, skill); break;
			case 3: bUsedSkill = MStep_role_Debuff(org, skill); break;
			case 4: bUsedSkill = Step_Summon(org, skill); break;
			case 5: bUsedSkill = Step_role_Treat(org, skill); break;
			default: bUsedSkill = true; break;
			}
		}
		if (bUsedSkill) {
			org->ResetSkillCD(nTmp);
			break;
		}
	}
	if (!bUsedSkill)
	{
		ui.display_Fighting->append(QStringLiteral("%1无可用技能").arg(org->get_name()));
	}
}
bool fight_fight::Step_Attack_Rb(COrganisms *attacker, const SkillFight &skill)
{	
	bool bLuck;
	qint32 damageId = g_SkillBasic.value(skill.id).no;
	qint32 targets = g_SkillDamage.value(damageId).targets;

	int32_t which = qrand() % camps_La.size();
	for (int i = 0; i < camps_La.size() && targets > 0; i++)
	{
		COrganisms *defender = camps_La[which++];
		if (which >= camps_La.size()) {
			which = 0;
		}
		if (defender->wasDead())
		{
			continue;
		}
		targets--;
		QList<int32_t> ListDamage;
		attacker->attack(defender, damageId, skill.level, bLuck, &ListDamage);
		ui.display_Fighting->append(Generate_Display_LineText(
			QStringLiteral("<font color = darkRed>%1</font>").arg(attacker->get_name()),
			skill.name,
			QStringLiteral("<font color=DarkCyan>%1</font>").arg(defender->get_name()),
			bLuck, false, ListDamage));

		if (defender->wasDead())
		{
			if (defender == edt_role) {
				FightFinish(FR_fail);
			} else if (defender == edt_edt) {
				ui.lbl_deadflag_edt->setVisible(true);
			} else {
				ui.lbl_deadflag_pet->setVisible(true);
			}
		}
	}
	return true;
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
	return true;
}

bool fight_fight::EncounterBoss()
{
	static qint32 nElapse_pre_boss = 0;
	bool bBoss = false;

	if (monster_boss_count > 0 && fis.nCount_victory > 5)
	{
		++nElapse_pre_boss;
		if (nElapse_pre_boss > 100) {
			bBoss = true;
		} else {
			bBoss = ((qrand() % g_fight_boss_probability) == 0);
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
		static int32_t nXSpeedCount = 0;
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
			rt = RT_Rest;
			break;
		}
	}
}


bool fight_fight::eventFilter(QObject *obj, QEvent *ev)
{
	if (ev->type() == QEvent::Enter)
	{
		for (qint32 i = 0; i < monsterCount; i++)
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

	qint32 sec = ct_start.secsTo(QDateTime::currentDateTime()) + 1;		//防止除0
	if (sec > 3600) {
		strTmp = QStringLiteral("%1小时").arg(sec / 3600);
	}
	strTmp += QStringLiteral("%1分钟").arg((sec % 3600)/60);
	ui.lbl_statistics_time->setText(strTmp);

	qint32 nCount_wars = fis.nCount_draw + fis.nCount_fail + fis.nCount_victory;
	ui.lbl_statistics_times->setText(QString::number(nCount_wars));

	strTmp = QStringLiteral("%1秒/场").arg(sec / nCount_wars);
	ui.lbl_statistics_warSpend->setText(strTmp);

	strTmp = QStringLiteral("%1%").arg(100 * fis.nCount_victory / nCount_wars);
	ui.lbl_statistics_winRate->setText(strTmp);

	strTmp = QStringLiteral("%1/小时").arg(nCount_exp * 3600 / sec);
	ui.lbl_statistics_exp->setText(strTmp);

	strTmp = QStringLiteral("%1/小时").arg(nCount_coin * 3600 / sec);
	ui.lbl_statistics_coin->setText(strTmp);

	strTmp = QStringLiteral("%1/小时").arg(nCount_items * 3600 / sec);
	ui.lbl_statistics_item->setText(strTmp);

	int32_t level = edt_role->get_lv();
	int32_t lvExp = g_JobAddSet.at(edt_role->get_voc() - 1).value(edt_role->get_lv()).exp;
	int32_t needExp = lvExp - edt_role->get_exp();
	QString strTmp2;
	if (lvExp >= 100000) {
		strTmp2 = QStringLiteral("%1万").arg(needExp / 10000);
	} else {
		strTmp2 = QString::number(needExp);
	}

	QString strTmp3;
	qint32 GainExpPerSec = nCount_exp / sec + 1;		//防止除0
	nTmp = needExp / GainExpPerSec;
	
	if (nTmp > 999 * 3600) {
		strTmp3 = QStringLiteral("未知时间");
	} else {
		strTmp3 = QStringLiteral("%1小时%2分钟").arg(nTmp / 3600).arg((nTmp % 3600) / 60);
	}
	strTmp = QStringLiteral("距离%1级还需要%2经验，预计升级还需要%3").arg(level + 1).arg(strTmp2).arg(strTmp3);
	ui.lbl_statistics_info->setText(strTmp);
}

void fight_fight::DisplayTaskStatus()
{
	QString strlabel, strTitle, strInfo, strTmp;

	if (taskOnDoing.size() == 0)
	{
		strlabel = QStringLiteral("<p>当前无可用任务</p>");
	}
	else
	{
		for (auto iter = taskOnDoing.constBegin(); iter != taskOnDoing.constEnd(); iter++)
		{
			//任务标题
			if (iter->ts == task::ts_Doing) {
				if (wasComplete(*iter)) {
					strTmp = QStringLiteral("<font color = green>完成</font>");
				} else {
					strTmp = QStringLiteral("<font color = white>正在执行</font>");
				}
			} else {
				strTmp = QStringLiteral("<font color = gray>未领取</font>");
			}
			strTitle = QStringLiteral("<p>%1(%2)</p>").arg(iter->name).arg(strTmp);

			//任务具体内容
			GeneralTaskInfo(*iter, strInfo);

			//组合
			strlabel += strTitle + QStringLiteral("<p>&nbsp;-&nbsp;%1</p>").arg(strInfo);
		}
	}

	ui.lbl_task->setText(strlabel);
}

bool fight_fight::wasComplete(const task::taskItem &item)
{
	bool bComplete = false;

	switch (item.tType)
	{
	case task::tt_HoldRound:bComplete = ((fis.whatsMap == item.tMap) && (fis.nCount_StraightVictory >= item.tCount)); break;
	case task::tt_KillMonster:bComplete = (fis.killMonster.value(item.tID) >= item.tCount);	break;
	case task::tt_Item:
		//暂未完成，以后研究
		bComplete = false;
		break;
	case task::tt_Level:bComplete = (PlayerIns.get_edt_role().get_lv() >= item.tCount); break;
	case task::tt_Coin:bComplete = (PlayerIns.get_coin() >= item.tCount); break;
	case task::tt_Gold:bComplete = (PlayerIns.get_gold() >= item.tCount); break;
	case task::tt_Rep:bComplete = (PlayerIns.get_rep() >= item.tCount); break;
		break;
	default:
		//nothing
		break;
	}
	return bComplete;
}

qint32 fight_fight::GeneralTaskInfo(const task::taskItem& item, QString &str)
{
	switch (item.tType)
	{
	case task::tt_HoldRound:GeneralTaskInfo_HoldRound(item, str); break;
	case task::tt_KillMonster: GeneralTaskInfo_KillMonster(item, str); break;
	case task::tt_Item:GeneralTaskInfo_Item(item.tID, item.tCount, str); break;
	default:
		str = QStringLiteral("请回到主界面查看"); break;
		break;
	}
	return 0;
}

void fight_fight::GeneralTaskInfo_HoldRound(const task::taskItem& item, QString &str)
{
	if (!g_MonsterDistribute.contains(item.tMap))
	{
		str = QStringLiteral("错误的地图ID：%1").arg(item.tMap);
		return;
	}
	const Info_Distribute &dis = g_MonsterDistribute.value(item.tMap);

	qint32 count = (fis.whatsMap == item.tMap ? fis.nCount_StraightVictory : 0);

	str = QStringLiteral("在<font color = cyan>%1</font>坚守<font color = cyan>%2</font>回合").arg(dis.name).arg(item.tCount);
	if (item.ts == task::ts_Doing)
	{
		str += QStringLiteral("（<font color = green>当前:%1</font>）").arg(count);
	}
}
void fight_fight::GeneralTaskInfo_KillMonster(const task::taskItem& item, QString &str)
{
	if (!g_MonsterInfo.contains(item.tID))
	{
		str = QStringLiteral("错误的怪物ID：%1").arg(item.tID);
		return;
	}
	if (!g_MonsterDistribute.contains(item.tMap))
	{
		str = QStringLiteral("错误的地图ID：%1").arg(item.tMap);
		return;
	}

	const Info_Distribute &dis = g_MonsterDistribute.value(item.tMap);
	const MonsterInfo &info = g_MonsterInfo.value(item.tID);
	qint32 kill = fis.killMonster.value(item.tID);

	str = QStringLiteral("前往<font color = cyan>%1</font>击杀<font color = cyan>%2</font>").arg(dis.name).arg(info.name);
	if (item.ts == task::ts_Doing)
	{
		str += QStringLiteral("（<font color = green>%1/%2</font>）").arg(kill).arg(item.tCount);
	}
}

void fight_fight::GeneralTaskInfo_Item(qint32 tID, qint32 tCount, QString &str)
{
	QString itemName;
	qint32 nCount = 0;
	if (g_EquipList.contains(tID))
	{
		itemName = g_EquipList.value(tID).name;
	}
	else if (g_ItemList.contains(tID))
	{
		itemName = g_ItemList.value(tID).name;

		nCount = PlayerIns.get_bag_item().value(tID);
	}
	else
	{
		str = QStringLiteral("错误的道具ID：%1").arg(tID);
		return;
	}

	str = QStringLiteral("收集<font color = cyan>%1</font>，共%2/%3").arg(itemName).arg(nCount).arg(tCount);
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

	ui.lbl_info_pet->move(ui.lbl_info_pet->pos());
}

void fight_fight::SetPetPos(bool hasEdt)
{
	if (hasEdt)
	{
		return; //不需要移动位置
	}

	QPoint offset(0, 140);
	ui.lbl_info_pet->move(ui.lbl_info_pet->pos() - offset);
	ui.lbl_name_pet->move(ui.lbl_name_pet->pos() - offset);
	ui.lbl_head_pet->move(ui.lbl_head_pet->pos() - offset);
	ui.progressBar_hp_pet->move(ui.progressBar_hp_pet->pos() - offset);
	ui.progressBar_mp_pet->move(ui.progressBar_mp_pet->pos() - offset);
	ui.lbl_level_pet->move(ui.lbl_level_pet->pos() - offset);
	ui.lbl_vocation_pet->move(ui.lbl_vocation_pet->pos() - offset);

	ui.lbl_buff_1_pet->move(ui.lbl_buff_1_pet->pos() - offset);
	ui.lbl_buff_2_pet->move(ui.lbl_buff_2_pet->pos() - offset);
	ui.lbl_buff_3_pet->move(ui.lbl_buff_3_pet->pos() - offset);
	ui.lbl_buff_4_pet->move(ui.lbl_buff_4_pet->pos() - offset);

	ui.lbl_deadflag_pet->move(ui.lbl_deadflag_pet->pos() - offset);
}

void fight_fight::setEdtVisible(bool Visible)
{
	ui.lbl_info_edt->setVisible(Visible);
	ui.lbl_name_edt->setVisible(Visible);
	ui.lbl_head_edt->setVisible(Visible);
	ui.progressBar_hp_edt->setVisible(Visible);
	ui.progressBar_mp_edt->setVisible(Visible);
	ui.lbl_level_edt->setVisible(Visible);
	ui.lbl_vocation_edt->setVisible(Visible);

	ui.progressBar_exp_edt->setVisible(Visible);

	ui.lbl_buff_1_edt->setVisible(Visible);
	ui.lbl_buff_2_edt->setVisible(Visible);
	ui.lbl_buff_3_edt->setVisible(Visible);
	ui.lbl_buff_4_edt->setVisible(Visible);
}

inline void fight_fight::CalcDropBasicAndDisplay()
{
	QString strTmp;
	double dTmp1;
	qint32 nTmp, nDropExpForRole, nDropExpForEdt, nDropCoin, nDropRep, nDropSoul;
	qint32 nSum;
	bool bHaveBoss = false;
	
	//怪物死掉，角色增加经验及金币。若是BOSS，再增加声望。
	//必须先乘1.0转化为double，否则等级相减运算将提升到uint层次从而得到一个无穷大。
	nTmp = nSum = nDropExpForRole = nDropExpForEdt = nDropCoin = nDropRep = nDropSoul = 0;
	for (int i = 0; i < monsterCount; i++)
	{
		nTmp += monster[i]->get_exp();
		nSum += monster[i]->get_lv();
		if (monster[i]->isBoss())
		{
			nDropRep += qMax(monster[i]->get_lv() - edt_role->get_lv() - 15, 1);
		}
	}
	nDropCoin = nSum;

	dTmp1 = atan(0.3 * (1.0 * nSum / monsterCount - edt_role->get_lv()));
	nDropExpForRole = nTmp * ((dTmp1 + 1.58) / 2);
	if (bHasEdt && !edt_edt->wasDead() && edt_edt->get_lv() < MaxLevel)
	{
		dTmp1 = atan(0.3 * (1.0 * nSum / monsterCount - edt_edt->get_lv()));
		nDropExpForEdt = nTmp / 2  * ((dTmp1 + 1.58) / 2);
		edt_edt->add_exp(nDropExpForEdt);
		ui.progressBar_exp_edt->setValue(edt_edt->get_exp());
		if (edt_edt->get_exp() == 0) {
			ui.progressBar_mp_edt->setValue(edt_edt->get_mp_c());
			DisplayEmbodimentParameter(edt_edt);
			ui.display_Fighting->append(QStringLiteral("<font color=white>%1升级了. </font>").arg(edt_edt->get_name()));
		}
		
		nDropExpForRole /= 2;
	}

	if (edt_role->get_lv() < MaxLevel)
	{
		edt_role->add_exp(nDropExpForRole);
		ui.progressBar_exp_role->setValue(edt_role->get_exp());
		if (edt_role->get_exp() == 0){
			ui.progressBar_mp_role->setValue(edt_edt->get_mp_c());
			DisplayRoleParameter(edt_role);
			ui.display_Fighting->append(QStringLiteral("<font color=white>%1升级了. </font>").arg(edt_role->get_name()));
		}
	} else {
		nDropExpForRole = 0;
	}
	
	PlayerIns.add_coin(nDropCoin);
	PlayerIns.add_rep(nDropRep);
//	player->add_soul(nDropSoul);

	//统计信息
	nCount_exp += nDropExpForRole + nDropExpForEdt;
	nCount_coin += nDropCoin;

	ui.display_Fighting->append("");
	strTmp = QStringLiteral("你率领小伙伴们击退了<font color=DarkRed>%1</font>率领的小分队").arg(monster[0]->get_name());
	strTmp += QStringLiteral(", 获得金币: <font color=#808000>%1</font>").arg(nDropCoin);
	if (bHaveBoss)
	{
		strTmp += QStringLiteral("，声望: <font color=DarkMagenta>%1</font>").arg(nDropRep);
//		strTmp += QStringLiteral("，灵魂点: <font color=DarkMagenta>%1</font>").arg(nDropSoul);
	}
	strTmp += QStringLiteral("。%1获得经验: <font color=green>%2</font>").arg(edt_role->get_name()).arg(nDropExpForRole);
	if (bHasEdt)
	{
		strTmp += QStringLiteral(", %1获得经验:<font color=green>%2</font>").arg(edt_edt->get_name()).arg(nDropExpForEdt);
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

	//理论上应该将camps_La和camps_Rb放一起调度，但是组合容易太麻烦，故采取此种折中方法。
	//先查询camps_La是否有可行动角色，如果有，则行动，并结束本回合。否则继续查询camps_Rb。
	bool bNotAction = true;	
	if (bNotAction) {
		foreach(COrganisms *org, camps_La)
		{
			if (!org->wasDead() && time_remain > org->get_live()) {
				Action(org);
				bNotAction = false;
				break;
			}
		}
	}
	if (bNotAction) {
		foreach(COrganisms *org, camps_Rb)
		{
			if (!org->wasDead() && time_remain > org->get_live()) {
				Action(org);
				bNotAction = false;
				break;
			}
		}
	}

// 	QString strTmp = QStringLiteral("F:%1 R:%2 E:%3 P:%4-")
// 		.arg(time_remain / nFightInterval)
// 		.arg(edt_role->get_live() / nFightInterval)
// 		.arg(edt_edt->get_live() / nFightInterval)
// 		.arg(pet.get_live() / nFightInterval);
// 	strTmp += QStringLiteral("m1:%1 m2:%2 m3:%3 m4:%4")
// 		.arg(monster[0]->get_live() / nFightInterval)
// 		.arg(monsterCount >= 2 ? monster[1]->get_live() / nFightInterval : 0)
// 		.arg(monsterCount >= 3 ? monster[0]->get_live() / nFightInterval : 0)
// 		.arg(monsterCount >= 4 ? monster[0]->get_live() / nFightInterval : 0);
// 	ui.lbl_time->setText(strTmp);		

	//战斗记时
	time_remain += nFightInterval;
}
void fight_fight::round_Rest()
{
	int nTmp;
	if (nRound % 10 == 0)
	{
		nTmp = RestTime();
		ui.lbl_round_time->setText(QString::number(nTmp));
		ui.lbl_round_info->setText(QStringLiteral("休息中..."));	
	
		if (nTmp <= 0)
		{
			time_remain = 0;
			foreach(COrganisms *org, camps_La)
			{
				org->reset_live(time_remain);
			}

			nRound = 0;
			time_findMonster =  qrand() % 40 + 40;
			rt = RT_FindMonster;
		}
	}
}

void fight_fight::FightFinish(FightResult fr)
{
	if (fr == FR_victory)
	{
		++fis.nCount_victory;
		++fis.nCount_StraightVictory;

		CalcDropBasicAndDisplay();
		CalcDropItemsAndDisplay();

		if (monster[0]->isBoss() && dis.ID < 1000 && GameMgrIns.get_maxMapID() <= dis.ID)
		{
			GameMgrIns.Set_maxMapID(dis.ID);
		}
	}
	else if (fr == FR_fail)
	{
		++fis.nCount_fail;
		fis.nCount_StraightVictory = 0;
		ui.display_Fighting->append(QStringLiteral("<font color=red>战斗失败。</font>"));
	}
	else
	{
		++fis.nCount_draw;
		fis.nCount_StraightVictory = 0;
		ui.display_Fighting->append(QStringLiteral("<font color=white>战斗超时。</font>"));
	}
	ui.lbl_deadflag_edt->setVisible(false);
	ui.lbl_deadflag_pet->setVisible(false);

	nRound = 0;

	foreach(COrganisms *org, camps_La)
	{
		org->ResetSkillCD();
	}
	DisplayStatistics();
	DisplayTaskStatus();

	rt = RT_Rest;
}

qint32 fight_fight::RestTime()
{
	//先计算已失去的血量，然后除以每秒回复血量，即得出剩余休息时间。
	int32_t nTmp1, nTmp;
	nTmp1 = nTmp = 0;

	double rhp = 0.1;
#ifdef _DEBUG
	rhp = 0.5;
#endif
	foreach(COrganisms *org, camps_La)
	{
		if (org->get_hp_max() <= 0) {
			nTmp1 = 0;
		} else {
			org->set_hp_c(org->get_hp_c() + org->get_hp_max() * rhp);
			nTmp1 = (org->get_hp_max() - org->get_hp_c()) * 100 / org->get_hp_max();
		}

		if (nTmp1 > nTmp) {
			nTmp = nTmp1;
		}
	}

	return nTmp * rhp + 0.5;
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
	case eet_fixed_hit:
	case eet_fixed_dodge:
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
		Tmp = 4;
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

	dTmp =  qrand() % 100;
	if (dTmp > 98) {
		nTmp = 3;
	} else if (dTmp > 90) {
		nTmp = 2;
	} else if (dTmp > 75) {
		nTmp = 1;
	} else {
		nTmp = 0;
	}

	//初始化
	DropEquip = { 0 };
	DropEquip.ID = id;
	DropEquip.extraAmount = nTmp;

	QVector<EquipExtraType> eet_attack = { eet_fixed_dc, eet_fixed_mc, eet_fixed_sc, eet_fixed_hit, eet_percent_dc, eet_percent_mc, eet_percent_sc };
	QVector<EquipExtraType> eet_defense = { eet_fixed_ac, eet_fixed_mac, eet_fixed_dodge, eet_percent_ac, eet_percent_mac };

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
			dTmp = dTmp * 0.7 * qrand() / RAND_MAX + dTmp * 0.3;
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

	memset(&pickFilter, 0, sizeof(pickFilter));

	do 
	{
		pickFilter[i++] = nTmp & 0x01;
		nTmp = nTmp >> 1;
	} while (nTmp);
}