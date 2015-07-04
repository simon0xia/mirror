#include "fight_fight.h"
#include <QFile>
#include <QMessageBox>
#include <QThread>
#include <time.h>
#include <QKeyEvent>

#include "Item_Base.h"
#include "def_System_para.h"
#include "mirrorlog.h"

//定义并初始化静态数据成员。
bool fight_fight::bCheckHp = false;
bool fight_fight::bCheckMp = false;
bool fight_fight::bCheckConcise = false;
bool fight_fight::bCheckFindBoss = false;
qint32 fight_fight::pickFilter = 0;
qint32 fight_fight::limit_rhp = 50;
qint32 fight_fight::limit_rmp = 50;

extern RoleInfo_False g_falseRole;

extern vecBuff g_buffList;
extern QVector<Info_skill> g_skillList;
extern QMap<itemID, Info_Item> g_ItemList;
extern QMap<mapID, Info_Distribute> g_MonsterDistribute;
extern QVector<MonsterInfo> g_MonsterNormal_List;
extern QVector<MonsterInfo> g_MonsterBoss_list;
extern mapDrop	g_mapDropSet;

fight_fight::fight_fight(QWidget* parent, qint32 id, RoleInfo *info, MapItem *bag_item, ListEquip *bag_equip)
	: QDialog(parent), m_MainFrame(parent), m_mapID(id), myRole(info), m_bag_item(bag_item), m_bag_equip(bag_equip)
{
	ui.setupUi(this);
	InitUI();

	Role_Lvl = (myRole->level >> 1) - 1;

	Cacl_Display_Role_Value();
	LoadItem();

	AssignMonster(g_MonsterNormal_List, g_MonsterBoss_list, g_MonsterDistribute);
	monster_cur = &g_MonsterNormal_List[monster_normal_assign[0]];
	Display_CurrentMonsterInfo();

	bFighting = false;
	bCheckHp = bCheckMp = true;
	nShowStatusRound = 0;
	nSkillIndex = 0;
	m_dlg_fightInfo = nullptr;

	nFightTimer = startTimer(nFightInterval);
	Time_fight.start();
	nCount_normalMonster = nCount_boss = nCount_exp = nCount_coin = nCount_rep = 0;
	nElapse_pre_boss = 0;
	nCount_fail = nCount_timeout = 0;

	nXSpeedTimer = startTimer(nXSpeedInvterval);
	xSpeedTime.start();
	nXSpeedCount = 0;

	connect(ui.comboBox_filter, SIGNAL(currentIndexChanged(int)), this, SLOT(pickFilterChange(int)));
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
	limit_rhp = ui.edit_hp->text().toInt();
	limit_rmp = ui.edit_mp->text().toInt();
	close();
}

void fight_fight::on_btn_statistics_clicked(void)
{
	if (m_dlg_fightInfo == nullptr)
	{
		m_dlg_fightInfo = new fight_info(this);
	}
	QPoint pos = QPoint(730, 370);// mapFromGlobal(cursor().pos()) + QPoint(20, 0);
	m_dlg_fightInfo->move(pos);

	qint32 time = Time_fight.elapsed() / 60000;
	m_dlg_fightInfo->updateInfo(time, nCount_fail, nCount_timeout, nCount_normalMonster, nCount_boss, nCount_exp, nCount_coin, nCount_rep);
	m_dlg_fightInfo->show();
}
void fight_fight::pickFilterChange(int index)
{
	pickFilter = index * 2 - 1;
	if (pickFilter < 0)
	{
		pickFilter = 0;
	}
}

void fight_fight::InitUI()
{
	bCheckAuto = false;

	ui.progressBar_monster_hp->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
	ui.progressBar_monster_mp->setStyleSheet("QProgressBar::chunk { background-color: rgb(0, 0, 255) }");
	ui.edit_monster_sc->setText("0 - 0");
	ui.edit_monster_rmp->setText("0");

	ui.edit_hp->setText(QString::number(limit_rhp));
	ui.edit_mp->setText(QString::number(limit_rmp));
 	ui.checkBox_concise->setChecked(bCheckConcise);
	ui.checkBox_boss->setChecked(bCheckFindBoss);
	ui.comboBox_filter->setCurrentIndex((pickFilter + 1) / 2);

	ui.label_role_head->setAttribute(Qt::WA_TranslucentBackground, true);
	ui.edit_role_name->setAttribute(Qt::WA_TranslucentBackground, true);
	ui.edit_role_level->setAttribute(Qt::WA_TranslucentBackground, true);
	ui.edit_role_vocation->setAttribute(Qt::WA_TranslucentBackground, true);

	ui.label_monster_head->setAttribute(Qt::WA_TranslucentBackground, true);
	ui.edit_monster_name->setAttribute(Qt::WA_TranslucentBackground, true);
	ui.edit_monster_level->setAttribute(Qt::WA_TranslucentBackground, true);
	ui.edit_monster_vocation->setAttribute(Qt::WA_TranslucentBackground, true);

	buffDisp_Role[0] = ui.lbl_role_buff_0;
	buffDisp_Role[1] = ui.lbl_role_buff_1;
	buffDisp_Role[2] = ui.lbl_role_buff_2;
	buffDisp_Mon[0] = ui.lbl_monster_buff_0;
	buffDisp_Mon[1] = ui.lbl_monster_buff_1;
	buffDisp_Mon[2] = ui.lbl_monster_buff_2;

	for (qint32 i = 0; i < MaxBuffCount; i++)
	{
		buffDisp_Role[i]->setText("");
		buffDisp_Mon[i]->setText("");
	}
}

void fight_fight::Cacl_Display_Role_Value()
{
	quint64 role_exp = (myRole->exp >> 1) - 1;
	ui.edit_role_name->setText(myRole->name);
	ui.edit_role_level->setText(QStringLiteral("Lv:") + QString::number(Role_Lvl));

	QString def_vocation[] = { QStringLiteral("无"), QStringLiteral("战"), QStringLiteral("法"), QStringLiteral("道") };
	ui.edit_role_vocation->setText(def_vocation[myRole->vocation]);

	ui.progressBar_role_exp->setMaximum(myRole->lvExp);
	if (role_exp>= ui.progressBar_role_exp->maximum())
		ui.progressBar_role_exp->setValue(ui.progressBar_role_exp->maximum());
	else
		ui.progressBar_role_exp->setValue(role_exp);

	quint32 nTmp1, nTmp2;
	nTmp1 = myRole->intervel_1 << 8 | myRole->intervel_2;
	ui.edit_role_interval->setText(QString::number(nTmp1));

	nTmp1 = myRole->dc1_1 << 24 | myRole->dc1_2 << 16 | myRole->dc1_3 << 8 | myRole->dc1_4;
	nTmp2 = myRole->dc2_1 << 24 | myRole->dc2_2 << 16 | myRole->dc2_3 << 8 | myRole->dc2_4;
	ui.edit_role_dc->setText(QString("%1-%2").arg(nTmp1).arg(nTmp2));

	nTmp1 = myRole->mc1_1 << 24 | myRole->mc1_2 << 16 | myRole->mc1_3 << 8 | myRole->mc1_4;
	nTmp2 = myRole->mc2_1 << 24 | myRole->mc2_2 << 16 | myRole->mc2_3 << 8 | myRole->mc2_4;
	ui.edit_role_mc->setText(QString("%1-%2").arg(nTmp1).arg(nTmp2));

	nTmp1 = myRole->sc1_1 << 24 | myRole->sc1_2 << 16 | myRole->sc1_3 << 8 | myRole->sc1_4;
	nTmp2 = myRole->sc2_1 << 24 | myRole->sc2_2 << 16 | myRole->sc2_3 << 8 | myRole->sc2_4;
	ui.edit_role_sc->setText(QString("%1-%2").arg(nTmp1).arg(nTmp2));

	nTmp1 = myRole->ac1_1 << 24 | myRole->ac1_2 << 16 | myRole->ac1_3 << 8 | myRole->ac1_4;
	nTmp2 = myRole->ac2_1 << 24 | myRole->ac2_2 << 16 | myRole->ac2_3 << 8 | myRole->ac2_4;
	ui.edit_role_ac->setText(QString("%1-%2").arg(nTmp1).arg(nTmp2));
	role_ac1 = nTmp1;
	role_ac2 = nTmp2;

	nTmp1 = myRole->mac1_1 << 24 | myRole->mac1_2 << 16 | myRole->mac1_3 << 8 | myRole->mac1_4;
	nTmp2 = myRole->mac2_1 << 24 | myRole->mac2_2 << 16 | myRole->mac2_3 << 8 | myRole->mac2_4;
	ui.edit_role_mac->setText(QString("%1-%2").arg(nTmp1).arg(nTmp2));
	role_mac1 = nTmp1;
	role_mac2 = nTmp2;

	nTmp1 = myRole->hp_1 << 24 | myRole->hp_2 << 16 | myRole->hp_3 << 8 | myRole->hp_4;
	ui.progressBar_role_hp->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
	ui.progressBar_role_hp->setMaximum(nTmp1);
	ui.progressBar_role_hp->setValue(nTmp1);
	role_hp_2c = (nTmp1 + 1 ) << 1;

	nTmp1 = myRole->mp_1 << 24 | myRole->mp_2 << 16 | myRole->mp_3 << 8 | myRole->mp_4;
	ui.progressBar_role_mp->setStyleSheet("QProgressBar::chunk { background-color: rgb(0, 0, 255) }");
	ui.progressBar_role_mp->setMaximum(nTmp1);
	ui.progressBar_role_mp->setValue(nTmp1);
	role_mp_c = nTmp1;

	role_rhp = 0;
	ui.edit_role_rhp->setText(QString::number(role_rhp));

	role_rmp = 0;
	ui.edit_role_rmp->setText(QString::number(role_rmp));

	qint32 headNo = ((myRole->vocation - 1) * 2 + myRole->gender);
	QString headImg = QString(":/mirror/Resources/head/%1.png").arg(headNo);
	ui.label_role_head->setPixmap(QPixmap(headImg));

	//从整个技能列表中单独提取出挂机技能，以节约后续调用的效率
	skill_fight skill;
	for (VecRoleSkill::const_iterator iterRole = myRole->skill.begin(); iterRole != myRole->skill.end(); iterRole++)
	{
		for (QVector<Info_skill>::iterator iterSkill = g_skillList.begin(); iterSkill != g_skillList.end(); iterSkill++)
		{
			if (iterRole->id == iterSkill->ID)
			{
				skill.id = iterSkill->ID;
				skill.name = iterSkill->name;
				skill.icon = iterSkill->icon;
				skill.type = iterSkill->type;
				skill.level = iterRole->level;
				skill.spell = iterSkill->spell[skill.level - 1];
				skill.cd = iterSkill->cd;
				skill.cd_c = 0;
				skill.times = iterSkill->times;
				skill.damage = iterSkill->damage[skill.level - 1];
				skill.buff = iterSkill->buff;
				fightingSkill.append(skill);
			}
		}
	}
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
	return NULL;
}

void fight_fight::LoadItem()
{
	QString strTmp;
	for (MapItem::iterator iter = m_bag_item->begin(); iter != m_bag_item->end(); iter++)
	{
		const Info_Item *itemItem = FindItem(iter.key());
		if (itemItem != nullptr && itemItem->level <= Role_Lvl)
		{
			if (itemItem->type == et_immediate_hp)
			{
				strTmp = Generate_ItemComboBox_Text(itemItem->name, QStringLiteral("血"), itemItem->value, iter.value());
				ui.comboBox_hp->addItem(strTmp);
			}
			else if (itemItem->type == et_immediate_mp)
			{
				strTmp = Generate_ItemComboBox_Text(itemItem->name, QStringLiteral("魔"), itemItem->value, iter.value());;
				ui.comboBox_mp->addItem(strTmp);
			}
		}
	}
	ui.comboBox_hp->setCurrentIndex(ui.comboBox_hp->count() - 1);
	ui.comboBox_mp->setCurrentIndex(ui.comboBox_mp->count() - 1);
}

bool fight_fight::AssignMonster(QVector<MonsterInfo> normalList, QVector<MonsterInfo> bossList, QMap<mapID, Info_Distribute> Distribute)
{
	quint32 c;
	memset(monster_normal_assign, 0, Max_monster * sizeof(quint32));
	memset(monster_boss_assign, 0, Max_monster * sizeof(quint32));

	const Info_Distribute &dis = Distribute[m_mapID];

	c = 0;
	foreach(quint32 n, dis.normal)
	{
		monster_normal_assign[c++] = n;
	}
	monster_normal_count = c;

	c = 0;
	foreach(quint32 n, dis.boss)
	{
		monster_boss_assign[c++] = n;
	}
	if (monster_boss_assign[0] == 0)
	{
		monster_boss_count = 0;			//有些地图不刷新BOSS
	}
	else
	{
		monster_boss_count = c;
	}

	//将怪物ID转化为其在总怪物列表中的索引序号，以方便后续加载。
	c = 0;
	for (quint32 i = 0; i < normalList.size() && c < monster_normal_count; i++)
	{
		if (monster_normal_assign[c] == normalList[i].ID)
		{
			monster_normal_assign[c++] = i;
		}
	}
	c = 0;
	for (quint32 i = 0; i < bossList.size() && c < monster_boss_count; i++)
	{
		if (monster_boss_assign[c] == bossList[i].ID)
		{
			monster_boss_assign[c++] = i;
		}
	}
	return true;
}

void fight_fight::Display_CurrentMonsterInfo()
{
	//设置体、魔最大值。
	ui.progressBar_monster_hp->setMaximum(monster_cur->hp);
	ui.progressBar_monster_mp->setMaximum(monster_cur->mp);
	//显示当前体、魔
	monster_cur_hp = monster_cur->hp;
//	monster_cur_mp = monster_cur->mp;
	ui.progressBar_monster_hp->setValue(monster_cur_hp);
	ui.progressBar_monster_mp->setValue(monster_cur->mp);
	
	if (m_mapID < 1000)
	{	//只有普通地图的怪有回血功能。
		monster_cur_rhp = monster_cur_hp >> 7;
	}
	else
	{
		monster_cur_rhp = 0;
	}
		
	ui.edit_monster_rhp->setText(QString::number(monster_cur_rhp));

	//加载头像
	ui.label_monster_head->setPixmap(QPixmap::fromImage(monster_cur->Head));

	//加载其他属性
	monster_cur_ac = monster_cur->AC;
	monster_cur_mac = monster_cur->MAC;
	ui.edit_monster_name->setText(monster_cur->name);
	ui.edit_monster_level->setText(QStringLiteral("Lv:") + QString::number(monster_cur->level));
	ui.edit_monster_dc->setText(QString::number(monster_cur->DC1) + " - " + QString::number(monster_cur->DC2));
	ui.edit_monster_mc->setText(QString::number(monster_cur->MC1) + " - " + QString::number(monster_cur->MC2));
	ui.edit_monster_ac->setText(QString::number(monster_cur_ac));
	ui.edit_monster_mac->setText(QString::number(monster_cur_mac));
	ui.edit_monster_interval->setText(QString::number(monster_cur->interval));


}

inline QString fight_fight::Generate_ItemComboBox_Text(const QString &name, const QString &type, quint32 value, quint32 count)
{
	QString strSplit = QStringLiteral("%1 %2:%3 剩:%4").arg(name).arg(type).arg(value).arg(count);
	return strSplit;
}
inline QString fight_fight::Generate_Display_LineText(const QString &str1, const QString &skill, const QString &str2, bool bLuck, bool bep, QList<qint32> listDamage)
{
	QString strTmp = QStringLiteral("<font color=DarkCyan>%1</font>使用<font color=gray>%2</font>，").arg(str1).arg(skill);
	if (bLuck)
		strTmp += QStringLiteral("获得战神祝福,");

	strTmp += QStringLiteral("对<font color = DarkCyan>%1</font>").arg(str2);
	
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

void fight_fight::Step_role_UsingItem_hp(void)
{
	quint32 ID, nTmp1;

	QString strTmp = ui.comboBox_hp->currentText();
	QStringList strList = strTmp.split(" ");

	const Info_Item *itemItem = FindItem(strList.at(0));
	if (itemItem != nullptr)
	{
		ID = itemItem->ID;
		//背包对应道具数量减1
		m_bag_item->insert(ID, m_bag_item->value(ID) - 1); 
		strTmp = Generate_ItemComboBox_Text(itemItem->name, QStringLiteral("血"), itemItem->value, m_bag_item->value(ID));
		ui.comboBox_hp->setItemText(ui.comboBox_hp->currentIndex(), strTmp);

		//更改角色状态
		nTmp1 = myRole->hp_1 << 24 | myRole->hp_2 << 16 | myRole->hp_3 << 8 | myRole->hp_4;
		nTmp1 = (nTmp1 + 1) << 1;
		role_hp_2c += itemItem->value << 1;
		if (role_hp_2c >= nTmp1)
		{
			role_hp_2c = nTmp1;
		}
		ui.progressBar_role_hp->setValue((role_hp_2c >> 1) - 1);
		if (!bCheckConcise)
		{
			strTmp = QStringLiteral("<font color=green>你使用了：") + itemItem->name + QStringLiteral("</font>");
			ui.edit_display->append(strTmp);
		}

		//如果道具已经用完，则删除当前道具.如果还有道具，则切换到0号道具，否则清除自动补血复选。
		if (m_bag_item->value(ID) <= 0)
		{
			m_bag_item->remove(ID);
			ui.comboBox_hp->removeItem(ui.comboBox_hp->currentIndex());
			if (ui.comboBox_hp->count() > 0)
			{
				ui.comboBox_hp->setCurrentIndex(0);
			}
			else
			{
				ui.checkBox_hp->setChecked(false);
				bCheckHp = false;
			}
		}
	}
	else
	{	//找不到对应道具，清除自动补血复选。
		ui.checkBox_hp->setCheckState(Qt::Unchecked);
		bCheckHp = false;
	}
}
void fight_fight::Step_role_UsingItem_mp(void)
{
	quint32 ID, nTmp1;
	QString strTmp = ui.comboBox_mp->currentText();
	QStringList strList = strTmp.split(" ");

	const Info_Item *itemItem = FindItem(strList.at(0));
	if (itemItem != NULL)
	{
		ID = itemItem->ID;
		//背包对应道具数量减1
		m_bag_item->insert(ID, m_bag_item->value(ID) - 1);
		strTmp = Generate_ItemComboBox_Text(itemItem->name, QStringLiteral("魔"), itemItem->value, m_bag_item->value(ID));
		ui.comboBox_mp->setItemText(ui.comboBox_mp->currentIndex(), strTmp);

		//更改角色状态
		nTmp1 = myRole->mp_1 << 24 | myRole->mp_2 << 16 | myRole->mp_3 << 8 | myRole->mp_4;
		role_mp_c += itemItem->value;
		if (role_mp_c >= nTmp1)
		{
			role_mp_c = nTmp1;
		}
		ui.progressBar_role_mp->setValue(role_mp_c);
		if (!bCheckConcise)
		{
			strTmp = QStringLiteral("<font color=green>你使用了：") + itemItem->name + QStringLiteral("</font>");
			ui.edit_display->append(strTmp);
		}
		//如果道具已经用完，则删除当前道具.如果还有道具，则切换到0号道具，否则清除自动补血复选。
		if (m_bag_item->value(ID) <= 0)
		{
			m_bag_item->remove(ID);
			ui.comboBox_mp->removeItem(ui.comboBox_mp->currentIndex());
			if (ui.comboBox_mp->count() > 0)
			{
				ui.comboBox_mp->setCurrentIndex(0);
			}
			else
			{
				ui.checkBox_mp->setChecked(false);
				bCheckMp = false;
			}		
		}
	}
	else
	{	//找不到对应道具，清除复选。
		ui.checkBox_mp->setCheckState(Qt::Unchecked);
		bCheckMp = false;
	}
}

inline quint32 fight_fight::GetRoleATK(qint32 type, bool &bLuck)
{
	quint32 nA, Min, Max, nTmp3;
	double dTmp;

	Min = 0;
	Max = 1;
	if (type == 1)
	{
		Min = myRole->dc1_1 << 24 | myRole->dc2_1 << 16 | myRole->dc1_3 << 8 | myRole->dc1_4;
		Max = myRole->dc2_1 << 24 | myRole->dc2_2 << 16 | myRole->dc2_3 << 8 | myRole->dc2_4;
	}
	else if (type == 2)
	{
		Min = myRole->mc1_1 << 24 | myRole->mc1_2 << 16 | myRole->mc1_3 << 8 | myRole->mc1_4;
		Max = myRole->mc2_1 << 24 | myRole->mc2_2 << 16 | myRole->mc2_3 << 8 | myRole->mc2_4;
	}
	else if (type == 3)
	{
		Min = myRole->sc1_1 << 24 | myRole->sc1_2 << 16 | myRole->sc1_3 << 8 | myRole->sc1_4;
		Max = myRole->sc2_1 << 24 | myRole->sc2_2 << 16 | myRole->sc2_3 << 8 | myRole->sc2_4;
	}

	nA = Min + qrand() % (Max - Min + 1);

	//发挥幸运
	dTmp = 10.0 * qrand() / RAND_MAX;
	nTmp3 = myRole->luck_1 << 8 | myRole->luck_2;
	if (dTmp < nTmp3)
	{
		nA = Max;
		bLuck = true;
	}
	
	return nA;
}

void fight_fight::Step_role_Skill(void)
{
	++nCount_attack;
	bool bUsedSkill = false;
	qint32 spell, nTmp;

	for (qint32 i = 0; i < fightingSkill.size(); i++)
	{
		nTmp = nSkillIndex;
		const skill_fight &skill = fightingSkill.at(nSkillIndex++);
		if (nSkillIndex >= fightingSkill.size())
		{
			nSkillIndex = 0;
		}

		if (role_mp_c < skill.spell)
		{
			QString strTmp = QStringLiteral("<font color=red>魔法不足，无法施放技能.</font>");
			ui.edit_display->append(strTmp);
			return;
		}
		spell = skill.spell;

		if (skill.cd_c <= 0)
		{
			if (skill.buff > 0)
			{
				if (m_mapID > 1000 && m_mapID < 2000 && skill.buff > 100)
				{
					QString strTmp = QStringLiteral("<font color=red>怪物拥有魔神守护.%1无效</font>").arg(skill.name);
					ui.edit_display->append(strTmp);
					bUsedSkill = true;
				}
				else
				{
					bUsedSkill = MStep_role_Buff(skill);
				}			
			}

			if (skill.times > 0)
			{
				bUsedSkill = MStep_role_Attack(skill);
			}
		}
		if (bUsedSkill)
		{
			fightingSkill[nTmp].cd_c = fightingSkill[nTmp].cd;
			role_mp_c -= spell;
			ui.progressBar_role_mp->setValue(role_mp_c);
			break;
		}
	}
	if (!bUsedSkill)
	{
		ui.edit_display->append(QStringLiteral("无可用技能"));
	}
}
bool fight_fight::MStep_role_Buff(const skill_fight &skill)
{
	quint32 nTmp, nTmp1;
	bool bLuck = false;
	if (skill.buff > 100)
	{
		foreach(const realBuff &real, buffInMonster)
		{
			if (real.id == skill.id)
			{//怪物身上已有此buff，无须再次施放技能。
				return false;
			}
		}
	}
	else
	{ 
		foreach(const realBuff &real, buffInRole)
		{
			if (real.id == skill.id)
			{//角色身上已有此buff，无须再次施放技能。
				return false;
			}
		}
	}

	info_buff *buff = nullptr;
	for (quint32 i = 0; i < g_buffList.size(); i++)
	{
		if (g_buffList[i].ID == skill.buff)
		{
			buff = &g_buffList[i];
			break;
		}
	}

	if (buff != nullptr)
	{
		quint32 nA = GetRoleATK(skill.type, bLuck);
		realBuff real;
		real.id = skill.id;
		real.name = skill.name;
		real.icon = skill.icon;
		real.time = nA * buff->time / 100 + 2;
		real.rhp = nA * buff->rhp * skill.level / 100;
		real.ac = nA * buff->ac * skill.level / 100;
		real.mac = nA * buff->mac * skill.level / 100;
		if (skill.buff < 100)
		{//自身增益buff
			nTmp1 = myRole->hp_1 << 24 | myRole->hp_2 << 16 | myRole->hp_3 << 8 | myRole->hp_4;
			nTmp = (role_hp_2c >> 1) - 1;
			if (real.rhp > 0 && 0.8 < 1.0 * nTmp / nTmp1)
			{
				return false;				//若自身血量大于80%，不使用恢复类buff。
			}
			buffInRole.append(real);
			buffDisp_Role[buffInRole.size() -1 ]->setPixmap(real.icon);
		}
		else
		{//对方减益buff
			buffInMonster.append(real);
			buffDisp_Mon[buffInMonster.size() - 1]->setPixmap(real.icon);
		}
		if (!bCheckConcise)
		{
			QString strTmp = QStringLiteral("<font color=DarkCyan>你</font>使用:<font color=gray>%1</font>").arg(skill.name);
			if (bLuck)
				strTmp += QStringLiteral("获得幸运女神赐福,");

			strTmp += QStringLiteral("  效果持续<font color=magenta>") + QString::number(real.time) + QStringLiteral("</font>回合 ");
#ifdef _DEBUG
			strTmp += QString::number(real.rhp) + " " + QString::number(real.ac) + " " + QString::number(real.mac);
#endif // _DEBUG
		
			ui.edit_display->append(strTmp);
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool fight_fight::MStep_role_Attack(const skill_fight &skill)
{
	qint32 nDamage, nTmp, nTmp1, nTmp2;
	bool bep, bLuck = false;
	QList<qint32> ListDamage;

	nTmp1 = myRole->ep_1 << 24 | myRole->ep_2 << 16 | myRole->ep_3 << 8 | myRole->ep_4;
	nTmp2 = myRole->ed_1 << 24 | myRole->ed_2 << 16 | myRole->ed_3 << 8 | myRole->ed_4;
	bep = nTmp1 > (qrand() % 10000);

	for (qint32 i = 0; i < skill.times; i++)
	{
		quint32 nA = GetRoleATK(skill.type, bLuck);
		if (skill.type == 2 || skill.type == 3)
		{
			nTmp = nA * skill.damage / 100;
			nDamage = (nTmp - monster_cur_mac);
		}
		else
		{
			//不为魔法、道术的一概念视为物理攻击。
			nTmp = nA * skill.damage / 100;
			nDamage = (nTmp - monster_cur_ac);
		}
		if (bep)
		{	//暴击
			nDamage += nTmp2;
		}
		nDamage = (nDamage < 1 ? 1 : nDamage);
		ListDamage.append(nDamage);

		nTmp = monster_cur_hp - nDamage;
		monster_cur_hp = nTmp < 0 ? 0 : nTmp;
		ui.progressBar_monster_hp->setValue(monster_cur_hp);	
	}
	if (!bCheckConcise)
	{
		ui.edit_display->append(Generate_Display_LineText(QStringLiteral("你"), skill.name, monster_cur->name, bLuck, bep, ListDamage));
	}
	//更改角色状态
	nTmp = role_hp_2c + (role_rhp << 1);
	nTmp1 = myRole->hp_1 << 24 | myRole->hp_2 << 16 | myRole->hp_3 << 8 | myRole->hp_4;
	nTmp1 = (nTmp1 + 1) << 1;
	role_hp_2c = nTmp > nTmp1 ? nTmp1 : nTmp;
	ui.progressBar_role_hp->setValue((role_hp_2c >> 1) -1);
	return true;
}
inline void fight_fight::DisplayDropBasic(quint32 nDropExp, quint32 nDropCoin, quint32 nDropRep)
{
	ui.edit_display->append(QStringLiteral("<font color=white>获得经验:") + QString::number(nDropExp) + QStringLiteral("</font>"));
	ui.edit_display->append(QStringLiteral("<font color=white>获得金币:") + QString::number(nDropCoin) + QStringLiteral("</font>"));
	if (bBoss)
	{
		ui.edit_display->append(QStringLiteral("<font color=white>获得声望:") + QString::number(nDropRep) + QStringLiteral("</font>"));
	}
}

void fight_fight::CreateEquip(itemID id, Info_Equip &DropEquip)
{
	//极品0--8点出现的概率
	double probability[9] = { 0, 0.4096, 0.2048, 0.0512, 0.0128, 0.0032, 0.0016, 0.004, 0.0004 };
	double dTmp = 1.0 * qrand() / RAND_MAX;
	qint32 extraAmount = 0;
	for (int i = 8; i > 0; i--)
	{
		if (dTmp > 1 - probability[i])
		{
			extraAmount = i;
			break;
		}
	}

	const Info_basic_equip *EquipBasicInfo = Item_Base::GetEquipBasicInfo(id);
	EquipExtra extra = { 0 };
	quint32 *p, extraPara = sizeof(EquipExtra) / sizeof(quint32);
	quint32 index, nCount, type;

	//此处强制转换是为了随机化实现极品装备的属性点位置及大小。操作需慎重。
	p = (quint32 *)&extra;

	//分配点数到具体的属性上面。
	while (extraAmount > 0)
	{
		index = qrand() % extraPara;
		nCount = qrand() % g_specialEquip_MaxExtra;

		p[index] = (extraAmount < nCount) ? extraAmount : nCount;
		extraAmount -= p[index];
	}
	//初始化
	DropEquip = { 0 };
	DropEquip.ID = id;
	DropEquip.lvUp = 0;
	DropEquip.extra = extra;

	type = (DropEquip.ID - g_itemID_start_equip) / 1000;
	//所有物品皆不允许有准确加成
	DropEquip.extra.acc = 0;
	//所有装备皆不允许有幸运加成,并且幸运范围只有0-3。
	if (DropEquip.extra.luck > 0)
	{	//fix 暂时先写死，以后必须在数据库中配置。
		if (type == g_equipType_necklace && (DropEquip.ID == 305006 || DropEquip.ID == 305007 || DropEquip.ID == 305016))
			DropEquip.extra.luck /= 3;
		else
			DropEquip.extra.luck = 0;
	}
	if (type == g_equipType_weapon || type == g_equipType_necklace || type == g_equipType_ring)
	{
		//武器、项链、戒指不允许有防御、魔御
		DropEquip.extra.ac = 0;
		DropEquip.extra.mac = 0;
	}
	//统计极品点数。
	nCount = DropEquip.extra.luck + DropEquip.extra.ac + DropEquip.extra.mac + DropEquip.extra.dc + DropEquip.extra.mc + DropEquip.extra.sc;
	DropEquip.extraAmount = nCount;
}

void fight_fight::CalcDropItemsAndDisplay(monsterID id)
{
	ui.edit_display->append(" ");
	Info_Equip DropEquip;
	double dTmp1, dTmp2;
	quint32 nTmp;
	const ListDrop &LD = g_mapDropSet[id];
	foreach(const Rational &rRat, LD)
	{
		dTmp1 = 1.0 * qrand() / RAND_MAX;
		dTmp2 = 1.0 * (rRat.den - 1) / rRat.den;
		if (dTmp1 > dTmp2)
		{
			if (rRat.ID > g_itemID_start_equip && rRat.ID <= g_itemID_stop_equip)
			{
				//暴出装备,大于拾取过滤或极品皆拾取，否取出售。
				CreateEquip(rRat.ID, DropEquip);
				const Info_basic_equip *equip = Item_Base::GetEquipBasicInfo(DropEquip.ID);
				ui.edit_display->append(QStringLiteral("<font color=white>获得:") + equip->name + QStringLiteral("</font>"));
				if (m_bag_equip->size() >= g_bag_maxSize)
				{
					nTmp = equip->price >> 2;
					myRole->coin += nTmp << 1;
					ui.edit_display->append(QStringLiteral("<font color=white>背包已满，卖出:") + equip->name
						+ QStringLiteral(" 获得金币:") + QString::number(nTmp) + QStringLiteral("</font>"));

					g_falseRole.coin += nTmp;
				}
				else if (DropEquip.extraAmount >= pickFilter)
				{
					m_bag_equip->append(DropEquip);
				}
				else
				{
					nTmp = equip->price >> 2;
					myRole->coin += nTmp << 1;
					ui.edit_display->append(QStringLiteral("<font color=white>卖出:") + equip->name 
						+ QStringLiteral(" 获得金币:") + QString::number(nTmp) + QStringLiteral("</font>"));

					g_falseRole.coin += nTmp;
				}
			}
			else
			{
				//暴出道具
				const Info_Item *item = Item_Base::FindItem_Item(rRat.ID);
				ui.edit_display->append(QStringLiteral("<font color=white>获得:") + item->name + QStringLiteral("</font>"));
				m_bag_item->insert(rRat.ID, m_bag_item->value(rRat.ID) + 1);
			}
		}
	}

	if (bBoss)
	{
		//boss额外友情赞助一些道具（一瓶大红，一瓶大蓝，1个银元）
		itemID nArr[5] = { 201003, 201013, 203007 };
		for (quint32 i = 0; i < 3; i++)
		{
			const Info_Item *item = Item_Base::FindItem_Item(nArr[i]);
			ui.edit_display->append(QStringLiteral("<font color=white>获得:") + item->name + QStringLiteral("</font>"));
			m_bag_item->insert(nArr[i], m_bag_item->value(nArr[i]) + 1);
		}
	}
}

void fight_fight::Action_role(void)
{
	quint32 nTmp1, nTmp_rhp, nTmp_rmp;

	nTmp1 = myRole->intervel_1 << 8 | myRole->intervel_2;
	time_remain_role += nTmp1;	//累加角色活动时间。

	//使用道具的下限
	nTmp1 = myRole->hp_1 << 24 | myRole->hp_2 << 16 | myRole->hp_3 << 8 | myRole->hp_4;
	nTmp1 = (nTmp1 + 1) << 1;
	nTmp_rhp = nTmp1 * ui.edit_hp->text().toInt() / 100;

	nTmp1 = myRole->mp_1 << 24 | myRole->mp_2 << 16 | myRole->mp_3 << 8 | myRole->mp_4;
	nTmp_rmp = nTmp1 * ui.edit_mp->text().toInt() / 100;

	//如果勾选了自动使用道具
	if (bCheckHp && role_hp_2c < nTmp_rhp)
	{
		Step_role_UsingItem_hp();
	}
	if (bCheckMp && role_mp_c < nTmp_rmp)
	{
		Step_role_UsingItem_mp();
	}

	Step_role_Skill();

	double dTmp;
	quint32 nTmp, nDropExp, nDropCoin, nRoleLevel, nDropRep = 0;
	QString strTmp;

	if (monster_cur_hp <= 0)
	{
		bFighting = false;
		buffInMonster.clear();
		for (int  i = 0; i < MaxBuffCount; i++)
		{
			buffDisp_Mon[i]->setPixmap(QPixmap(""));
		}
		
		//怪物死掉，角色增加经验及金币。若是BOSS，再增加声望。
		//必须先乘1.0转化为double，否则等级相减运算仅提升到uint层次从而得到一个无穷大。
		dTmp = atan(0.3 * (1.0 * monster_cur->level - Role_Lvl));
		nDropExp = monster_cur->exp * ((dTmp + 1.58) / 2);
		nDropCoin = nDropExp * 0.1;
		myRole->exp += nDropExp << 1;
		myRole->coin += nDropCoin << 1;

		g_falseRole.exp += nDropExp;
		g_falseRole.coin += nDropCoin;

		if (bBoss)
		{
			nDropRep = nDropExp * 0.01;
			myRole->reputation += nDropRep << 1;

			g_falseRole.reputation += nDropRep;
		}

		quint64 role_exp = (myRole->exp >> 1) - 1;
		if (role_exp > ui.progressBar_role_exp->maximum())
			ui.progressBar_role_exp->setValue(ui.progressBar_role_exp->maximum());
		else
			ui.progressBar_role_exp->setValue(role_exp);
		
		if (bCheckConcise)
			ui.edit_display->setText(QStringLiteral("<font color=white>你击退了 %1 </font>").arg(monster_cur->name));
		else
			ui.edit_display->append(QStringLiteral("<font color=white>你击退了 %1 </font>").arg(monster_cur->name));

		if (bCheckConcise)
		{
			strTmp = QStringLiteral("<font color=white>攻击：") + QString::number(nCount_attack) + QStringLiteral("次</font>");
			ui.edit_display->append(strTmp);
			strTmp = QStringLiteral("<font color=white>格挡：") + QString::number(nCount_parry) + QStringLiteral("次</font>");
			ui.edit_display->append(strTmp);
		}

		if (bBoss)	{
			++nCount_boss;
		}	else	{
			++nCount_normalMonster;
		}
		
		nCount_exp += nDropExp;
		nCount_coin += nDropCoin;
		nCount_rep += nDropRep;
		
		DisplayDropBasic(nDropExp, nDropCoin, nDropRep);
		CalcDropItemsAndDisplay(monster_cur->ID);
	}
}
void fight_fight::Action_monster(void)
{	
	time_remain_monster += monster_cur->interval;	//减少怪物的剩余活动时间。	
	++nCount_parry;									//人物格档一次

	qint32 monster_dc = monster_cur->DC1 + qrand() % (monster_cur->DC2 - monster_cur->DC1 + 1);	
	qint32 role_ac = role_ac1 + qrand() % (role_ac2 - role_ac1 + 1);	
	qint32 nTmp1 = monster_dc - role_ac;
	double dTmp1 = nTmp1 > 0 ? pow(1.0 * nTmp1 / monster_dc + 0.25,4) : 0;	
	qint32 damage_dc = (monster_dc * dTmp1);

	qint32 monster_mc = monster_cur->MC1 + qrand() % (monster_cur->MC2 - monster_cur->MC1 + 1);
	qint32 role_mac = role_mac1 + qrand() % (role_mac2 - role_mac1 + 1);
	nTmp1 = monster_mc - role_mac;
	dTmp1 = nTmp1 > 0 ? pow(1.0 * nTmp1 / monster_mc + 0.25, 4) : 0;
	qint32 damage_mc = (monster_mc * dTmp1);
	
	qint32 nTmp = (damage_dc > 0 ? damage_dc : 1) + (damage_mc > 0 ? damage_mc : 1);
	role_hp_2c -= nTmp << 1;
	if (role_hp_2c < 2)
	{
		role_hp_2c = 2;		//因为是2X + 1，故此判断必须为2，否则后面算出来为负值，会造成显示bug
	}
	qint32 hp_true = (role_hp_2c >> 1) - 1;
	ui.progressBar_role_hp->setValue(hp_true);

	//怪物回血
	monster_cur_hp += monster_cur_rhp;
	if (monster_cur_hp > monster_cur->hp)
		monster_cur_hp = monster_cur->hp;
	if (monster_cur_hp < 0)
		monster_cur_hp = 0;
	ui.progressBar_monster_hp->setValue(monster_cur_hp);

	//非“简洁模式”下显示伤害信息。
	if (!bCheckConcise)
	{
		QList<qint32> list;
		list.append(nTmp);
		ui.edit_display->append(Generate_Display_LineText(monster_cur->name, QStringLiteral("普攻"), QStringLiteral("你"), false, false, list));
	}

	if (hp_true <= 0)
	{
		//角色死亡
		bFighting = false;
 		killTimer(nFightTimer);
		++nCount_fail;

		//角色死亡，损失经验30%、金币20%
		quint64 role_exp = (myRole->exp >> 1) - 1;
		quint64 role_coin = (myRole->coin >> 1) - 1;
		quint32 nExp = role_exp * 0.3;
		quint32 nCoin = role_coin * 0.2;
		myRole->exp -= nExp << 1;
		myRole->coin -= nCoin << 1;

		g_falseRole.exp -= nExp;
		g_falseRole.coin -= nCoin;

		ui.progressBar_role_exp->setValue((myRole->exp >> 1) - 1);
		ui.edit_display->append(QStringLiteral("<font color=white>战斗失败!</font>"));
		ui.edit_display->append(QStringLiteral("损失经验：") + QString::number(nExp));
		ui.edit_display->append(QStringLiteral("损失金币：") + QString::number(nCoin));
	}
}

void fight_fight::GenerateMonster()
{
	bBoss = false;
	QString strTmp = "";
	++nElapse_pre_boss;
	if (bCheckFindBoss && monster_boss_count > 0)
	{
		if (nElapse_pre_boss > 100) {
			bBoss = true;
		} else 	{
			bBoss = (1.0 * qrand() / RAND_MAX) > g_fight_boss_probability;
		}

		if (bBoss) 	{
			nElapse_pre_boss = 0;
		}
	}
	if (bBoss)
	{
		qint32 n = qrand() % monster_boss_count;
		monster_cur = &g_MonsterBoss_list[monster_boss_assign[n]];

		strTmp = QStringLiteral("强大的<font size = 4 color=blue>") + monster_cur->name
			+ QStringLiteral("</font>来袭,勇敢地<font size = 5 color = red>战</font>吧！");
	}
	else
	{
		qint32 n = qrand() % monster_normal_count;
		monster_cur = &g_MonsterNormal_List[monster_normal_assign[n]];

		strTmp = QStringLiteral("<font color= white>遭遇 %1</font>").arg(monster_cur->name);
	}
	if (!bCheckConcise)
	{
		ui.edit_display->setText(strTmp);
	}
}

inline __int64 GetCPUTickCount()
{
	__asm
	{
		rdtsc;
	}
}

void fight_fight::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == nXSpeedTimer)
	{
		//检测是否加速
		if (xSpeedTime.elapsed() - nXSpeedInvterval < 0)
		{
			++nXSpeedCount;
			if (nXSpeedCount > 20)
			{
				LogIns.append(LEVEL_ERROR, __FUNCTION__, mirErr_XSpeed);
				exit(0);
			}
		}
		else
		{
			--nXSpeedCount;
			if (nXSpeedCount < 0)
			{
				nXSpeedCount = 0;
			}
		}
		xSpeedTime.restart();
	}
	else if (event->timerId() == nFightTimer)
	{
		//每一次timerEvent为一个回合。 
		//当前未处于战斗状态，故延时显示上一次的战斗信息。延时完后，生成下一个怪物。
		if (!bFighting)
		{
			--nShowStatusRound;
			if (nShowStatusRound >= 0)
			{
				return;
			}
		
			nShowStatusRound = 10;
			//生成一个怪物，并显示怪物信息。
			GenerateMonster();
			Display_CurrentMonsterInfo();
			bFighting = true;
			time_remain = time_remain_role = time_remain_monster = 0;
			nCount_attack = nCount_parry = 0;	
		}

		//回合时间已用完，判断战斗超时。
		if (time_remain >= 5 * 60 * 1000)
		{
			++nCount_timeout;
			ui.edit_display->append(QStringLiteral("<font color=white>战斗超时，重新寻找怪物。</font>"));
			bFighting = false;
			return;
		}
	
		//若回合时间大于角色时间，则角色活动一回合。再判断，若回合时间小于怪物时间，则怪物活动一回合。
		if (time_remain > time_remain_role)
		{
			Action_role();
			updateRoleBuffInfo();
			updateSkillCD();
		}
		else if (time_remain > time_remain_monster)
		{
			Action_monster();
			updateMonsterBuffInfo();
		}
	
		//战斗记时
		time_remain += nFightInterval;
	}
}

void fight_fight::updateRoleBuffInfo(void)
{
	qint32 i;
	role_rhp = 0;

	role_ac1 = myRole->ac1_1 << 24 | myRole->ac1_2 << 16 | myRole->ac1_3 << 8 | myRole->ac1_4;
	role_ac2 = myRole->ac2_1 << 24 | myRole->ac2_2 << 16 | myRole->ac2_3 << 8 | myRole->ac2_4;
	role_mac1 = myRole->mac1_1 << 24 | myRole->mac1_2 << 16 | myRole->mac1_3 << 8 | myRole->mac1_4;
	role_mac2 = myRole->mac2_1 << 24 | myRole->mac2_2 << 16 | myRole->mac2_3 << 8 | myRole->mac2_4;

	for (i = 0; i < MaxBuffCount && i < buffInRole.size(); i++)
	{
		--buffInRole[i].time;
		if (buffInRole[i].time <= 0)
		{
			buffDisp_Role[i]->setPixmap(QPixmap(""));
			buffInRole.remove(i);
		}
		else
		{
			role_rhp += buffInRole[i].rhp;
			role_ac1 += buffInRole[i].ac;
			role_ac2 += buffInRole[i].ac;
			role_mac1 += buffInRole[i].mac;
			role_mac2 += buffInRole[i].mac;
		}
	}

	i = 0;
	for (; i < buffInRole.size(); i++)
	{
		buffDisp_Role[i]->setPixmap(buffInRole[i].icon);
	}
	for (; i < MaxBuffCount; i++)
	{
		buffDisp_Role[i]->setPixmap(QPixmap(""));
	}
	
	ui.edit_role_rhp->setText(QString::number(role_rhp));
	ui.edit_role_ac->setText(QString("%1-%2").arg(role_ac1).arg(role_ac2));
	ui.edit_role_mac->setText(QString("%1-%2").arg(role_mac1).arg(role_mac2));
}
void fight_fight::updateMonsterBuffInfo(void)
{
	qint32 i,nTmp;
	nTmp = 0;
	monster_cur_ac = monster_cur->AC;
	monster_cur_mac = monster_cur->MAC;

	for (i = 0; i < MaxBuffCount && i < buffInMonster.size(); i++)
	{
		--buffInMonster[i].time;
		if (buffInMonster[i].time <= 0)
		{
			buffDisp_Mon[i]->setPixmap(QPixmap(""));
			buffInMonster.remove(i);
		}
		else
		{
			nTmp -= buffInMonster[i].rhp;
			monster_cur_ac -= buffInMonster[i].ac;
			monster_cur_mac -= buffInMonster[i].mac;
		}
	}

	i = 0;
	for (; i < buffInMonster.size(); i++)
	{
		buffDisp_Mon[i]->setPixmap(buffInMonster[i].icon);
	}
	for (; i < MaxBuffCount; i++)
	{
		buffDisp_Mon[i]->setPixmap(QPixmap(""));
	}

	//如果BOSS没有减血buff,则恢复其原来的回血设置。
	if (nTmp >= 0)
	{
		if (m_mapID < 1000)
		{	//只有普通地图的怪有回血功能。
			monster_cur_rhp = monster_cur_hp >> 7;
		}
		else
		{
			monster_cur_rhp = 0;
		}
	}
	else
		monster_cur_rhp = nTmp;

	if (monster_cur_ac < 0)
	{
		monster_cur_ac = 0;
	}
	if (monster_cur_mac < 0)
	{
		monster_cur_mac = 0;
	}
	ui.edit_monster_rhp->setText(QString::number(monster_cur_rhp));
	ui.edit_monster_ac->setText(QString::number(monster_cur_ac));
	ui.edit_monster_mac->setText(QString::number(monster_cur_mac));
}

void fight_fight::updateSkillCD()
{
	for (int i = 0; i < fightingSkill.size(); i++)
	{
		--fightingSkill[i].cd_c;
	}
}