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

QWidget *g_widget;
QMap<skillID, Info_skill> g_skillList;				//技能设定
vecBuff g_buffList;									//buff设定
QMap<itemID, Info_Item> g_ItemList;					//游戏道具列表
QVector<Info_basic_equip> g_EquipList;				//游戏装备列表
QMap<itemID, Info_StateEquip> g_StateEquip;			//角色身上装备外观
QMap<mapID, Info_Distribute> g_MonsterDistribute;	//怪物分布列表
QVector<MonsterInfo> g_MonsterNormal_List;			//普通怪物列表
QVector<MonsterInfo> g_MonsterBoss_list;			//BOSS怪列表
QVector<info_task> g_task_main_list;				//主线任务列表
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

	if (!LoadVerify())
	{
		QString message = QStringLiteral("加载验证信息失败，《mirror传奇》是否已经在运行？");
		QMessageBox::critical(this, msgTitle, message);
		exit(0);
	}

	if (!LoadRole() || !LoadJobSet())
	{
		QString message = QStringLiteral("加载职业设定失败，请重新运行游戏。");
		QMessageBox::critical(this, msgTitle, message);
		exit(0);
	}

	if (!LoadSkill() || !LoadBuff())
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
	if (!LoadMonster() || !LoadBoss() || !LoadDistribute() || !LoadDropSet())
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

	GiveSomeItem();	//_test
	m_tab_fight = new fight(player);
	ui.stackedWidget_main->addWidget(m_tab_fight);

	m_tab_role = new role(player);
	ui.stackedWidget_main->addWidget(m_tab_role);

	m_tab_city = new city(player);
	ui.stackedWidget_main->addWidget(m_tab_city);
	ui.stackedWidget_main->setCurrentIndex(1);

	bgAudioList = nullptr;
	bgAudio = nullptr;
	if (QFile::exists("./sound/b-2.mp3"))
	{
		bgAudioList = new QMediaPlaylist;
		bgAudioList->setPlaybackMode(QMediaPlaylist::Loop);
		bgAudio = new QMediaPlayer;
		bgAudio->setPlaylist(bgAudioList);
		bgAudio->setVolume(100);
		bgAudioList->addMedia(QUrl::fromLocalFile("./sound/b-2.mp3"));
		bgAudioList->setCurrentIndex(0);
	}

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

	QObject::connect(m_tab_role, &role::mirrorSave, this, &mirror::on_mirror_save);
	QObject::connect(m_tab_role, &role::bkSound, this, &mirror::enable_bkSound);
}

mirror::~mirror()
{
	delete m_tab_fight;
	delete m_tab_role;
	delete m_tab_city;
	delete trayIcon;

	if (bgAudio != nullptr)
	{
		bgAudio->stop();
	}
	
	delete bgAudio;
	delete bgAudioList;

	delete player;

	SetThreadExecutionState(ES_CONTINUOUS);
}

void mirror::closeEvent(QCloseEvent *event)
{
	QString title = QStringLiteral("退出确认");
	QString message = QStringLiteral("是否需要保存游戏？");
	QMessageBox msgBox(QMessageBox::Question, title, message);
	QPushButton *YsBtn = msgBox.addButton(QStringLiteral(" 保存并退出 "), QMessageBox::AcceptRole);
	QPushButton *NoBtn = msgBox.addButton(QStringLiteral(" 直接退出 "), QMessageBox::RejectRole);
	msgBox.exec();
	if (msgBox.clickedButton() == YsBtn)
	{
		silentSave();
	}
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

	QString strTitle = QStringLiteral("mirror传奇_beta_%1.%2.%3").arg(version_major).arg(version_minor).arg(version_build);
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
		trayIcon->setToolTip(player->get_name() + QStringLiteral("  Level:") + QString::number(player->get_lv()));
		if (!bFirstMinimum)
		{
			QString strMsg = player->get_name() + QStringLiteral("  Lv:") + QString::number(player->get_lv());
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
	QString message = QStringLiteral("暂未开放，敬请期待");
	QMessageBox::information(this, QStringLiteral("设置"), message);
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
}
void mirror::on_action_limit(bool checked)
{
	QString message = QStringLiteral("暂未开放，敬请期待");
	QMessageBox::information(this, QStringLiteral("成就"), message);
}

bool mirror::LoadVerify()
{
	std::string strPath("./db/verify.db");
	hVerify = CreateFileA(strPath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hVerify == INVALID_HANDLE_VALUE)
	{
		LogIns.append(LEVEL_ERROR, __FUNCTION__, mirErr_FileOpen);
		return false;
	}
	return true;
}
bool mirror::LoadJobSet()
{
	char MD5[] = "562cfed093c8b982ecf1c17b9f6cfbfc";
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
			>> job.sc1 >> job.sc2 >> job.ac1 >> job.ac2 >> job.mac1 >> job.mac2;

		g_JobAddSet.append(job);
	}


	if (g_JobAddSet[g_JobAddSet.size()-1].level != 1350)
	{
		return false;
	}
	else
	{
		return true;
	}	
}
bool mirror::LoadSkill()
{
	char MD5[] = "8cbaf363db6c1ee90dda4b94a3322c07";
	QFile file("./db/skill.db");
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
	
	Info_skill skill;
	QImage img;

	while (!out.atEnd())
	{
		out >> skill.ID >> skill.name >> img >> skill.type >> skill.level >> skill.spell_basic >> skill.spell_add >> skill.cd;
		out >> skill.times >> skill.basic >> skill.damage_basic >> skill.damage_add >> skill.buff >> skill.stiff >> skill.descr;

		skill.icon = QPixmap::fromImage(img);
		g_skillList[skill.ID] = skill;
	}
	return true;
}
bool mirror::LoadBuff()
{
	char MD5[] = "281ceb001f45b61688a9d882b5977027";
	QFile file("./db/buff.db");
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}
	info_buff buff;
	
	QByteArray documentContent = file.readAll();
	file.close();

	if (!cryptography::verifyDB_MD5(MD5, documentContent))
	{
		LogIns.append(LEVEL_ERROR, __FUNCTION__, mirErr_MD5);
		return false;
	}

	QDataStream out(documentContent);

	while (!out.atEnd())
	{
		out >> buff.ID >> buff.name >> buff.time >> buff.rhp >> buff.ac >> buff.mac;

		g_buffList.append(buff);
	}

	return true;
}
bool mirror::LoadItemList()
{
	char MD5[] = "5cf0436297ff0fad208afc4b22164eb4";
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
	char MD5[] = "3bf02fcf0bfca8ba1a47287c0fe9e5e9";

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
		out >> equip.ID >> equip.name >> img >> equip.lv >> secrettype >> equip.sbv >> equip.sgv;
		out >> equip.luck >> equip.acc >> equip.sacred >> equip.ag >> equip.spd >> equip.md >> equip.ep >> equip.ed;
		out >> equip.ac1 >> equip.ac2 >> equip.mac1 >> equip.mac2 >> equip.dc1 >> equip.dc2 >> equip.mc1 >> equip.mc2 >> equip.sc1 >> equip.sc2;
		out >> equip.need >> equip.needLvl >> equip.price;

		equip.icon = QPixmap::fromImage(img);
		equip.st = static_cast<SecretType>(secrettype);
		g_EquipList.append(equip);
	}

	return true;
}

bool mirror::LoadStateEquip()
{
	char MD5[] = "9a459dbe2fe165e943f26cb36c7fca45";
	QFile file("./db/StateEquip.db");
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}
	itemID id;
	QImage img;
	Info_StateEquip equip;
	
	QByteArray documentContent = file.readAll();
	file.close();

	if (!cryptography::verifyDB_MD5(MD5, documentContent))
	{
		LogIns.append(LEVEL_ERROR, __FUNCTION__, mirErr_MD5);
		return false;
	}
	
	QDataStream out(documentContent);
	while (!out.atEnd())
	{
		out >> id >> img >> equip.offset_x >> equip.offset_y;

		equip.img = QPixmap::fromImage(img);
		g_StateEquip.insert(id, equip);
	}

	return true;
}

void mirror::GiveSomeItem()
{
	Info_Equip equip = { 0 };
// 	for (int i = 301001; i < 301002; i++)
// 	{
// 		equip.ID = i;
// 		m_bag_equip.append(equip);
// 	}

	player->set_Lv(50);
	player->get_bag_item()->insert(220017, 10);
	player->get_bag_item()->insert(220030, 10);
}

bool mirror::LoadDistribute()
{
	char MD5[] = "d1318cbe299eb7f091692ed95929816a";
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
		out >> dis.ID >> dis.name >> img >> dis.need_lv >> dis.expend_rep >> dis.expend_item >> dis.normal >> dis.boss;

		dis.img = QIcon(QPixmap::fromImage(img));
		g_MonsterDistribute.insert(dis.ID, dis);
	}

	return true;
}

bool mirror::LoadMonster()
{
	char MD5[] = "6903b4748c51c3d6b7569d87122cbb02";

	QFile file("./db/Monster_normal1.db");
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
		out >> mon.ID >> mon.name >> mon.Head >> mon.level >> mon.exp >> mon.hp >> mon.mp;
		out >> mon.DC1 >> mon.DC2 >> mon.MC1 >> mon.MC2 >> mon.AC >> mon.MAC >> hit >> mon.interval;
		g_MonsterNormal_List.append(mon);
	}

	return true;
}

bool mirror::LoadBoss()
{
	char MD5[] = "01adbb79950ec3ea9737dabaa3b5a744";
	QFile file("./db/Monster_boss1.db");
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

	MonsterInfo mon;
	quint32 hit;


	while (!out.atEnd())
	{
		out >> mon.ID >> mon.name >> mon.Head >> mon.level >> mon.exp >> mon.hp >> mon.mp;
		out >> mon.DC1 >> mon.DC2 >> mon.MC1 >> mon.MC2 >> mon.AC >> mon.MAC >> hit >> mon.interval;
		g_MonsterBoss_list.append(mon);		
	}

	return true;
}

bool mirror::LoadDropSet()
{
	char MD5[] = "dc4e21b743f3a2db5334233280e2c80c";
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

	quint32 monsterID, ListSize;
	Rational rRat;
	QList<Rational> listDrop;

	while (!out.atEnd())
	{
		out >> monsterID >> ListSize;

		listDrop.clear();
		while (ListSize--)
		{
			out >> rRat.ID >> rRat.den;
			listDrop.append(rRat);
		}
		g_mapDropSet.insert(monsterID, listDrop);
	}

	return true;
}

bool mirror::LoadTaskSet()
{
	char MD5[] = "9a459dbe2fe165e943f26cb36c7fca45";
	QFile file("./db/task.db");
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	QByteArray documentContent = file.readAll();
	file.close();

// 	if (!verifyDB_MD5(MD5, documentContent, __FUNCTION__))
// 	{
// 		return false;
// 	}

	QDataStream out(documentContent);
	info_task task;

	while (!out.atEnd())
	{
		out >> task.requireItem >> task.requireCount >> task.giveItem >> task.giveCount >> task.msg;
		g_task_main_list.append(task);
	}
	return true;
}

bool mirror::LoadFormula()
{
	char MD5[] = "ca21603b2c2d396ca7201e2d4bb721e6";
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
	quint64 coin, gold, reputation, exp;
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
	out >> vocation >> gender >> coin >> gold >> reputation >> exp >> level;

	player = new CPlayer(name, static_cast<RoleVoc>(vocation), level, gender, coin, gold, reputation, exp);
	if (player == nullptr)
	{
		return false;
	}

	out.readRawData((char *)player->get_onBodyEquip_point(), sizeof(Info_Equip) * MaxEquipCountForRole);

	//加载道具背包信息
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		out >> nItemID >> nItemCount;
		m_bag_item.insert(nItemID, nItemCount);
	}

	//加载道具仓库信息
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		out >> nItemID >> nItemCount;
		m_storage_item.insert(nItemCount, nItemCount);
	}
	
	//加载装备背包信息
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		out.readRawData((char *)&equip, sizeof(Info_Equip));
		m_bag_equip.append(equip);
	}

	//加载装备仓库信息
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		out.readRawData((char *)&equip, sizeof(Info_Equip));
		m_storage_equip.append(equip);
	}

	//加载技能
	roleSkill2 sk2 = { 0, 0, true };
	quint32 SaveVer = ver_major * 1000000 + ver_minor * 1000 + ver_build;
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		if (SaveVer < 3006)
		{
			out >> sk2.id >> sk2.level;
		}
		else
		{
			out >> sk2.id >> sk2.level >> sk2.Used;
		}
		
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
// 	qint32 nTmp, level;
// 	bool bTest = true;
// 	bTest &= roleInfo.coin == (g_falseRole.coin + 1) << 1;
// 	bTest &= roleInfo.gold == (g_falseRole.gold + 1) << 1;
// 	bTest &= roleInfo.reputation == (g_falseRole.reputation + 1) << 1;
// 	bTest &= roleInfo.exp == (g_falseRole.exp + 1) << 1;
// 	bTest &= roleInfo.level == (g_falseRole.level + 1) << 1;
// 	if (!bTest)
// 	{
// 		return false;
// 	}
// 
// 
// 	quint8 *p = &roleInfo.mark_1;
// 	for (quint8 i = 0; i < markCount; i++)
// 	{
// 		if (*(p + i * 2) != 0)
// 		{
// 			return false;
// 		}
// 	}
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

bool mirror::silentSave(const QString SaveFileName)
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
	quint64 nTmp64Arr[4];
	QByteArray save_plain, save_cryptograph;

	QDataStream out(&save_plain, QIODevice::WriteOnly);
	out << version_major << version_minor << version_build << SaveFileVer;

	//保存基本信息
	out.writeRawData(player->get_name(), 128);
	out << player->get_voc() << player->get_gender();
	out << player->get_coin() << player->get_gold() << player->get_rep() << player->get_exp() << player->get_lv();

	//保存身上装备
	out.writeRawData((char *)player->get_onBodyEquip_point(), sizeof(Info_Equip) * MaxEquipCountForRole);
	
	//保存道具背包信息
	nTmp = m_bag_item.size();
	out << nTmp;
	for (MapItem::iterator iter = m_bag_item.begin(); iter != m_bag_item.end(); iter++)
	{
		out << iter.key() << iter.value();
	}

	//保存道具仓库信息
	nTmp = m_storage_item.size();
	out << nTmp;
	for (MapItem::iterator iter = m_storage_item.begin(); iter != m_storage_item.end(); iter++)
	{
		out << iter.key() << iter.value();
	}

	//保存装备背包信息
	nTmp = m_bag_equip.size();
	out << nTmp;
	for (ListEquip::iterator iter = m_bag_equip.begin(); iter != m_bag_equip.end(); iter++)
	{
		out.writeRawData((char *)&*iter, sizeof(Info_Equip));
	}

	//保存装备仓库信息
	nTmp = m_storage_equip.size();
	out << nTmp;
	for (ListEquip::iterator iter = m_storage_equip.begin(); iter != m_storage_equip.end(); iter++)
	{
		out.writeRawData((char *)&*iter, sizeof(Info_Equip));
	}

	//保存玩家已学会的技能
	nTmp = m_skill_study.size();
	out << nTmp;
	for (MapRoleSkill::const_iterator iter = m_skill_study.begin(); iter != m_skill_study.end(); iter++)
	{
		out << iter->id << iter->level << iter->Used;
	}

	if (!cryptography::Encrypt(save_cryptograph, save_plain))
	{
		return false;
	}

	QFile file(SaveFileName);
	if (!file.open(QIODevice::WriteOnly))
	{
		return false;
	}
	QDataStream dfs(&file);
	dfs.writeRawData(save_cryptograph.data(), save_cryptograph.size());
	dfs.writeRawData(save_plain.data(), save_plain.size());
	file.close();
	return true;
}

void mirror::on_mirror_save()
{
	if (silentSave())
	{
		QString message = QStringLiteral("游戏已保存。");
		QMessageBox::information(this, QStringLiteral("手动保存"), message);
	}
	else
	{
		QString message = QStringLiteral("无法保存，存档文件无法访问。");
		QMessageBox::critical(this, QStringLiteral("手动保存"), message);
	}
}

void mirror::on_btn_skill_clicked()
{
	m_skill_study.remove(0);

	role_skill *dlg_skill = new role_skill(this, player->get_voc(), &m_skill_study);
	dlg_skill->setWindowFlags(Qt::Tool);
	dlg_skill->show();
}

void mirror::on_btn_task_clicked()
{
	task *taskDlg = new task(this);
	taskDlg->exec();
	delete taskDlg;
}


void mirror::enable_bkSound(bool bEnable)
{
	if (bgAudio != nullptr)
	{
		if (bEnable)
		{
			bgAudio->play();
		}
		else
		{
			bgAudio->stop();
		}
	}
}

void mirror::timerEvent(QTimerEvent *event)
{	
	if (!silentSave("autoSave.sxv"))
	{
		QString message = QStringLiteral("无法保存，存档文件无法访问。");
		QMessageBox::critical(this, QStringLiteral("自动保存"), message);
	}
}
