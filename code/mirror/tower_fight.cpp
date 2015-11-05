#include "tower_fight.h"

#include <QFile>
#include "cryptography.h"
#include "mirrorlog.h"

extern QMap<itemID, Info_Item> g_ItemList;
extern QMap<monsterID, MonsterInfo> g_MonsterInfo;
extern QMap<skillID, Info_SkillBasic> g_SkillBasic;
extern QMap<qint32, Info_SkillDamage> g_SkillDamage;
extern QMap<qint32, Info_SkillBuff> g_SkillBuff;
extern QMap<qint32, Info_SkillSummon> g_SkillSummon;
extern QVector<Info_jobAdd> g_JobAddSet;

tower_fight::tower_fight(QWidget* parent, qint32 w_towerLv, const TowerDistributeInfo &w_dis, CPlayer *const w_player)
	: QDialog(parent), towerLv(w_towerLv), dis(w_dis), player(w_player)
{
	ui.setupUi(this);

	nTimeOutTime = 999 * 60 * 1000;
	time_remain = 0;
	nSkillIndex = 0;
	whichMonster = 0;
	bSuccess = false;

	m_bag_item = player->get_bag_item();

	InitUI();
	loadDropSet();
	DisplayDropSetting();
	DisplayRoleinfo();

	GenerateMonster();

	player->set_hp_c(player->get_hp_max());
	player->set_mp_c(player->get_mp_max());
	player->reset_live(0);
}

tower_fight::~tower_fight()
{

}

void tower_fight::on_btn_start_clicked()
{
	ui.btn_start->setEnabled(false);
	nFightTimer = startTimer(nFightInterval);
}

void tower_fight::on_btn_quit_clicked(void)
{
	ui.listWidget->clear();
	killTimer(nFightTimer);

	close();
}

bool tower_fight::loadDropSet()
{
	char MD5[] = "89979cd3cadd93052be87088f573c515";
	QFile file("./db/TowerDrop.db");
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	QByteArray documentContent = file.readAll();
	file.close();

	if (!cryptography::verifyDB_MD5(MD5, documentContent))
	{
		LogIns.append(LEVEL_ERROR, __FUNCTION__, mirErr_MD5);
		return false;
	}

	QDataStream out(documentContent);

	QVector<quint32> items;
	quint32 lv, count, item;

	while (!out.atEnd())
	{
		out >> lv >> count;

		items.clear();
		while (count)
		{
			out >> item;
			items.append(item);
			--count;
		};

		drop.append(items);
	}

	return true;
}


void tower_fight::InitUI()
{
	ui.progressBar_hp_role->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
	ui.progressBar_hp_pet->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
	ui.progressBar_hp_monster1->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
	ui.progressBar_hp_monster2->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
	ui.progressBar_hp_monster3->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");

	ui.progressBar_mp_role->setStyleSheet("QProgressBar::chunk { background-color: rgb(0, 0, 255) }");
	ui.progressBar_mp_pet->setStyleSheet("QProgressBar::chunk { background-color: rgb(0, 0, 255) }");
	ui.progressBar_mp_monster1->setStyleSheet("QProgressBar::chunk { background-color: rgb(0, 0, 255) }");
	ui.progressBar_mp_monster2->setStyleSheet("QProgressBar::chunk { background-color: rgb(0, 0, 255) }");
	ui.progressBar_mp_monster3->setStyleSheet("QProgressBar::chunk { background-color: rgb(0, 0, 255) }");

	
	buffDisp_Role[0] = ui.lbl_buff_1_role;
	buffDisp_Role[1] = ui.lbl_buff_2_role;
	buffDisp_Role[2] = ui.lbl_buff_3_role;
	buffDisp_Role[3] = ui.lbl_buff_4_role;
	player->bingWidget(ui.lbl_name_role, ui.lbl_level_role, ui.lbl_head_role, buffDisp_Role, MaxBuffCount, ui.progressBar_hp_role, ui.progressBar_mp_role);

	buffDisp_pet[0] = ui.lbl_buff_1_pet;
	buffDisp_pet[1] = ui.lbl_buff_2_pet;
	buffDisp_pet[2] = ui.lbl_buff_3_pet;
	buffDisp_pet[3] = ui.lbl_buff_4_pet;
	pet.bingWidget(ui.lbl_name_pet, ui.lbl_level_pet, ui.lbl_head_pet, buffDisp_pet, MaxBuffCount, ui.progressBar_hp_pet, ui.progressBar_mp_pet);
	
	buffDisp_Mon1[0] = ui.lbl_buff_1_monster1;
	buffDisp_Mon1[1] = ui.lbl_buff_2_monster1;
	buffDisp_Mon1[2] = ui.lbl_buff_3_monster1;
	buffDisp_Mon1[3] = ui.lbl_buff_4_monster1;
	monster[0].bingWidget(ui.lbl_name_monster1, ui.lbl_level_monster1, ui.lbl_head_monster1, buffDisp_Mon1, MaxBuffCount, ui.progressBar_hp_monster1, ui.progressBar_mp_monster1);

	buffDisp_Mon2[0] = ui.lbl_buff_1_monster2;
	buffDisp_Mon2[1] = ui.lbl_buff_2_monster2;
	buffDisp_Mon2[2] = ui.lbl_buff_3_monster2;
	buffDisp_Mon2[3] = ui.lbl_buff_4_monster2;
	monster[1].bingWidget(ui.lbl_name_monster2, ui.lbl_level_monster2, ui.lbl_head_monster2, buffDisp_Mon2, MaxBuffCount, ui.progressBar_hp_monster2, ui.progressBar_mp_monster2);

	buffDisp_Mon3[0] = ui.lbl_buff_1_monster3;
	buffDisp_Mon3[1] = ui.lbl_buff_2_monster3;
	buffDisp_Mon3[2] = ui.lbl_buff_3_monster3;
	buffDisp_Mon3[3] = ui.lbl_buff_4_monster3;
	monster[2].bingWidget(ui.lbl_name_monster3, ui.lbl_level_monster3, ui.lbl_head_monster3, buffDisp_Mon3, MaxBuffCount, ui.progressBar_hp_monster3, ui.progressBar_mp_monster3);

	setVisible_pet(false);
	setVisible_monster2(false);
	setVisible_monster3(false);
}

void tower_fight::DisplayRoleinfo()
{
	ui.lbl_name_role->setText(player->get_name());
	ui.lbl_level_role->setText(QString("Lv: %1").arg(player->get_lv()));

	QString VocImg = QString(":/mirror/Resources/ui/f_0_%1.png").arg(player->get_voc() + 1);
	ui.lbl_vocation_role->setPixmap(QPixmap(VocImg));

	qint32 headNo = ((player->get_voc() - 1) * 2 + player->get_gender());
	QString headImg = QString(":/mirror/Resources/head/%1.png").arg(headNo);
	ui.lbl_head_role->setPixmap(QPixmap(headImg));

	ui.progressBar_hp_role->setMaximum(player->get_hp_max());
	ui.progressBar_mp_role->setMaximum(player->get_mp_max());

	//从整个技能列表中单独提取出挂机技能，以节约后续调用的效率	
	MapRoleSkill *m_skill = player->get_skill();
	int32_t nTmp = 1;
	for (int32_t i = 0; i < m_skill->size(); i++)
	{
		for (auto iterRole = m_skill->constBegin(); iterRole != m_skill->constEnd(); iterRole++)
		{
			if (iterRole->usdIndex == nTmp)
			{
				fightingSkill.append(skill_fight(g_SkillBasic.value(iterRole->id), iterRole->level));
				nTmp++;
			}
		}
	}
	if (fightingSkill.size() == 0)
	{
		fightingSkill.append(skill_fight(g_SkillBasic.first(), 1));
	}
	basicSkill = skill_fight(g_SkillBasic.first(), 1);
}

void tower_fight::UpdatePetParameter()
{

}

void tower_fight::SummonPet(const skill_fight &skill)
{
	bool bLuck;
	qint32 nDamage = player->GetAttack(player->get_voc(), bLuck);
	pet.ReplaceSoul(skill.no, skill.level, player->get_lv(), nDamage);
	pet.reset_live(time_remain);

	ui.lbl_name_pet->setText(pet.get_name());
	ui.lbl_head_pet->setPixmap(QPixmap::fromImage(pet.get_head()));
	ui.lbl_level_pet->setText(QStringLiteral("Lv:") + QString::number(pet.get_lv()));

	ui.progressBar_hp_pet->setMaximum(pet.get_hp_max());
	ui.progressBar_hp_pet->setValue(pet.get_hp_c());

	ui.progressBar_mp_pet->setMaximum(pet.get_mp_max());
	ui.progressBar_mp_pet->setValue(pet.get_mp_c());
	setVisible_pet(true);
}

void tower_fight::PetDead()
{
	setVisible_pet(false);
}

void tower_fight::setVisible_pet(bool Visible)
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

void tower_fight::setVisible_monster2(bool Visible)
{
	ui.lbl_info_monster2->setVisible(Visible);
	ui.lbl_name_monster2->setVisible(Visible);
	ui.lbl_head_monster2->setVisible(Visible);
	ui.progressBar_hp_monster2->setVisible(Visible);
	ui.progressBar_mp_monster2->setVisible(Visible);
	ui.lbl_level_monster2->setVisible(Visible);
	ui.lbl_vocation_monster2->setVisible(Visible);

	ui.lbl_buff_1_monster2->setVisible(Visible);
	ui.lbl_buff_2_monster2->setVisible(Visible);
	ui.lbl_buff_3_monster2->setVisible(Visible);
	ui.lbl_buff_4_monster2->setVisible(Visible);
}

void tower_fight::setVisible_monster3(bool Visible)
{
	ui.lbl_info_monster3->setVisible(Visible);
	ui.lbl_name_monster3->setVisible(Visible);
	ui.lbl_head_monster3->setVisible(Visible);
	ui.progressBar_hp_monster3->setVisible(Visible);
	ui.progressBar_mp_monster3->setVisible(Visible);
	ui.lbl_level_monster3->setVisible(Visible);
	ui.lbl_vocation_monster3->setVisible(Visible);

	ui.lbl_buff_1_monster3->setVisible(Visible);
	ui.lbl_buff_2_monster3->setVisible(Visible);
	ui.lbl_buff_3_monster3->setVisible(Visible);
	ui.lbl_buff_4_monster3->setVisible(Visible);
}

void tower_fight::GenerateMonster()
{
	monsterCount = 0;
	if (dis.monster1 != 0)
	{
		++monsterCount;
		monster[0].ReplaceSoul(g_MonsterInfo.value(dis.monster1), false);
		ShowMonsterInfo(monster[0], ui.lbl_name_monster1, ui.lbl_level_monster1, ui.lbl_head_monster1, ui.progressBar_hp_monster1, ui.progressBar_mp_monster1);
	}
	if (dis.monster2 != 0)
	{
		++monsterCount;
		monster[1].ReplaceSoul(g_MonsterInfo.value(dis.monster2), false);
		setVisible_monster2(true);
		ShowMonsterInfo(monster[1], ui.lbl_name_monster2, ui.lbl_level_monster2, ui.lbl_head_monster2, ui.progressBar_hp_monster2, ui.progressBar_mp_monster2);
	}
	if (dis.monster3 != 0)
	{
		++monsterCount;
		monster[2].ReplaceSoul(g_MonsterInfo.value(dis.monster3), false);
		setVisible_monster3(true);
		ShowMonsterInfo(monster[2], ui.lbl_name_monster3, ui.lbl_level_monster3, ui.lbl_head_monster3, ui.progressBar_hp_monster3, ui.progressBar_mp_monster3);
	}
}

void tower_fight::ShowMonsterInfo(const CMonster &mon, QLabel *lbl_name, QLabel *lbl_level, QLabel *lbl_head, QProgressBar *pbHP, QProgressBar *pbMP)
{
	lbl_name->setText(mon.get_name());
	lbl_level->setText(QString("Lv : %1").arg(mon.get_lv()));
	lbl_head->setPixmap(QPixmap::fromImage(mon.get_head()));

	pbHP->setMaximum(mon.get_hp_max());
	pbHP->setValue(mon.get_hp_c());
	pbMP->setMaximum(mon.get_mp_max());
	pbMP->setValue(mon.get_mp_c());
}

void tower_fight::DisplayDropSetting()
{
	ui.listWidget->setIconSize(QSize(40, 40));
	ui.listWidget->setGridSize(QSize(40, 40));

	foreach(itemID id, drop[towerLv])
	{
		const Info_Item &item = g_ItemList.value(id);

		QListWidgetItem *WidgetItem = new QListWidgetItem;
		WidgetItem->setIcon(item.icon);
		WidgetItem->setToolTip(item.name);
		ui.listWidget->addItem(WidgetItem);
	}
}

inline QString tower_fight::Generate_Display_LineText(const QString &str1, const QString &skill, const QString &str2, bool bLuck, bool bep, QList<qint32> listDamage)
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


QString tower_fight::Generate_Display_buffInfo(bool bLuck, const QString &SkillName, const realBuff &real)
{
	QString strTmp = QStringLiteral("<font color=DarkCyan>你</font>使用:<font color=gray>%1</font>").arg(SkillName);
	if (bLuck)
		strTmp += QStringLiteral("获得幸运女神赐福,");

	strTmp += QStringLiteral("  效果持续<font color=magenta>") + QString::number(real.time) + QStringLiteral("</font>回合 ");
#ifdef _DEBUG
	strTmp += QString::number(real.rhp) + " " + QString::number(real.damage) + " " + QString::number(real.defense);
#endif // _DEBUG

	return strTmp;
}
void tower_fight::timerEvent(QTimerEvent *event)
{
	//每一次timerEvent为一个回合。 
	//回合时间已用完，判定战斗超时。
	if (time_remain > nTimeOutTime)
	{
		ui.display->append(QStringLiteral("<font color=white>战斗超时，重新寻找怪物。</font>"));
		killTimer(nFightTimer);
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
	else if (!monster[0].wasDead() && time_remain > monster[0].get_live())
	{
		Action_monster(&monster[0]);
	}
	else if (!monster[1].wasDead() && time_remain > monster[1].get_live())
	{
		Action_monster(&monster[1]);
	}
	else if (!monster[2].wasDead() && time_remain > monster[2].get_live())
	{
		Action_monster(&monster[2]);
	}

	//战斗记时
	time_remain += nFightInterval;
}

void tower_fight::Action_monster(CMonster *monster)
{
	monster->update_beforeAction();

	bool bLuck, bAttackPlayer;
	QList<int32_t> ListDamage;
	QString strTmp;

	if (pet.wasDead()) 	{
		bAttackPlayer = true;
	} else {
		bAttackPlayer = 0.65 < (1.0 * qrand() / RAND_MAX);
	}

	if (bAttackPlayer)
	{
		monster->attack(player, bLuck, &ListDamage);
		ui.display->append(Generate_Display_LineText(
			QStringLiteral("<font color = darkRed>%1</font>").arg(monster->get_name()),
			monster->getVirtualSkillName(),
			QStringLiteral("<font color=DarkCyan>%1</font>").arg(player->get_name()),
			bLuck, false, ListDamage));

		if (player->wasDead())
		{
			killTimer(nFightTimer);

			//角色死亡，损失声望 = 塔层 * 100
			int32_t nDropRep = towerLv * 100;
			player->sub_rep(nDropRep);

			ui.display->append(QStringLiteral("<font color=white>战斗失败!</font>"));
			ui.display->append(QStringLiteral("损失声望：") + QString::number(nDropRep));
		}
	}
	else
	{
		monster->attack(&pet, bLuck, &ListDamage);
		ui.display->append(Generate_Display_LineText(
			QStringLiteral("<font color = darkRed>%1</font>").arg(monster->get_name()),
			monster->getVirtualSkillName(),
			QStringLiteral("<font color=DarkCyan>%1</font>").arg(pet.get_name()),
			bLuck, false, ListDamage));

		if (pet.wasDead())
		{
			PetDead();
		}
	}
}

void tower_fight::Action_role(void)
{
	player->update_beforeAction();

	//确定要打哪个monster，其后pet与角色攻击同一对象。
	whichMonster = 1.0 * monsterCount * qrand() / RAND_MAX;

	Step_role_Skill();
	updateSkillCD();
}

void tower_fight::Step_role_Skill(void)
{
	bool bUsedSkill = false;
	qint32 nTmp;
	skill_fight skill;

	for (qint32 i = 0; i < fightingSkill.size(); i++)
	{
		nTmp = nSkillIndex;
		skill = fightingSkill.at(nSkillIndex++);
		if (nSkillIndex >= fightingSkill.size())
		{
			nSkillIndex = 0;
		}

		if (skill.cd_c > 0)
		{
			continue;
		}

		if (player->get_mp_c() <= 0)
		{
			QString strTmp = QStringLiteral("<font color=red>法力不足，无法施放技能，切换为普通攻击.</font>");
			ui.display->append(strTmp);

			skill = basicSkill;
		}

		switch (skill.type)
		{
		case 1: bUsedSkill = MStep_role_Attack(skill); break;
		case 2: bUsedSkill = MStep_role_Buff(skill); break;
		case 3: bUsedSkill = MStet_role_Debuff(skill); break;
		case 4: SummonPet(skill); bUsedSkill = true; break;
		default: bUsedSkill = true; break;
		}

		if (bUsedSkill)
		{
			break;
		}
	}

	if (!bUsedSkill)
	{
		QString strTmp = QStringLiteral("<font color=red>无可用技能，切换为普通攻击.</font>");
		ui.display->append(strTmp);
		MStep_role_Attack(basicSkill);
	}
	fightingSkill[nTmp].cd_c = fightingSkill[nTmp].cd;
	player->set_mp_c(player->get_mp_c() - skill.spell);
}

bool tower_fight::MStep_role_Attack(const skill_fight &skill)
{
	qint32 nDamage, nTmp, nTmp1, nTmp2, m_ac, m_mac;
	bool bTmp, bep = false, bLuck = false;
	QList<qint32> ListDamage;

	player->attack(&monster[whichMonster], skill.id, skill.level, bLuck, &ListDamage);

	ui.display->append(Generate_Display_LineText(
		QStringLiteral("<font color=DarkCyan>你</font>"),
		g_SkillBasic.value(skill.id).name,
		QStringLiteral("<font color=DarkRed>%1</font>").arg(monster[whichMonster].get_name()),
		bLuck, bep, ListDamage));

	if (monster[whichMonster].wasDead())
	{
		MonsterDead();
	}

	return true;
}

bool tower_fight::MStep_role_Buff(const skill_fight &skill)
{
	quint32 nTmp, nTmp1;
	realBuff real;
	bool bLuck = false;

	if (player->HasBuff(skill.no) && pet.HasBuff(skill.no))
	{
		return false;	//no need to used skill
	}

	if (!Init_realBuff(skill, bLuck, real))
	{
		//has error, can't use the skill
		LogIns.append(LEVEL_ERROR, __FUNCTION__, mirErr_para);
		return false;
	}

	if (real.rhp > 0 &&
		0.8 < 1.0 * player->get_hp_c() / player->get_hp_max() &&
		(pet.wasDead() || 0.8 < 1.0 * pet.get_hp_c() / pet.get_hp_max()))
	{
		return false;				//若自身以及宠物血量大于80%，不使用恢复类buff。
	}
	player->appendBuff(real);
	pet.appendBuff(real);

	ui.display->append(Generate_Display_buffInfo(bLuck, skill.name, real));
	return true;
}

bool tower_fight::MStet_role_Debuff(const skill_fight &skill)
{
	quint32 nTmp, nTmp1;
	realBuff real;
	bool bLuck = false;

	if (monster[whichMonster].HasBuff(skill.no))
	{
		return false;	//no need to used skill
	}

	if (!Init_realBuff(skill, bLuck, real))
	{
		//has error, can't use the skill
		LogIns.append(LEVEL_ERROR, __FUNCTION__, mirErr_para);
		return false;
	}

	monster[whichMonster].appendBuff(real);
	ui.display->append(Generate_Display_buffInfo(bLuck, skill.name, real));
	return true;
}

bool tower_fight::Init_realBuff(const skill_fight &skill, bool &bLuck, realBuff &real)
{
	bool res = false;
	qint32 nTmp;

	int32_t flag;
	if (skill.type == 2) {
		flag = 1;
	} else {
		flag = -1;
	}

	const Info_SkillBuff &sb = g_SkillBuff.value(skill.no);

	nTmp = player->GetAttack(player->get_voc(), bLuck);

	real.id = skill.no;
	real.name = skill.name;
	real.icon = skill.icon;
	real.time = nTmp * sb.time * skill.level / 100;
	real.rhp = flag * nTmp * sb.rhp / 100;
	real.damage = flag * nTmp * sb.damage / 100;
	real.defense = flag * nTmp * sb.defense / 100;

	real.speed = player->get_intervel() * sb.speed / 100;

	res = true;

	return res;
}


void tower_fight::Action_pet(void)
{
	pet.update_beforeAction();

	bool bLuck, bAttackPlayer;
	QList<int32_t> ListDamage;
	QString strTmp;

	pet.M_attack(&monster[whichMonster], bLuck, &ListDamage);
	ui.display->append(Generate_Display_LineText(
		QStringLiteral("<font color=DarkCyan>%1</font>").arg(pet.get_name()),
		pet.get_skill().name,
		QStringLiteral("<font color=DarkRed>%1</font>").arg(monster[whichMonster].get_name()),
		bLuck, false, ListDamage));


	if (monster[whichMonster].wasDead())
	{
		MonsterDead();
	}
}

void tower_fight::MonsterDead()
{
	killTimer(nFightTimer);
	bSuccess = true;

	quint32 nTmp, nDropExp;
	QString strTmp;

	nTmp = towerLv + 1;
	nDropExp = nTmp * nTmp * 500;
	if (player->get_lv() < MaxLevel)	{
		player->add_exp(nDropExp);
	}
	strTmp = QStringLiteral("<font color=white>挑战成功，获得经验:</font> <font color=green>%1</font>").arg(nDropExp);
	ui.display->append(strTmp);

	quint64 lvExp = g_JobAddSet[player->get_lv()].exp;
	if (player->get_exp() > lvExp)
	{
		player->levelUp();
		ui.display->append(QStringLiteral("<font color=white>升级了. </font>"));		
	}

	const QVector<itemID> items = drop[towerLv];
	strTmp = QStringLiteral("<font color=white>获得通关奖励:");
	for (int i = 0; i < 3; i++)
	{
		qint32 index = 1.0 * qrand() / RAND_MAX * items.size();
		const Info_Item &item = g_ItemList.value(items[index]);
		if (item.ID != 0)
		{
			m_bag_item->insert(item.ID, m_bag_item->value(item.ID) + 1);
			strTmp += item.name + " ";	
		}
	}
	strTmp += QStringLiteral("</font");
	ui.display->append(strTmp);
}

void tower_fight::updateSkillCD()
{
	for (int i = 0; i < fightingSkill.size(); i++)
	{
		--fightingSkill[i].cd_c;
	}
}