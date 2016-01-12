#include "mirror.h"
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include "cryptography.h"
#include "def_item_equip.h"
#include "def_takInfo.h"
#include "about.h"
#include "mirrorlog.h"
#include "MirrorVersion.h"
#include "role_skill.h"
#include "task.h"
#include "general_gameintroduce.h"
#include "systemsetting.h"

QWidget *g_widget;
QMap<skillID, Info_SkillBasic> g_SkillBasic;		//技能设定-基本信息
QMap<qint32, Info_SkillDamage> g_SkillDamage;		//技能设定-伤害设定
QMap<qint32, Info_SkillBuff> g_SkillBuff;			//技能设定-buff,debuff
QMap<qint32, Info_SkillSummon> g_SkillSummon;		//技能设定-召唤兽设定
QMap<qint32, Info_SkillTreat> g_SkillTreat;			//技能设定-治愈术技能
QMap<itemID, Info_Item> g_ItemList;					//游戏道具列表
QMap<itemID, Info_basic_equip> g_EquipList;			//游戏装备列表
QMap<itemID, Info_StateEquip> g_StateEquip;			//角色身上装备外观
QMap<mapID, Info_Distribute> g_MonsterDistribute;	//怪物分布列表
QMap<monsterID, MonsterInfo> g_MonsterInfo;			//怪物信息表
//QVector<info_task> g_task_main_list;				//主线任务列表
mapDrop	g_mapDropSet;								//怪物暴率设定
QVector<Info_jobAdd> g_JobAddSet;					//职业加成设定
QMap<itemID, info_formula> g_formula;				//装备合成公式

mirror::mirror(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	initUi();
#ifdef _DEBUG
	LogIns.init(LEVEL_INFO);
#else
	LogIns.init(LEVEL_ERROR);
#endif	
	QString msgTitle = QStringLiteral("出错啦");

	g_widget = this;
	bFirstMinimum = false;
	player = nullptr;

	if (!LoadRole() || !LoadJobSet())
	{
		QString message = QStringLiteral("加载职业设定失败，请重新运行游戏。");
		QMessageBox::critical(this, msgTitle, message);
		exit(0);
	}

	if (!LoadSkillBasic() || !LoadSkillDamage() || !LoadSkillBuff() || !LoadSkillSummon() || !LoadSkillTreat())
	{
		QString message = QStringLiteral("加载技能失败，请重新运行游戏。");
		QMessageBox::critical(this, msgTitle, message);
		exit(0);
	}
	if (!LoadItemList() || !LoadEquipList() || !LoadStateEquip())
	{
		QString message = QStringLiteral("加载道具或装备失败，请重新运行游戏。");
		QMessageBox::critical(this, msgTitle, message);
		exit(0);
	}
	if (!LoadMonster() || !LoadDistribute() || !LoadDropSet())
	{
		QString message = QStringLiteral("加载怪物失败，请重新运行游戏。");
		QMessageBox::critical(this, msgTitle, message);
		exit(0);
	}
	if (!LoadTaskSet())
	{
		QString message = QStringLiteral("加载任务系统失败，请重新运行游戏。");
		QMessageBox::critical(this, msgTitle, message);
		exit(0);
	}
	if (!LoadFormula())
	{
		QString message = QStringLiteral("加载锻造系统失败，请重新运行游戏。");
		QMessageBox::critical(this, msgTitle, message);
		exit(0);
	}

	player->bind_skill(&m_skill_study);
	player->bind_bag(&m_bag_item, &m_bag_equip);
	player->bind_storage(&m_storage_item, &m_storage_equip);

#ifdef _DEBUG
	GiveSomeItem();	//_test
#endif

	m_tab_fight = new fight(player);
	ui.stackedWidget_main->addWidget(m_tab_fight);
	
	m_tab_role = new role(player);
	ui.stackedWidget_main->addWidget(m_tab_role);
	
	m_tab_city = new city(player);
	ui.stackedWidget_main->addWidget(m_tab_city);
	ui.stackedWidget_main->setCurrentIndex(1);

	QIcon mainIcon = QIcon(":/mirror/Resources/mainIcon.png");
	setWindowIcon(mainIcon);
	//设置通知区域图标
	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(mainIcon);
	trayIcon->setToolTip(player->get_name() + QStringLiteral("  Level:") + QString::number(player->get_lv()));

	nSaveTimer = startTimer(5 * 60 * 1000);		//自动存档
	  
	//建立通知区域图标的响应事件处理连接
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

	//使得电脑不会进入休眠。
	SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED);

	//备份存档
	if (QFile::exists(BackFileName))
	{
		QFile::remove(BackFileName);
	}
	QFile::copy(SaveFileName, BackFileName);
}

mirror::~mirror()
{
	delete m_tab_fight;
	delete m_tab_role;
	delete m_tab_city;
	delete trayIcon;

	delete player;

	SetThreadExecutionState(ES_CONTINUOUS);
}

void mirror::closeEvent(QCloseEvent *event)
{
// 	QString title = QStringLiteral("退出确认");
// 	QString message = QStringLiteral("是否需要保存游戏？");
// 	QMessageBox msgBox(QMessageBox::Question, title, message);
// 	QPushButton *YsBtn = msgBox.addButton(QStringLiteral(" 保存并退出 "), QMessageBox::AcceptRole);
// 	QPushButton *NoBtn = msgBox.addButton(QStringLiteral(" 直接退出 "), QMessageBox::RejectRole);
// 	msgBox.exec();
// 	if (msgBox.clickedButton() == YsBtn)
// 	{
		silentSave();
//	}
}

void mirror::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::Trigger:
		setWindowState(Qt::WindowActive);
		activateWindow();
		showNormal();
		trayIcon->hide();
		break;

	default:
		break;
	}
}

void mirror::initUi()
{
	setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

	QString strTitle = QStringLiteral("mirror传奇_%1.%2.%3").arg(version_major).arg(version_minor).arg(version_build);
	setWindowTitle(strTitle);

	popMenu = new QMenu();
	action_setting = new QAction(QStringLiteral("设置"), this);
	action_limit = new QAction(QStringLiteral("成就"), this);
	action_about = new QAction(QStringLiteral("关于"), this);
	action_help = new QAction(QStringLiteral("帮助"), this);
	popMenu->addAction(action_setting);
	popMenu->addAction(action_limit);
	popMenu->addSeparator();
	popMenu->addAction(action_about);
	popMenu->addSeparator();
	popMenu->addAction(action_help);
	ui.btn_system->setMenu(popMenu);

	connect(action_setting, SIGNAL(triggered(bool)), this, SLOT(on_action_setting(bool)));
	connect(action_limit, SIGNAL(triggered(bool)), this, SLOT(on_action_limit(bool)));
	connect(action_about, SIGNAL(triggered(bool)), this, SLOT(on_action_about(bool)));
	connect(action_help, SIGNAL(triggered(bool)), this, SLOT(on_action_help(bool)));
}

void mirror::changeEvent(QEvent *e)
{
	if ((e->type() == QEvent::WindowStateChange) && this->isMinimized())
	{
		hide();

		trayIcon->show();

		QString strMsg = player->get_name() + QStringLiteral("  Lv:") + QString::number(player->get_lv());
		trayIcon->setToolTip(strMsg);
		if (!bFirstMinimum)
		{
			trayIcon->showMessage(QStringLiteral("mirror传奇"), strMsg, QSystemTrayIcon::Information, 500);
			bFirstMinimum = true;
		}
	}
}

void mirror::on_btn_fight_clicked(void)
{
	ui.stackedWidget_main->setCurrentIndex(0);
}
void mirror::on_btn_role_clicked(void)
{
	ui.stackedWidget_main->setCurrentIndex(1);
	m_tab_role->updateRoleInfo();
}
void mirror::on_btn_city_clicked(void)
{
	m_tab_city->hideAllDlg();
	ui.stackedWidget_main->setCurrentIndex(2);
}
void mirror::on_action_setting(bool checked)
{
	SystemSetting *dlg = new SystemSetting;
	dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
	dlg->exec();

	delete dlg;
}
void mirror::on_action_help(bool checked)
{
	QString message = QStringLiteral("详细帮助请查看文件夹内的《新手指导》");
	QMessageBox::information(this, QStringLiteral("帮助"), message);
}
void mirror::on_action_about(bool checked)
{
	about *Dlg = new about();
	Dlg->exec();

	delete Dlg;
}
void mirror::on_action_limit(bool checked)
{
	QString message = QStringLiteral("暂未开放，敬请期待");
	QMessageBox::information(this, QStringLiteral("成就"), message);
}

bool mirror::LoadJobSet()
{
	char MD5[] = "b3b16a7429eb7ecddda2dff50d044fa2";
	QFile file("./db/jobSet.db");
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

	//因为对齐问题，此处需要特别处理。
	qint32 Len_jobAdd = sizeof(Info_jobAdd) - sizeof(qint32);

	Info_jobAdd job;
	quint32 count;

	quint32 nSkipBytes;

	//只加载本职业的数据库信息
	out >> count;
	nSkipBytes = count * Len_jobAdd * (player->get_voc() - 1) + (player->get_voc() - 1) * sizeof(count);
	if (nSkipBytes > 0)
	{
		if (nSkipBytes != out.skipRawData(nSkipBytes))
			return false;
	}

	while (count--)
	{
		out >> job.level >> job.exp >> job.hp >> job.mp >> job.dc1 >> job.dc2 >> job.mc1 >> job.mc2
			>> job.sc1 >> job.sc2 >> job.ac >> job.mac;

		g_JobAddSet.append(job);
	}	
	return true;
}
bool mirror::LoadSkillBasic()
{
	char MD5[] = "7ab80cffe6a74918f5835b81857cee39";
	QFile file("./db/skill_basic.db");
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
	
	Info_SkillBasic sb;
	QImage img;

	while (!out.atEnd())
	{
		out >> sb.ID >> sb.name >> img >> sb.level >> sb.cd[0] >> sb.cd[1] >> sb.cd[2] >> sb.cd[3] >> sb.type >> sb.no >> sb.descr;

		sb.icon = QPixmap::fromImage(img);
		g_SkillBasic[sb.ID] = sb;
	}
	return true;
}
bool mirror::LoadSkillDamage()
{
	char MD5[] = "24c946b5f00b8f6f4769db8a727dfb0b";
	QFile file("./db/skill_damage.db");
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
	Info_SkillDamage sd;

	while (!out.atEnd())
	{
		out >> sd.id >> sd.type >> sd.targets >> sd.times >> sd.extra >> sd.basic >> sd.add;
		g_SkillDamage[sd.id] = sd;
	}

	return true;
}
bool mirror::LoadSkillBuff()
{
	char MD5[] = "1c450bd899d58e21d8acb25399b9e0b9";
	QFile file("./db/skill_buff.db");
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
	Info_SkillBuff sb;
	qint32 effect;

	while (!out.atEnd())
	{
		out >> sb.id >> sb.time >> sb.targets >> effect >> sb.basic >> sb.add;

		sb.et = static_cast<BufferType>(effect);
		g_SkillBuff[sb.id] = sb;
	}

	return true;
}
bool mirror::LoadSkillSummon()
{
	char MD5[] = "fb9f469be969b77a57670dc058d22d2b";
	QFile file("./db/skill_summon.db");
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
	Info_SkillSummon sm;

	while (!out.atEnd())
	{
		out >> sm.id >> sm.photo >> sm.type >> sm.hp >> sm.damage1 >> sm.damage2 >> sm.defense;
		g_SkillSummon[sm.id] = sm;
	}

	return true;
}
bool mirror::LoadSkillTreat()
{
	char MD5[] = "6deb14b06c99e8d22c3e2631ec20b598";
	QFile file("./db/skill_treat.db");
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
	Info_SkillTreat st;

	while (!out.atEnd())
	{
		out >> st.id >> st.targets >> st.hpr_basic >> st.hpr_add;
		g_SkillTreat[st.id] = st;
	}

	return true;
}
bool mirror::LoadItemList()
{
	char MD5[] = "b409b0caccc31608cbf946ea73a9e069";
	QFile file("./db/item_item.db");
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

	Info_Item item;
	quint32 type;
	QImage img;

	while (!out.atEnd())
	{
		out >> item.ID >> item.name >> img >> item.vocation >> item.level >> item.sale >> item.coin;
		out >> type >> item.value >> item.descr;
		
		item.icon = QPixmap::fromImage(img);
		item.type = static_cast<EffectType>(type);

		g_ItemList.insert(item.ID, item);
	}

	return true;
}
bool mirror::LoadEquipList()
{
	char MD5[] = "97c3b2200e063ef3b397b3d762488b33";

	QFile file("./db/item_equip.db");
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
	Info_basic_equip equip;
	QImage img;
	quint32 secrettype;

	while (!out.atEnd())
	{		
		out >> equip.ID >> equip.name >> img >> equip.lv >> equip.luck >> equip.spd >> equip.hp;
		out >> equip.ac >> equip.mac >> equip.dc1 >> equip.dc2 >> equip.mc1 >> equip.mc2 >> equip.sc1 >> equip.sc2;
		out >> equip.need >> equip.needLvl >> equip.price;

		equip.icon = QPixmap::fromImage(img);
		g_EquipList.insert(equip.ID, equip);
	}

	return true;
}

bool mirror::LoadStateEquip()
{
	QFile file("./db/StateEquip.db");
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}
	itemID id;
	QImage img;
	Info_StateEquip equip;
	
	QDataStream out(&file);
	while (!out.atEnd())
	{
		out >> id >> img >> equip.offset_x >> equip.offset_y;

		equip.img = QPixmap::fromImage(img);
		g_StateEquip.insert(id, equip);
	}
	file.close();

	return true;
}

void mirror::GiveSomeItem()
{
// 	m_bag_equip.clear();
// 
// 	Info_Equip equip = { 0 };
// 	QList<itemID> list = { 301035, 302008, 304010, 305024, 306027, 306027, 307024, 307024, 311003, 312003 };
// 	for (auto iter = list.begin(); iter != list.end(); iter++)
// 	{
// 		equip.ID = *iter;
// 		m_bag_equip.append(equip);
// 	}
// 
// 	player->set_Lv(50);
// 
// 	m_bag_item[220030] = 5;
// 	m_bag_item[220012] = 5;
}

bool mirror::LoadDistribute()
{
	char MD5[] = "def3f8d96b5602569ed38111d6ce6c8f";

	QFile file("./db/distribute.db");
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

	quint32 id;
	QVector<quint32> vec1, vec2;
	Info_Distribute dis;
	QImage img;

	while (!out.atEnd())
	{
		out >> dis.ID >> dis.name >> img >> dis.need_lv >> dis.monsterCount >> dis.normal >> dis.boss;

		dis.img = QIcon(QPixmap::fromImage(img));
		g_MonsterDistribute.insert(dis.ID, dis);
	}
	return true;
}

bool mirror::LoadMonster()
{
	char MD5[] = "b2b38be7f1254ea29a3c3f7a46940613";

	QFile file("./db/Monster.db");
	if (!file.open(QIODevice::ReadOnly))
	{
		false;
	}

	QByteArray documentContent = file.readAll();
	file.close();

	if (!cryptography::verifyDB_MD5(MD5, documentContent))
	{
		LogIns.append(LEVEL_ERROR, __FUNCTION__, mirErr_MD5);
		return false;
	}

	QDataStream out(documentContent);

	MonsterInfo mon;
	quint32 hit;

	while (!out.atEnd())
	{
		out >> mon.ID >> mon.name >> mon.Head >> mon.boss >> mon.level >> mon.exp >> mon.hp >> mon.mp;
		out >> mon.DC1 >> mon.DC2 >> mon.MC1 >> mon.MC2 >> mon.AC >> mon.MAC >> hit >> mon.interval;
		g_MonsterInfo.insert(mon.ID, mon);
	}

	return true;
}

bool mirror::LoadDropSet()
{
	char MD5[] = "837b98c74362aa4bc8fff69e79fa2cde";
	QFile file("./db/drop.db");
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

	quint32 mapid, chance, ListSize, nTmp;
	_tagDrop drop;


	while (!out.atEnd())
	{
		out >> mapid >> chance >> ListSize; 

		drop.chance = chance;

		drop.list.clear();
		while (ListSize--)
		{
			out >> nTmp;
			drop.list.append(nTmp);
		}
		g_mapDropSet.insert(mapid, drop);
	}

	return true;
}

bool mirror::LoadTaskSet()
{
	//no task

// 	char MD5[] = "9a459dbe2fe165e943f26cb36c7fca45";
// 	QFile file("./db/task.db");
// 	if (!file.open(QIODevice::ReadOnly))
// 	{
// 		return false;
// 	}
// 
// 	QByteArray documentContent = file.readAll();
// 	file.close();
//
// 	if (!verifyDB_MD5(MD5, documentContent, __FUNCTION__))
// 	{
// 		return false;
// 	}
// 
// 	QDataStream out(documentContent);
// 	info_task task;
// 
// 	while (!out.atEnd())
// 	{
// 		out >> task.requireItem >> task.requireCount >> task.giveItem >> task.giveCount >> task.msg;
// 		g_task_main_list.append(task);
// 	}
	return true;
}

bool mirror::LoadFormula()
{
	char MD5[] = "4c4a855da3805a9b5383bf6efb72ace8";
	QFile file("./db/formula.db");
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

	info_formula f;

	while (!out.atEnd())
	{
		out >> f.ID >> f.npc >> f.m_m >> f.m1_t >> f.m1_c >> f.m2_t >> f.m2_c >> f.m3_t >> f.m3_c >> f.m4_t >> f.m4_c;

		g_formula[f.ID] = f;
	}

	return true;
}

bool mirror::LoadRole()
{
	QFile file(SaveFileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	char name[128];
	qint32 ver_file, ver_major, ver_minor, ver_build, level, vocation, gender;
	quint64 coin, gold, reputation, soul, exp;
	quint32 nTmp, nItemID, nItemCount;
	Info_Equip equip;
	QByteArray md5Arr_s, cryptData, validData;

	cryptData = file.read(2000);
	validData = file.readAll();
	file.close();

	cryptography::Decrypt(md5Arr_s, cryptData);
	if (!cryptography::verifyDB_MD5(md5Arr_s.data(), validData))
	{
		LogIns.append(LEVEL_ERROR, __FUNCTION__, mirErr_MD5);
		return false;
	}

	QDataStream out(validData);
	out >> ver_major >> ver_minor >> ver_build >> ver_file;

	out.readRawData(name, 128);
	out >> vocation >> gender >> coin >> gold >> reputation >> soul >> exp >> level;

	player = new CPlayer(name, static_cast<RoleVoc>(vocation), level, gender, coin, gold, reputation, soul);
	if (player == nullptr)
	{
		return false;
	}
	player->set_exp(exp);

	out.readRawData((char *)player->get_onBodyEquip_point(), sizeof(Info_Equip) * MaxEquipCountForRole);

	//加载道具背包信息
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		out >> nItemID >> nItemCount;
		if (nItemID > g_itemID_start_item && nItemID < g_itemID_stop_item)
		{
			m_bag_item.insert(nItemID, nItemCount);
		}
	}

	//加载道具仓库信息
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		out >> nItemID >> nItemCount;
		if (nItemID > g_itemID_start_item && nItemID < g_itemID_stop_item)
		{
			m_storage_item.insert(nItemCount, nItemCount);
		}
	}
	
	//加载装备背包信息
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		out.readRawData((char *)&equip, sizeof(Info_Equip));
		if (equip.ID > g_itemID_start_equip && equip.ID < g_itemID_stop_equip)
		{
			m_bag_equip.append(equip);
		}
	}

	//加载装备仓库信息
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		out.readRawData((char *)&equip, sizeof(Info_Equip));
		if (equip.ID > g_itemID_start_equip && equip.ID < g_itemID_stop_equip)
		{
			m_storage_equip.append(equip);
		}
	}

	//加载技能
	roleSkill sk2 = { 0, 0, 0 };
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		out >> sk2.id >> sk2.level >> sk2.usdIndex;	
		m_skill_study[sk2.id] = sk2;
	}
	return true;
}

bool mirror::updateSaveFileVersion()
{
	return false;
}

bool mirror::verifyRoleInfo()
{
	return true;
}
bool mirror::verifyXSpeed(QDateTime time_c)
{
	//比对当前时间和文件最后读取时间。若当前时间大于(晚于)文件最后修改时间，则说明采用了系统级加速。
	//注：此方法只在win7及以上系统上可用。因为win7及以上系统使用加速，也不能修改系统时间，只能hook当前进程
	//的时间相关函数。
	QString strTmp = "./mirror.tmp";
	QFile tmpFile(strTmp);
	if (!tmpFile.open(QIODevice::WriteOnly))
	{
		return false;
	}
	tmpFile.write(strTmp.toStdString().c_str());
	tmpFile.close();


	QFileInfo fi(strTmp);
	QDateTime time_f = fi.lastModified();

	QString str_C = time_c.toString("yy-MM-dd HH:mm:ss");
	QString str_F = time_f.toString("yy-MM-dd HH:mm:ss");
	
	if (time_c > time_f)
	{
		return false;
	}

	return true;
}

bool mirror::silentSave()
{
	if (!verifyRoleInfo())
	{
		LogIns.append(LEVEL_FATAL, __FUNCTION__, mirErr_Modify);
		exit(0);
	}
	QDateTime time_c = QDateTime::currentDateTime();
	if (!verifyXSpeed(time_c))
	{
		LogIns.append(LEVEL_FATAL, __FUNCTION__, mirErr_XSpeed);
		exit(0);
	}

	qint32 nTmp;
	QByteArray save_plain, save_cryptograph;

	QDataStream in(&save_plain, QIODevice::WriteOnly);
	in << version_major << version_minor << version_build << SaveFileVer;

	//保存基本信息
	in.writeRawData(player->get_name().toStdString().c_str(), 128);
	in << player->get_voc() << player->get_gender();
	in << player->get_coin() << player->get_gold() << player->get_rep() << player->get_soul() << player->get_exp() << player->get_lv();

	//保存身上装备
	in.writeRawData((char *)player->get_onBodyEquip_point(), sizeof(Info_Equip) * MaxEquipCountForRole);
	
	//保存道具背包信息
	nTmp = m_bag_item.size();
	in << nTmp;
	for (MapItem::iterator iter = m_bag_item.begin(); iter != m_bag_item.end(); iter++)
	{
		in << iter.key() << iter.value();
	}

	//保存道具仓库信息
	nTmp = m_storage_item.size();
	in << nTmp;
	for (MapItem::iterator iter = m_storage_item.begin(); iter != m_storage_item.end(); iter++)
	{
		in << iter.key() << iter.value();
	}

	//保存装备背包信息
	nTmp = m_bag_equip.size();
	in << nTmp;
	for (ListEquip::iterator iter = m_bag_equip.begin(); iter != m_bag_equip.end(); iter++)
	{
		in.writeRawData((char *)&*iter, sizeof(Info_Equip));
	}

	//保存装备仓库信息
	nTmp = m_storage_equip.size();
	in << nTmp;
	for (ListEquip::iterator iter = m_storage_equip.begin(); iter != m_storage_equip.end(); iter++)
	{
		in.writeRawData((char *)&*iter, sizeof(Info_Equip));
	}

	//保存玩家已学会的技能
	nTmp = m_skill_study.size();
	in << nTmp;
	foreach(const roleSkill &sk2, m_skill_study)
	{
		in << sk2.id << sk2.level << sk2.usdIndex;
	}

	if (!cryptography::Encrypt(save_cryptograph, save_plain))
	{
		return false;
	}

	QFile file("save.sxv");
	if (!file.open(QIODevice::WriteOnly))
	{
		return false;
	}
	QDataStream ds(&file);
	ds.writeRawData(save_cryptograph.data(), save_cryptograph.size());
	ds.writeRawData(save_plain.data(), save_plain.size());
	file.close();
	return true;
}

// void mirror::on_mirror_save()
// {
// 	if (silentSave())
// 	{
// 		QString message = QStringLiteral("游戏已保存。");
// 		QMessageBox::information(this, QStringLiteral("手动保存"), message);
// 	}
// 	else
// 	{
// 		QString message = QStringLiteral("无法保存，存档文件无法访问。");
// 		QMessageBox::critical(this, QStringLiteral("手动保存"), message);
// 	}
// }

void mirror::on_btn_skill_clicked()
{
	m_skill_study.remove(0);
	
	role_skill *dlg_skill = new role_skill(this, player, m_tab_role);
	dlg_skill->setWindowFlags(Qt::Tool);
	dlg_skill->exec();

	delete dlg_skill;
}

void mirror::on_btn_task_clicked()
{
	task *taskDlg = new task(this);
	taskDlg->exec();
	delete taskDlg;
}

void mirror::timerEvent(QTimerEvent *event)
{	
	if (!silentSave())
	{
		QString message = QStringLiteral("无法保存，存档文件无法访问。");
		QMessageBox::critical(this, QStringLiteral("自动保存"), message);
	}
}
