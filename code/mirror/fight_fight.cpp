#include "fight_fight.h"
#include <QFile>
#include <QMessageBox>
#include <QThread>

const int interval = 100;
extern QVector<ItemInfo> g_ItemList;

fight_fight::fight_fight(QWidget* parent, qint32 id, RoleInfo *info, MapItem *bag_item)
: QDialog(parent), m_mapID(id), myRole(info), m_bag_item(bag_item)
{
	ui.setupUi(this);

	ui.edit_display->setText(QString::number(m_mapID));

	Cacl_Display_Role_Value();
	LoadItem();

	monster_count = Boss_count = 0;
	bKeepFight = bFighting = false;
	monster_cur = &monsterArr[0];

	LoadDistribute();
	LoadMonster();
	LoadBoss();
	Display_CurrentMonsterInfo();

	ui.progressBar_monster_hp->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
	ui.progressBar_monster_mp->setStyleSheet("QProgressBar::chunk { background-color: rgb(0, 0, 255) }");
	ui.progressBar_monster_ap->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 255, 0) }");
}

fight_fight::~fight_fight()
{
	
}

void fight_fight::on_btn_quit_clicked(void)
{
	if (!bFighting)
	{
		this->close();
	}
	else
	{
		QString title = QString::fromLocal8Bit("提示");
		QString message = QString::fromLocal8Bit("当前正在战斗中，逃跑将损失50%声望及30%金币。");
		QMessageBox msgBox(QMessageBox::Question, title, message);
		msgBox.addButton(QString::fromLocal8Bit(" 是 "), QMessageBox::AcceptRole);
		msgBox.addButton(QString::fromLocal8Bit(" 否 "), QMessageBox::RejectRole);
		if (msgBox.exec() == QMessageBox::AcceptRole);
		{
			myRole->coin -= myRole->coin * 0.3;
			myRole->reputation -= myRole->reputation * 0.5;
			this->close();
		}
	}
}

void fight_fight::on_btn_start_clicked(void)
{
	time_remain = time_remain_role = time_remain_monster = 100;
	nCount_attack = nCount_parry= nCount_item = nRound = 0;
	nShowStatusRound = 5;

	//生成一个怪物，并显示怪物信息。
	qint32 n = qrand() % monster_count;
	monster_cur = &monsterArr[n];
	Display_CurrentMonsterInfo();

	ui.edit_display->setText(QString::fromLocal8Bit("战斗开始"));

	nFightTimer = startTimer(interval);
	bFighting = true;
	ui.btn_start->setEnabled(false);
}

void fight_fight::Cacl_Display_Role_Value()
{
	ui.edit_role_name->setText(myRole->name);
	ui.edit_role_level->setText(QString::number(myRole->level));

	ui.progressBar_role_exp->setMaximum(myRole->lvExp);
	if (myRole->exp >= ui.progressBar_role_exp->maximum())
		ui.progressBar_role_exp->setValue(ui.progressBar_role_exp->maximum());
	else
		ui.progressBar_role_exp->setValue(myRole->exp);

	role_Speed = qMin(2.5, 1 + myRole->agility * 0.01);
	ui.edit_role_AttackSpeed->setText(QString::number(role_Speed));

	role_DC = myRole->level * 1.0 + myRole->strength * 1 + 3;			//simon:暂时给人物多加3点攻击。
	ui.edit_role_PhysicsAttack->setText(QString::number(role_DC));

	role_MC = myRole->level * 0.9 + myRole->wisdom * 1;
	ui.edit_role_MagicAttack->setText(QString::number(role_MC));

	role_SC = myRole->level * 0.8 + myRole->spirit * 1;
	ui.edit_role_SpellAttack->setText(QString::number(role_SC));

	role_AC = 0;
	ui.edit_role_PhysicsGuard->setText(QString::number(role_AC));

	role_MAC = 0;
	ui.edit_role_ElementGuard->setText(QString::number(role_MAC));

	role_extrarate = 0;
	ui.edit_role_extrarate->setText(QString::number(role_extrarate));

	role_extrahurt = 0;
	ui.edit_role_extrahurt->setText(QString::number(role_extrahurt));

	ui.progressBar_role_hp->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
	ui.progressBar_role_hp->setMaximum(myRole->hp_m);
	ui.progressBar_role_hp->setValue(myRole->hp_m);
	role_hp_c = myRole->hp_m;

	ui.progressBar_role_mp->setStyleSheet("QProgressBar::chunk { background-color: rgb(0, 0, 255) }");
	ui.progressBar_role_mp->setMaximum(myRole->mp_m);
	ui.progressBar_role_mp->setValue(myRole->mp_m);
	role_mp_c = myRole->mp_m;

	ui.progressBar_role_ap->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 255, 0) }");
	ui.progressBar_role_ap->setMaximum(myRole->ap_m);
	ui.progressBar_role_ap->setValue(0);
	role_ap_c = 0;

	role_rhp = 0;
	ui.edit_role_rhp->setText(QString::number(role_rhp));

	role_rmp = 0;
	ui.edit_role_rmp->setText(QString::number(role_rmp));

	role_rap = 1;
	ui.edit_role_rap->setText(QString::number(role_rap));
}

ItemInfo* fight_fight::FindItem(quint32 ID)
{
	for (QVector<ItemInfo>::iterator iter = g_ItemList.begin(); iter != g_ItemList.end(); iter++)
	{
		if (iter->ID == ID)
		{
			return &*iter;
		}
	}
	return NULL;
}
ItemInfo* fight_fight::FindItem(const QString &name)
{
	for (QVector<ItemInfo>::iterator iter = g_ItemList.begin(); iter != g_ItemList.end(); iter++)
	{
		if (iter->name == name)
		{
			return &*iter;
		}
	}
	return NULL;
}

void fight_fight::LoadItem()
{
	QString strTmp;
	ItemInfo *itemItem;
	for (MapItem::iterator iter = m_bag_item->begin(); iter != m_bag_item->end(); iter++)
	{
		itemItem = FindItem(iter.key());
		if (itemItem != NULL)
		{
			if (itemItem->type == et_immediate_hp)
			{
				strTmp = Generate_ItemComboBox_Text(itemItem->name, QString::fromLocal8Bit("血"), itemItem->value, iter.value());
				ui.comboBox_hp->addItem(strTmp);
			}
			else if (itemItem->type == et_immediate_mp)
			{
				strTmp = Generate_ItemComboBox_Text(itemItem->name, QString::fromLocal8Bit("魔"), itemItem->value, iter.value());;
				ui.comboBox_mp->addItem(strTmp);
			}
		}
	}
}

void fight_fight::LoadDistribute()
{
	QString db_distribute = "distribute.db";
	QFile file(db_distribute);
	if (!file.open(QIODevice::ReadOnly))
	{
		QString message = QString::fromLocal8Bit("加载怪物分布列表失败，请重新运行游戏。");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);

		exit(0);
	}

	quint32 id;
	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> id >> monster_id_start >> monster_id_stop >> boss_id_start >> boss_id_stop;
		if (id == m_mapID)
		{
			break;
		}
	}

	file.close();
}


void fight_fight::LoadMonster()
{
	QString db_monster = "monster";
	if (m_mapID < 30) {
		db_monster += "1";
	}
	else if (m_mapID < 70) {
		db_monster += "2";
	}
	else {
		db_monster += "3";
	}
	db_monster += ".db";

	QFile file(db_monster);
	if (!file.open(QIODevice::ReadOnly))
	{
		QString message = QString::fromLocal8Bit("加载失败，请重新运行游戏。");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);

		exit(0);
	}

	MonsterInfo *mon;
	qint32 i = 0;
	QDataStream out(file.readAll());
	while (!out.atEnd() && i < Max_monster)
	{
		mon = &monsterArr[i];

		out >> mon->ID >> mon->name >> mon->Head >> mon->level >> mon->exp;
		out >> mon->hp_m >> mon->hp_r >> mon->mp_m >> mon->mp_r >> mon->ap_m >> mon->ap_r;
		out >> mon->DC >> mon->MC >> mon->SC >> mon->AC >> mon->MAC;
		out >> mon->extrarate >> mon->extrahurt >> mon->penetrate >> mon->Speed;

		//只有id处于有效区间的才记录，其他皆被覆盖。
		if (mon->ID > monster_id_stop)
		{
			break;
		}
		if (mon->ID >= monster_id_start)
		{
			++i;
			++monster_count;
		}
	}

	file.close();
}

void fight_fight::LoadBoss()
{
	QString db_boss = "boss.db";
	QFile file(db_boss);
	if (!file.open(QIODevice::ReadOnly))
	{
		QString message = QString::fromLocal8Bit("加载失败，请重新运行游戏。");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);

		exit(0);
	}

	MonsterInfo *boss;
	qint32 i = 0;
	QDataStream out(file.readAll());
	while (!out.atEnd() && i < Max_monster)
	{
		boss = &BossArr[i];

		out >> boss->name >> boss->ID >> boss->level >> boss->exp;
		out >> boss->hp_m >> boss->hp_r >> boss->mp_m >> boss->mp_r >> boss->ap_m >> boss->ap_r;
		out >> boss->DC >> boss->MC >> boss->SC >> boss->AC >> boss->MAC;
		out >> boss->extrarate >> boss->extrahurt >> boss->penetrate >> boss->Speed;

		//只有id处于有效区间的才记录，其他皆被覆盖。
		if (boss->ID > boss_id_start)
		{
			break;
		}
		if (boss->ID >= boss_id_stop)
		{
			++i;
			++Boss_count;
		}
	}

	file.close();
}

void fight_fight::Display_CurrentMonsterInfo()
{
	//设置体、魔、气最大值。
	ui.progressBar_monster_hp->setMaximum(monster_cur->hp_m);
	ui.progressBar_monster_mp->setMaximum(monster_cur->mp_m);
	ui.progressBar_monster_ap->setMaximum(monster_cur->ap_m);
	
	//回复体、魔为最大值,气为0
	monster_cur->hp_c = monster_cur->hp_m;
	monster_cur->mp_c = monster_cur->mp_m;
	monster_cur->ap_c = 0;

	//显示当前体、魔、气
	ui.progressBar_monster_hp->setValue(monster_cur->hp_c);
	ui.progressBar_monster_mp->setValue(monster_cur->mp_c);
	ui.progressBar_monster_ap->setValue(monster_cur->ap_c);

	//加载头像
	ui.label_monster_head->setPixmap(QPixmap::fromImage(monster_cur->Head));

	//加载其他属性
	ui.edit_monster_name->setText(monster_cur->name);
	ui.edit_monster_level->setText(QString::number(monster_cur->level));
	ui.edit_monster_PhysicsAttack->setText(QString::number(monster_cur->DC));
	ui.edit_monster_MagicAttack->setText(QString::number(monster_cur->MC));
	ui.edit_monster_SpellAttack->setText(QString::number(monster_cur->SC));
	ui.edit_monster_PhysicsGuard->setText(QString::number(monster_cur->AC));
	ui.edit_monster_ElementGuard->setText(QString::number(monster_cur->MAC));
	ui.edit_monster_rhp->setText(QString::number(monster_cur->hp_r));
	ui.edit_monster_rmp->setText(QString::number(monster_cur->mp_r));
	ui.edit_monster_rap->setText(QString::number(monster_cur->ap_r));
	ui.edit_monster_extrarate->setText(QString::number(monster_cur->extrarate));
	ui.edit_monster_extrahurt->setText(QString::number(monster_cur->extrahurt));
	ui.edit_monster_AttackSpeed->setText(QString::number(monster_cur->Speed));
	ui.edit_monster_penetrate->setText(QString::number(monster_cur->penetrate));
}

inline QString fight_fight::Generate_ItemComboBox_Text(const QString &name, const QString &type, quint32 value, quint32 count)
{
	QString strSplit = QString::fromLocal8Bit(" ");
	QString strTmp = name;
	strTmp += strSplit + type + QString::fromLocal8Bit(":") + QString::number(value);
	strTmp += strSplit + QString::fromLocal8Bit("剩:") + QString::number(count);
	return strTmp;
}
inline QString fight_fight::Generate_Display_LineText(const QString &str1, const QString &skill, const QString &str2, quint32 damage)
{
	QString strTmp = QString::fromLocal8Bit("<font color=blue>") + str1
		+ QString::fromLocal8Bit("</font>使用<font color=darkRed>") + skill
		+ QString::fromLocal8Bit("</font>，对<font color = blue>") + str2
		+ QString::fromLocal8Bit("</font>造成伤害:<font color = magenta>") + QString::number(damage)
		+ QString::fromLocal8Bit("</font>");
	return strTmp;
}

void fight_fight::Step_role_UsingItem_hp(void)
{
	++nCount_item;

	quint32 ID;
	bool bHasNotItem = true;

	QString strTmp = ui.comboBox_hp->currentText();
	QStringList strList = strTmp.split(" ");

	ItemInfo *itemItem = FindItem(strList.at(0));
	if (itemItem != NULL)
	{
		ID = itemItem->ID;
		//背包对应道具数量减1
		m_bag_item->insert(ID, m_bag_item->value(ID) - 1); 
		strTmp = Generate_ItemComboBox_Text(itemItem->name, QString::fromLocal8Bit("血"), itemItem->value, m_bag_item->value(ID));
		ui.comboBox_hp->setItemText(ui.comboBox_hp->currentIndex(), strTmp);

		//更改角色状态
		role_hp_c += itemItem->value;
		if (role_hp_c >= myRole->hp_m)
		{
			role_hp_c = myRole->hp_m;
		}
		ui.progressBar_role_hp->setValue(role_hp_c);
		if (!ui.checkBox_concise->isChecked())
		{
			strTmp = QString::fromLocal8Bit("你使用了：") + itemItem->name;
			ui.edit_display->append(strTmp);
		}

		//如果道具已经用完，则删除当前道具.如果还有道具，则切换到0号道具，否则清除自动补血复选。
		if (m_bag_item->value(ID) <= 0)
		{
			ui.comboBox_hp->removeItem(ui.comboBox_hp->currentIndex());
			if (ui.comboBox_hp->count() > 0)
			{
				bHasNotItem = false;
				ui.comboBox_hp->setCurrentIndex(0);
			}
			else
			{
				ui.checkBox_hp->setChecked(false);
			}
			m_bag_item->remove(ID);
		}
	}
	else
	{	//找不到对应道具，清除自动补血复选。
		ui.checkBox_hp->setCheckState(Qt::Unchecked);
	}
}
void fight_fight::Step_role_UsingItem_mp(void)
{
	++nCount_item;
}
void fight_fight::Step_role_UsingItem_ap(void)
{
	++nCount_item;
}
void fight_fight::Step_role_NormalAttack(void)
{
	++nCount_attack;
	//角色普通攻击，伤害值 = (角色物理力-怪物物理防御力）
	qint32 nTmp = role_DC - monster_cur->AC;
	monster_cur->hp_c -= nTmp;
	if (monster_cur->hp_c <= 0)
	{
		monster_cur->hp_c = 0;
	}
	ui.progressBar_monster_hp->setValue(monster_cur->hp_c);

	if (!ui.checkBox_concise->isChecked())
	{
		ui.edit_display->append(
			Generate_Display_LineText(QString::fromLocal8Bit("你"), QString::fromLocal8Bit("基本剑术"), monster_cur->name, nTmp)
			);
	}
}
void fight_fight::Step_role_SkillAttack(void)
{
	++nCount_attack;
}
void fight_fight::Step_role_BoostAccack(void)
{
	++nCount_attack;
}

void fight_fight::Action_role(void)
{
	//减少角色的剩余活动时间。
	time_remain_role -= 1 / role_Speed;

	qint32 limit_rhp = myRole->hp_m * ui.edit_hp->text().toInt() / 100;
	qint32 limit_rmp = myRole->mp_m * ui.edit_mp->text().toInt() / 100;
	qint32 limit_rap = myRole->ap_m * ui.edit_ap->text().toInt() / 100;

	//如果勾选了自动使用道具
	if (ui.checkBox_hp->isChecked() && role_hp_c < limit_rhp)
	{
		Step_role_UsingItem_hp();
	}
	else if (ui.checkBox_mp->isChecked() && role_mp_c < limit_rmp)
	{
		Step_role_UsingItem_mp();
	}
	else if (ui.checkBox_ap->isChecked() && false)
	{	//暂时没有rap道具。
		Step_role_UsingItem_ap();
	}
	else if (role_ap_c >= myRole->ap_m && false)
	{
		//暂时没有爆气技能
		Step_role_BoostAccack();
	}
	else if (false /*技能列表*/)
	{
		Step_role_SkillAttack();
	}
	else
	{
		Step_role_NormalAttack();
	}

	quint32 nDropExp, nDropCoin;

	if (monster_cur->hp_c <= 0)
	{
		bFighting = false;

		//怪物死掉，角色增加经验及金币。
		nDropExp = monster_cur->exp;
		nDropCoin = nDropExp * 0.1;

		myRole->exp += nDropExp;
		myRole->coin += nDropCoin;

		if (myRole->exp >= ui.progressBar_role_exp->maximum())
			ui.progressBar_role_exp->setValue(ui.progressBar_role_exp->maximum());
		else
			ui.progressBar_role_exp->setValue(myRole->exp);

		ui.edit_display->append(QString::fromLocal8Bit("战斗胜利!"));
		if (ui.checkBox_concise->isChecked())
		{
			QString strTmp;
			strTmp = QString::fromLocal8Bit("攻击：") + QString::number(nCount_attack) + QString::fromLocal8Bit("次");
			ui.edit_display->append(strTmp);
			strTmp = QString::fromLocal8Bit("格挡：") + QString::number(nCount_parry) + QString::fromLocal8Bit("次");
			ui.edit_display->append(strTmp);
			strTmp = QString::fromLocal8Bit("使用道具：") + QString::number(nCount_item) + QString::fromLocal8Bit("次");
			ui.edit_display->append(strTmp);
		}
		ui.edit_display->append(QString::fromLocal8Bit("获得经验:") + QString::number(nDropExp));
		ui.edit_display->append(QString::fromLocal8Bit("获得金币:") + QString::number(nDropCoin));
	}
}
void fight_fight::Action_monster(void)
{
	//减少怪物的剩余活动时间。
	time_remain_monster -= 1 / monster_cur->Speed;

	//人物格档一次
	++nCount_parry;

	//怪物砍角色一刀，伤害值 = (怪物物理攻击力-角色物理防御力）
	qint32 nTmp = monster_cur->DC - role_AC;
	role_hp_c -= nTmp;
	if (role_hp_c <= 0)
	{
		role_hp_c = 0;
	}
	ui.progressBar_role_hp->setValue(role_hp_c);

	//非“简洁模式”下显示伤害信息。
	if (!ui.checkBox_concise->isChecked())
	{
		ui.edit_display->append(
			Generate_Display_LineText(monster_cur->name, QString::fromLocal8Bit("普通攻击"), QString::fromLocal8Bit("你"), nTmp)
			);
	}

	if (role_hp_c <= 0)
	{
		//设置战斗状态为非战斗，并且角色死亡后不可再次战斗。
		bFighting = false;
		ui.btn_start->setEnabled(false);
		ui.checkBox_auto->setChecked(false);

		//角色死亡，损失经验、声望、金币各10%
		quint32 nExp = myRole->exp * 0.1;
		quint32 nCoin = myRole->coin * 0.1;
		quint32 nRep = myRole->reputation * 0.1;
		myRole->exp -= nExp;
		myRole->coin -= nCoin;
		myRole->reputation -= nRep;

		ui.progressBar_role_exp->setValue(myRole->exp);
		ui.edit_display->append(QString::fromLocal8Bit("战斗失败!"));
		ui.edit_display->append(QString::fromLocal8Bit("损失经验：") + QString::number(nExp));
		ui.edit_display->append(QString::fromLocal8Bit("损失金币：") + QString::number(nCoin));
		ui.edit_display->append(QString::fromLocal8Bit("损失声望：") + QString::number(nRep));
	}
}

void fight_fight::timerEvent(QTimerEvent *event)
{
	//每一次timerEvent为一个回合。 
	//当前未处于战斗状态，故延时显示上一次的战斗信息。同时倒计时结束后停止计时器，并检测是否勾选“自动战斗”，若有则再次启动战斗。
	if (bFighting == false)
	{
		--nShowStatusRound;
		if (nShowStatusRound <= 0)
		{
			killTimer(nFightTimer);
			ui.btn_start->setEnabled(true);
			if (ui.checkBox_auto->isChecked())
			{
				on_btn_start_clicked();
			}
		}	
		return;
	}

//	++nRound;
//	if (!ui.checkBox_concise->isChecked())
//	{
//		ui.edit_display->append(QString::fromLocal8Bit("第") + QString::number(nRound)
//			+ QString::fromLocal8Bit("回合"));
//	}

	//回合时间已用完，判断战斗超时。并停止所有战斗，包括自动战斗。
	if (time_remain <= 0)
	{
		ui.edit_display->append(QString::fromLocal8Bit("战斗超时！"));
		killTimer(nFightTimer);
		ui.checkBox_auto->setCheckState(Qt::Unchecked);
		return;
	}
	
	//若回合时间小于角色时间，则角色活动一回合。再判断，若回合时间小于怪物时间，则怪物活动一回合。
	if (time_remain < time_remain_role)
	{
		Action_role();
	}
	else if (time_remain < time_remain_monster)
	{
		Action_monster();
	}
	
	//减少一回合时间。(统一单位到秒)
	time_remain -= interval / 1000.0;
}
