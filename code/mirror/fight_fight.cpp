#include "fight_fight.h"
#include <QFile>
#include <QMessageBox>
#include <QThread>
#include <time.h>
#include <QKeyEvent>

#include "Item_Base.h"
#include "def_System_para.h"

const int interval = 100;

//定义并初始化静态数据成员。
bool fight_fight::bCheckHp = false;
bool fight_fight::bCheckMp = false;
bool fight_fight::bCheckConcise = false;
bool fight_fight::bCheckFindBoss = false;
qint32 fight_fight::pickFilter = 0;

extern vecBuff g_buffList;
extern QVector<Info_skill> g_skillList;
extern QVector<Info_Item> g_ItemList;
extern QVector<Info_basic_equip> g_EquipList;
extern QVector<Info_Distribute> g_MonsterDistribute;
extern QVector<MonsterInfo> g_MonsterNormal_List;
extern QVector<MonsterInfo> g_MonsterBoss_list;
extern mapDrop	g_mapDropSet;

fight_fight::fight_fight(QWidget* parent, qint32 id, RoleInfo *info, MapItem *bag_item, ListEquip *bag_equip)
	: QDialog(parent), m_MainFrame(parent), m_mapID(id), myRole(info), m_bag_item(bag_item), m_bag_equip(bag_equip)
{
	ui.setupUi(this);
	InitUI();

	Cacl_Display_Role_Value();
	LoadItem();

	AssignMonster(g_MonsterNormal_List, g_MonsterBoss_list, g_MonsterDistribute);
	monster_cur = &g_MonsterNormal_List[monster_normal_assign[0]];
	Display_CurrentMonsterInfo();

	bKeepFight = bFighting = bTimeOut = false;	
	time_remain = time_remain_role = time_remain_monster = 0;
	nShowStatusRound = 0;
	nSkillIndex = 0;
	m_dlg_fightInfo = nullptr;

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
	if (!bFighting || bTimeOut)
	{
		this->close();
	}
	else
	{
		QString title = QStringLiteral("提示");
		QString message = QStringLiteral("当前正在战斗中，逃跑将损失50%声望及30%金币。");
		QMessageBox msgBox(QMessageBox::Question, title, message);
		QPushButton *YsBtn = msgBox.addButton(QStringLiteral(" 是 "), QMessageBox::AcceptRole);
		QPushButton *NoBtn = msgBox.addButton(QStringLiteral(" 否 "), QMessageBox::RejectRole);
		msgBox.exec();
		if (msgBox.clickedButton() == YsBtn)
		{
			myRole->coin -= myRole->coin * 0.3;
			myRole->reputation -= myRole->reputation * 0.5;
			this->close();
		}
	}
}

void fight_fight::on_btn_start_clicked(void)
{	
	nFightTimer = startTimer(interval);
	ui.btn_start->setEnabled(false);
}
void fight_fight::on_btn_statistics_clicked(void)
{
	time_t t_Cur, t_cost;
	time(&t_Cur);
	t_cost = (t_Cur - t_Count_start) / 60;
	if (m_dlg_fightInfo != nullptr)
	{
		delete m_dlg_fightInfo;
	}
	m_dlg_fightInfo = new fight_info(this);
	m_dlg_fightInfo->updateInfo(t_cost, nCount_normalMonster, nCount_boss, nCount_exp, nCount_coin, nCount_rep);
	m_dlg_fightInfo->show();
}
void fight_fight::on_checkBox_auto_clicked(void)
{
	if (ui.checkBox_auto->isChecked())
	{
		nCount_normalMonster = nCount_boss = nCount_exp = nCount_coin = nCount_rep = 0;
		time(&t_Count_start);
		bCheckAuto = true;
	}
	else
	{
		bCheckAuto = false;
	}
	
}
void fight_fight::pickFilterChange(int index)
{
	pickFilter = index;
}

void fight_fight::InitUI()
{
	bCheckAuto = false;

	ui.progressBar_monster_hp->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
	ui.progressBar_monster_mp->setStyleSheet("QProgressBar::chunk { background-color: rgb(0, 0, 255) }");
	ui.edit_monster_sc->setText("0 - 0");
	ui.edit_monster_rmp->setText("0");

 	ui.checkBox_hp->setChecked(bCheckHp);
 	ui.checkBox_mp->setChecked(bCheckMp);
 	ui.checkBox_concise->setChecked(bCheckConcise);
	ui.checkBox_boss->setChecked(bCheckFindBoss);
	ui.comboBox_filter->setCurrentIndex(pickFilter);

	ui.label_role_head->setAttribute(Qt::WA_TranslucentBackground, true);
	ui.edit_role_name->setAttribute(Qt::WA_TranslucentBackground, true);
	ui.edit_role_level->setAttribute(Qt::WA_TranslucentBackground, true);
	ui.edit_role_vocation->setAttribute(Qt::WA_TranslucentBackground, true);
// 	ui.label_3->setAttribute(Qt::WA_TranslucentBackground, true);
// 	ui.label_4->setAttribute(Qt::WA_TranslucentBackground, true);
// 	ui.label_5->setAttribute(Qt::WA_TranslucentBackground, true);
// 	ui.label_6->setAttribute(Qt::WA_TranslucentBackground, true);
// 	ui.label_7->setAttribute(Qt::WA_TranslucentBackground, true);
// 	ui.label_8->setAttribute(Qt::WA_TranslucentBackground, true);
// 	ui.label_9->setAttribute(Qt::WA_TranslucentBackground, true);
// 	ui.label_10->setAttribute(Qt::WA_TranslucentBackground, true);
// 	ui.label_11->setAttribute(Qt::WA_TranslucentBackground, true);
// 	ui.label_12->setAttribute(Qt::WA_TranslucentBackground, true);

	ui.label_monster_head->setAttribute(Qt::WA_TranslucentBackground, true);
	ui.edit_monster_name->setAttribute(Qt::WA_TranslucentBackground, true);
	ui.edit_monster_level->setAttribute(Qt::WA_TranslucentBackground, true);
	ui.edit_monster_vocation->setAttribute(Qt::WA_TranslucentBackground, true);
// 	ui.label_53->setAttribute(Qt::WA_TranslucentBackground, true);
// 	ui.label_54->setAttribute(Qt::WA_TranslucentBackground, true);
// 	ui.label_55->setAttribute(Qt::WA_TranslucentBackground, true);
// 	ui.label_56->setAttribute(Qt::WA_TranslucentBackground, true);
// 	ui.label_57->setAttribute(Qt::WA_TranslucentBackground, true);
// 	ui.label_58->setAttribute(Qt::WA_TranslucentBackground, true);
// 	ui.label_59->setAttribute(Qt::WA_TranslucentBackground, true);
// 	ui.label_60->setAttribute(Qt::WA_TranslucentBackground, true);
// 	ui.label_61->setAttribute(Qt::WA_TranslucentBackground, true);
// 	ui.label_62->setAttribute(Qt::WA_TranslucentBackground, true);

// 	ui.checkBox_hp->setStyleSheet("QCheckBox{ background:transparent} ");
// 	ui.checkBox_mp->setStyleSheet("QCheckBox{ background:transparent} ");
// 	ui.checkBox_auto->setStyleSheet("QCheckBox{ background:transparent} ");
// 	ui.checkBox_concise->setStyleSheet("QCheckBox{ background:transparent} ");
// 	ui.checkBox_boss->setStyleSheet("QCheckBox{ background:transparent} ");

//	ui.groupBox->setStyleSheet("QGroupBox{ background:transparent} ");

//	ui.edit_hp->setStyleSheet("QLineEdit{ background:transparent} ");
//	ui.edit_mp->setStyleSheet("QLineEdit{ background:transparent} ");
//	ui.edit_display->setStyleSheet("QTextEdit{ background:transparent} ");

//	ui.btn_start->setStyleSheet("QPushButton{ background:rgb(170, 170, 170)} ");
//	ui.btn_quit->setStyleSheet("QPushButton{ background:rgb(170, 170, 170)} ");
//	ui.btn_statistics->setStyleSheet("QPushButton{ background:rgb(170, 170, 170)} ");

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
	ui.edit_role_name->setText(myRole->name);
	ui.edit_role_level->setText(QStringLiteral("Lv:") + QString::number(myRole->level));

	QString def_vocation[] = { QStringLiteral("无"), QStringLiteral("战"), QStringLiteral("法"), QStringLiteral("道") };
	ui.edit_role_vocation->setText(def_vocation[myRole->vocation]);

	ui.progressBar_role_exp->setMaximum(myRole->lvExp);
	if (myRole->exp >= ui.progressBar_role_exp->maximum())
		ui.progressBar_role_exp->setValue(ui.progressBar_role_exp->maximum());
	else
		ui.progressBar_role_exp->setValue(myRole->exp);

	ui.edit_role_interval->setText(QString::number(myRole->intervel));
	ui.edit_role_dc->setText(QString::number(myRole->dc1) + "-" + QString::number(myRole->dc2));
	ui.edit_role_mc->setText(QString::number(myRole->mc1) + "-" + QString::number(myRole->mc2));
	ui.edit_role_sc->setText(QString::number(myRole->sc1) + "-" + QString::number(myRole->sc2));
	ui.edit_role_ac->setText(QString::number(myRole->ac1) + "-" + QString::number(myRole->ac2));
	ui.edit_role_mac->setText(QString::number(myRole->mac1) + "-" + QString::number(myRole->mac2));

	role_ac1 = myRole->ac1;
	role_ac2 = myRole->ac2;
	role_mac1 = myRole->mac1;
	role_mac2 = myRole->ac2;

	ui.progressBar_role_hp->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
	ui.progressBar_role_hp->setMaximum(myRole->hp);
	ui.progressBar_role_hp->setValue(myRole->hp);
	role_hp_c = myRole->hp;

	ui.progressBar_role_mp->setStyleSheet("QProgressBar::chunk { background-color: rgb(0, 0, 255) }");
	ui.progressBar_role_mp->setMaximum(myRole->mp);
	ui.progressBar_role_mp->setValue(myRole->mp);
	role_mp_c = myRole->mp;

	role_rhp = 0;
	ui.edit_role_rhp->setText(QString::number(role_rhp));

	role_rmp = 0;
	ui.edit_role_rmp->setText(QString::number(role_rmp));

	qint32 headNo = ((myRole->vocation - 1) * 2 + myRole->gender) * 10;
	QString headImg = (":/role/Resources/role/") + QString::number(headNo) + ".png";
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

const Info_Item* fight_fight::FindItem(quint32 ID)
{
	foreach(const Info_Item &item, g_ItemList)
	{
		if (item.ID == ID)
		{
			return &item;
		}
	}
	return NULL;
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
		if (itemItem != nullptr && itemItem->level <= myRole->level)
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
}

bool fight_fight::AssignMonster(QVector<MonsterInfo> normalList, QVector<MonsterInfo> bossList, QVector<Info_Distribute> Distribute)
{
	quint32 c;
	memset(monster_normal_assign, 0, Max_monster * sizeof(quint32));
	memset(monster_boss_assign, 0, Max_monster * sizeof(quint32));

	//先列出本地图可刷新怪物的ID。
	for (quint32 i = 0; i < Distribute.size(); i++)
	{
		if (Distribute[i].ID == m_mapID)
		{
			c = 0;
			foreach(quint32 n, Distribute[i].normal)
			{
				monster_normal_assign[c++] = n;
			}
			monster_normal_count = c;

			c = 0;
			foreach(quint32 n, Distribute[i].boss)
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
			break;
		}
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
	
	//回复体、魔为最大值	
	monster_cur_rhp = monster_cur_hp >> 7;	
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
	QString strSplit = QStringLiteral(" ");
	QString strTmp = name;
	strTmp += strSplit + type + QStringLiteral(":") + QString::number(value);
	strTmp += strSplit + QStringLiteral("剩:") + QString::number(count);
	return strTmp;
}
inline QString fight_fight::Generate_Display_LineText(const QString &str1, const QString &skill, const QString &str2, QList<qint32> listDamage)
{
	QString strTmp = QStringLiteral("<font color=blue>") + str1
		+ QStringLiteral("</font>使用<font color=darkRed>") + skill
		+ QStringLiteral("</font>，对<font color = blue>") + str2
		+ QStringLiteral("</font>造成伤害:<font color = magenta>");

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
	quint32 ID;

	QString strTmp = ui.comboBox_hp->currentText();
	QStringList strList = strTmp.split(" ");

	const Info_Item *itemItem = FindItem(strList.at(0));
	if (itemItem != NULL)
	{
		ID = itemItem->ID;
		//背包对应道具数量减1
		m_bag_item->insert(ID, m_bag_item->value(ID) - 1); 
		strTmp = Generate_ItemComboBox_Text(itemItem->name, QStringLiteral("血"), itemItem->value, m_bag_item->value(ID));
		ui.comboBox_hp->setItemText(ui.comboBox_hp->currentIndex(), strTmp);

		//更改角色状态
		role_hp_c += itemItem->value;
		if (role_hp_c >= myRole->hp)
		{
			role_hp_c = myRole->hp;
		}
		ui.progressBar_role_hp->setValue(role_hp_c);
		if (!bCheckConcise)
		{
			strTmp = QStringLiteral("<font color=black>你使用了：") + itemItem->name + QStringLiteral("</font>");
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
	quint32 ID;
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
		role_mp_c += itemItem->value;
		if (role_mp_c >= myRole->mp)
		{
			role_mp_c = myRole->mp;
		}
		ui.progressBar_role_mp->setValue(role_mp_c);
		if (!bCheckConcise)
		{
			strTmp = QStringLiteral("<font color=black>你使用了：") + itemItem->name + QStringLiteral("</font>");
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

inline quint32 fight_fight::GetRoleATK()
{
	quint32 nA;
	if (myRole->vocation == 1)
	{
		nA = myRole->dc1 + qrand() % (myRole->dc2 - myRole->dc1 + 1);
	}
	else if (myRole->vocation == 2)
	{
		nA = myRole->mc1 + qrand() % (myRole->mc2 - myRole->mc1 + 1);
	}
	else
	{
		nA = myRole->sc1 + qrand() % (myRole->sc2 - myRole->sc1 + 1);
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
				bUsedSkill = MStep_role_Buff(skill);
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
		quint32 nA = GetRoleATK();
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
			if (real.rhp > 0 && 0.8 < 1.0 * role_hp_c / myRole->hp)
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
			QString strTmp = QStringLiteral("<font color=blue>你</font>使用:<font color=darkRed>") + skill.name
				+ QStringLiteral("</font>  效果持续<font color=cyan>") + QString::number(real.time) + QStringLiteral("</font>回合 ");
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
	qint32 nDamage, nTmp;
	QList<qint32> ListDamage;
	for (qint32 i = 0; i < skill.times; i++)
	{
		quint32 nA = GetRoleATK();
		if (skill.id == g_skillList.at(0).ID)
		{
			nTmp = myRole->dc1 + qrand() % (myRole->dc2 - myRole->dc1 + 1);
			nDamage = (nTmp - monster_cur_ac);
		}
		else if (myRole->vocation == 1)
		{
			nTmp = nA * skill.damage / 100;
			nDamage = (nTmp - monster_cur_ac);
		}
		else
		{
			nTmp = nA * skill.damage / 100;
			nDamage = (nTmp - monster_cur_mac);
		}
		nDamage = (nDamage < 1 ? 1 : nDamage);
		ListDamage.append(nDamage);

		nTmp = monster_cur_hp - nDamage;
		monster_cur_hp = nTmp < 0 ? 0 : nTmp;
		ui.progressBar_monster_hp->setValue(monster_cur_hp);	
	}
	if (!bCheckConcise && skill.times != 0)
	{
		ui.edit_display->append(Generate_Display_LineText(QStringLiteral("你"), skill.name, monster_cur->name, ListDamage));
	}
	//更改角色状态
	nTmp = role_hp_c + role_rhp;
	role_hp_c = nTmp > myRole->hp ? myRole->hp : nTmp;
	ui.progressBar_role_hp->setValue(role_hp_c);
	return true;
}
inline void fight_fight::DisplayDropBasic(quint32 nDropExp, quint32 nDropCoin, quint32 nDropRep)
{
	ui.edit_display->append(QStringLiteral("<font color=black>获得经验:") + QString::number(nDropExp) + QStringLiteral("</font>"));
	ui.edit_display->append(QStringLiteral("<font color=black>获得金币:") + QString::number(nDropCoin) + QStringLiteral("</font>"));
	if (bBoss)
	{
		ui.edit_display->append(QStringLiteral("<font color=black>获得声望:") + QString::number(nDropRep) + QStringLiteral("</font>"));
	}
}

void fight_fight::CreateEquip(itemID id, Info_Equip &DropEquip)
{
	//极品0--8点出现的概率
	double probability[9] = { 0, 0.4096, 0.2048, 0.0512, 0.0128, 0.0064, 0.0032, 0.0016, 0.0008 };
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
	{
		if (type == g_equipType_necklace)
			DropEquip.extra.luck = 0;
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
				ui.edit_display->append(QStringLiteral("<font color=black>获得:") + equip->name + QStringLiteral("</font>"));
				if (m_bag_equip->size() >= g_bag_maxSize)
				{
					myRole->coin += equip->price >> 1;
					ui.edit_display->append(QStringLiteral("<font color=black>背包已满，卖出:") + equip->name
						+ QStringLiteral(" 获得金币:") + QString::number(equip->price >> 2) + equip->name + QStringLiteral("</font>"));
				}
				else if (equip->lv > pickFilter || DropEquip.extraAmount > 0)
				{
					m_bag_equip->append(DropEquip);
				}
				else
				{
					myRole->coin += equip->price >> 1;
					ui.edit_display->append(QStringLiteral("<font color=black>卖出:") + equip->name 
						+ QStringLiteral(" 获得金币:") + QString::number(equip->price >> 2) + equip->name + QStringLiteral("</font>"));
				}
			}
			else
			{
				//暴出道具
				const Info_Item *item = Item_Base::FindItem_Item(rRat.ID);
				ui.edit_display->append(QStringLiteral("<font color=black>获得:") + item->name + QStringLiteral("</font>"));
				m_bag_item->insert(rRat.ID, m_bag_item->value(rRat.ID) + 1);
			}
		}
	}

	if (bBoss)
	{
		//boss额外友情赞助一些道具（两瓶大红，两瓶大蓝，1个银元）
		itemID nArr[5] = { 201003, 201003, 201013, 201013, 203007 };
		for (quint32 i = 0; i < 5; i++)
		{
			const Info_Item *item = Item_Base::FindItem_Item(nArr[i]);
			ui.edit_display->append(QStringLiteral("<font color=black>获得:") + item->name + QStringLiteral("</font>"));
			m_bag_item->insert(nArr[i], m_bag_item->value(nArr[i]) + 1);
		}
	}
}

void fight_fight::Action_role(void)
{
	time_remain_role += myRole->intervel;	//累加角色活动时间。

	//使用道具的下限
	qint32 limit_rhp = myRole->hp * ui.edit_hp->text().toInt() / 100;
	qint32 limit_rmp = myRole->mp * ui.edit_mp->text().toInt() / 100;

	//如果勾选了自动使用道具
	if (bCheckHp && role_hp_c < limit_rhp)
	{
		Step_role_UsingItem_hp();
	}
	else if (bCheckMp && role_mp_c < limit_rmp)
	{
		Step_role_UsingItem_mp();
	}
	else
	{
		Step_role_Skill();
	}
	double dTmp;
	quint32 nTmp, nDropExp, nDropCoin, nDropRep = 0;
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
		dTmp = atan(0.3 * (1.0 * monster_cur->level - myRole->level));
		nDropExp = monster_cur->exp * ((dTmp + 1.58) / 2);
		nDropCoin = nDropExp * 0.1;
		myRole->exp += nDropExp;
		myRole->coin += nDropCoin;

		if (bBoss)
		{
			nDropRep = nDropExp * 0.01;
			myRole->reputation += nDropRep;
		}

		if (myRole->exp > ui.progressBar_role_exp->maximum())
			ui.progressBar_role_exp->setValue(ui.progressBar_role_exp->maximum());
		else
			ui.progressBar_role_exp->setValue(myRole->exp);

		ui.edit_display->append(QStringLiteral("<font color=black>战斗胜利!</font>"));
		if (bCheckConcise)
		{
			strTmp = QStringLiteral("<font color=black>攻击：") + QString::number(nCount_attack) + QStringLiteral("次</font>");
			ui.edit_display->append(strTmp);
			strTmp = QStringLiteral("<font color=black>格挡：") + QString::number(nCount_parry) + QStringLiteral("次</font>");
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

		//如果角色胜利并且未勾选自动战斗，则允许其再次点击“开始战斗”
		if (!bCheckAuto)
		{
			killTimer(nFightTimer);
			ui.btn_start->setEnabled(true);
		}
	}
}
void fight_fight::Action_monster(void)
{	
	time_remain_monster += monster_cur->interval;	//减少怪物的剩余活动时间。	
	++nCount_parry;									//人物格档一次

	//怪物砍角色一刀，伤害值 = (怪物物理攻击力-角色物理防御力） + (怪物魔法攻击力 - 角色魔法防御力） + 2 (强制伤害)
	qint32 monster_dc = monster_cur->DC1 + qrand() % (monster_cur->DC2 - monster_cur->DC1 + 1);
	qint32 monster_mc = monster_cur->MC1 + qrand() % (monster_cur->MC2 - monster_cur->MC1 + 1);
	qint32 role_ac = role_ac1 + qrand() % (role_ac2 - role_ac1 + 1);
	qint32 role_mac = role_mac1 + qrand() % (role_mac2 - role_mac1 + 1);
	qint32 damage_dc = (monster_dc - role_ac);
	qint32 damage_mc = (monster_mc - role_mac);
	qint32 nTmp = (damage_dc > 1 ? damage_dc : 1) + (damage_mc > 1 ? damage_mc : 1);
	
	role_hp_c -= nTmp;
	if (role_hp_c < 0)
	{
		role_hp_c = 0;
	}
	ui.progressBar_role_hp->setValue(role_hp_c);

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
		ui.edit_display->append(Generate_Display_LineText(monster_cur->name, QStringLiteral("普攻"), QStringLiteral("你"), list));
	}

	if (role_hp_c <= 0)
	{
		//设置战斗状态为非战斗，并且角色死亡后不可再次战斗。
		bFighting = false;
		killTimer(nFightTimer);
		ui.checkBox_auto->setChecked(false);

		//角色死亡，损失经验30%、金币20%
		quint32 nExp = myRole->exp * 0.3;
		quint32 nCoin = myRole->coin * 0.2;
		myRole->exp -= nExp;
		myRole->coin -= nCoin;

		ui.progressBar_role_exp->setValue(myRole->exp);
		ui.edit_display->append(QStringLiteral("<font color=black>战斗失败!</font>"));
		ui.edit_display->append(QStringLiteral("损失经验：") + QString::number(nExp));
		ui.edit_display->append(QStringLiteral("损失金币：") + QString::number(nCoin));
	}
}

void fight_fight::GenerateMonster()
{
	bBoss = false;
	if (bCheckFindBoss && monster_boss_count > 0)
	{
		bBoss = (1.0 * qrand() / RAND_MAX) > g_fight_boss_probability;
	}
	if (bBoss)
	{
		qint32 n = qrand() % monster_boss_count;
		monster_cur = &g_MonsterBoss_list[monster_boss_assign[n]];

		QString strTmp = QStringLiteral("强大的<font size = 4 color=blue>") + monster_cur->name
			+ QStringLiteral("</font>来袭,勇敢地<font size = 5 color = red>战</font>吧！");
		ui.edit_display->setText(strTmp);
	}
	else
	{
		qint32 n = qrand() % monster_normal_count;
		monster_cur = &g_MonsterNormal_List[monster_normal_assign[n]];
		ui.edit_display->setText("");
	}
}

void fight_fight::timerEvent(QTimerEvent *event)
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
		ui.edit_display->append(QStringLiteral("<font color=black>战斗开始</font>"));
	}

	//回合时间已用完，判断战斗超时。并停止所有战斗，包括自动战斗。
	if (time_remain >= 600000)
	{
		ui.edit_display->append(QStringLiteral("战斗超时！"));
		bTimeOut = true;
		killTimer(nFightTimer);
		ui.checkBox_auto->setCheckState(Qt::Unchecked);
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
	time_remain += interval;
}

void fight_fight::updateRoleBuffInfo(void)
{
	qint32 i;
	role_rhp = 0;
	role_ac1 = myRole->ac1;
	role_ac2 = myRole->ac2;
	role_mac1 = myRole->mac1;
	role_mac2 = myRole->mac2;

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
	
	QString strTmp;
	ui.edit_role_rhp->setText(QString::number(role_rhp));
	strTmp = QString::number(role_ac1) + "-" + QString::number(role_ac2);
	ui.edit_role_ac->setText(strTmp);
	strTmp = QString::number(role_mac1) + "-" + QString::number(role_mac2);
	ui.edit_role_mac->setText(strTmp);
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

	//如果BOSS没有减少buff,则恢复其原来的回血设置。
	if (nTmp >= 0)
		monster_cur_rhp = monster_cur->hp >> 7;
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