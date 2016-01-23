#include "login_main.h"
#include <QFile>

#include <QMessageBox>
#include "login_create.h"
#include "MirrorVersion.h"
#include "mirrorlog.h"

#include "cryptography.h"
#include "MirrorVersion.h"
#include "ItemDefine.h"
#include "MonsterDefine.h"
#include "player.h"

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
QVector<QVector<Info_jobAdd>> g_JobAddSet;			//职业加成设定
//QMap<itemID, info_formula> g_formula;				//装备合成公式

bool LoadAccount(QString &str);
bool LoadJobSet(QString &str);
bool LoadMonster(QString &str);
bool LoadSkillBasic(QString &str);
bool LoadSkillBuff(QString &str);
bool LoadSkillDamage(QString &str);
bool LoadSkillSummon(QString &str);
bool LoadSkillTreat(QString &str);
bool LoadStateEquip(QString &str);
bool LoadTaskSet(QString &str);

bool silentSave();

login_main::login_main(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_roleIndex = 0;
	setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);

	ui.btn_start->setEnabled(false);
	ui.btn_delect->setEnabled(false);
	ui.btn_recover->setEnabled(false);
	ui.btn_1_select->setEnabled(false);
	ui.btn_2_select->setEnabled(false);

	bgAudioList = nullptr;
	bgAudio = nullptr;
	if (QFile::exists("./sound/b-3.mp3"))
	{
		bgAudioList = new QMediaPlaylist;
		bgAudioList->setPlaybackMode(QMediaPlaylist::Loop);
		bgAudio = new QMediaPlayer;
		bgAudio->setPlaylist(bgAudioList);
		bgAudio->setVolume(80);
		bgAudioList->addMedia(QUrl::fromLocalFile("./sound/b-3.mp3"));
		bgAudioList->setCurrentIndex(0);
		bgAudio->play();
	}

	QString strTitle = QStringLiteral("mirror传奇_%1.%2.%3").arg(version_major).arg(version_minor).arg(version_build);
	this->setWindowTitle(strTitle);
	QIcon mainIcon = QIcon(":/mirror/Resources/mainIcon.png");
	setWindowIcon(mainIcon);

	if (QFile::exists(SaveFileName)) {
		roleCount = 1;
	} else {
		roleCount = 0;
	}
	bWasLoad = false;
	timer_main = startTimer(100);
}

login_main::~login_main()
{
	if (bgAudio != nullptr)
	{
		bgAudio->stop();		
	}
	delete bgAudio;
	delete bgAudioList;
}

void login_main::on_btn_1_select_clicked()
{
	m_roleIndex = 1;
	ui.btn_start->setEnabled(true);
	ui.btn_delect->setEnabled(true);

	movie->setPaused(false);
	nChangeMovieTimer = startTimer(1000);
}

void login_main::on_btn_2_select_clicked()
{
	m_roleIndex = 2;
	ui.btn_start->setEnabled(true);
}

void login_main::on_btn_start_clicked()
{
	if (m_roleIndex != 0)
	{
		done(QDialog::Accepted);
	}
}

void login_main::on_btn_create_clicked()
{
	if (roleCount > 0)
	{
		QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("暂时不支持第二存档"));
		return;
	}
	ui.btn_quit->setEnabled(false);
	//存在bug.若不关闭创建角色窗口，而是直接关闭程序，会导致进程驻留。故屏蔽右上角X
	login_create *lc = new login_create(this);
	lc->setWindowFlags(Qt::SubWindow);
	lc->move(QPoint(50, 20));
	if (QDialog::Accepted == lc->exec())
	{
		ReadBasicInfo();
		if (!CheckSaveFile()) {
			exit(0);
		}
		++roleCount;
		DisplayBasicInfo();
		ui.lbl_1_role->setVisible(true);
	}
	ui.btn_quit->setEnabled(true);

	delete lc;

	QString strTmp;
	bLoadSuccess &= LoadAccount(strTmp);
	if (bLoadSuccess){
		ui.btn_1_select->setEnabled(true);
	} else {
		ui.lbl_loadStepInfo->setText(QStringLiteral("加载失败于：%1").arg(strTmp));
	}
}

void login_main::on_btn_delect_clicked()
{
	QString strTmp = QStringLiteral("你是否确定删除角色:%1？\n注意：删除之后无法恢复.").arg(PlayerIns.get_edt_role().get_name());
	if (QMessageBox::Yes == QMessageBox::question(this, QStringLiteral("删除人物"), strTmp))
	{
		QFile::copy(RecoveryFileName, SaveFileName);
		if (QFile::remove(SaveFileName))
		{
			ui.btn_start->setEnabled(false);
			ui.btn_1_select->setEnabled(false);
			ui.btn_delect->setEnabled(false);

			ui.lbl_1_role->setVisible(false);
			ui.lbl_1_name->setText("");
			ui.lbl_1_level->setText("");
			ui.lbl_1_voc->setText("");

			roleCount = 0;
		}
	}
}

void login_main::on_btn_quit_clicked()
{
	done(QDialog::Rejected);
}

bool login_main::DisplayBasicInfo(void)
{
	ui.lbl_1_name->setText(m_rolename);
	ui.lbl_1_level->setText(QString::number(m_level));
	ui.lbl_1_voc->setText(def_vocation[m_vocation]);

	ShowUnSelectMovie();
	movie->start();
	movie->setPaused(true);
	return true;
}

void login_main::ShowUnSelectMovie()
{
	qint32 headNo = ((m_vocation - 1) * 2 + m_gender) * 10 + 1;
	QString path = (":/mirror/Resources/role/") + QString::number(headNo) + ".gif";

	QPoint pos;
	switch (headNo)
	{
	case 11: pos = QPoint(100, 40); break;
	case 21: pos = QPoint(50, 60); break;
	case 31: pos = QPoint(90, 40); break;
	case 41: pos = QPoint(60, 50); break;
	case 51: pos = QPoint(80, 40); break;
	case 61: pos = QPoint(70, 50); break;
	default: pos = QPoint(100, 40); break;
	}
	ui.lbl_1_role->move(pos + QPoint(50, 25));

	movie = new QMovie(path);
	ui.lbl_1_role->setMovie(movie);
	movie->setSpeed(100);
}

void login_main::ShowSelectMovie()
{
	qint32 headNo = ((m_vocation - 1) * 2 + m_gender) * 10 + 0;
	QString path = (":/mirror/Resources/role/") + QString::number(headNo) + ".gif";

	QPoint pos;
	switch (headNo)
	{
	case 10: pos = QPoint(100, 39); break;
	case 20: pos = QPoint(50, 60); break;
	case 30: pos = QPoint(90, 40); break;
	case 40: pos = QPoint(60, 50); break;
	case 50: pos = QPoint(80, 40); break;
	case 60: pos = QPoint(70, 50); break;
	default: pos = QPoint(100, 40); break;
	}
	ui.lbl_1_role->move(pos + QPoint(50, 25));

	movie = new QMovie(path);
	ui.lbl_1_role->setMovie(movie);
	movie->setSpeed(50);
	movie->start();
}

void login_main::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == nChangeMovieTimer)
	{
		killTimer(nChangeMovieTimer);
		ShowSelectMovie();
	}
	else if (event->timerId() == timer_Load)
	{
		killTimer(timer_Load);
		Load();
	}
	else if (event->timerId() == timer_main)
	{
		killTimer(timer_main);	
		if (roleCount > 0)
		{
			ReadBasicInfo();
			if (!CheckSaveFile())
			{
				exit(0);
			}
			DisplayBasicInfo();
		}
		
		if (bLoadSuccess && roleCount > 0){
			ui.btn_1_select->setEnabled(true);
		}
		timer_Load = startTimer(100);
	}
}

bool login_main::updateSaveFileVersion()
{
	QFile file(SaveFileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	qint32 nTmp, nVoc, gender, roleLv;
	quint32 nItemID, nItemCount;
	quint64 coin, gold, reputation, soul, roleExp;
	Info_Equip equip;
	char rolename[128] = { '\0' };

	QDataStream out(&file);
	out.skipRawData(2000 + 4 * sizeof(int32_t));
	out.readRawData(rolename, 128);
	out >> nVoc >> gender;
	out >> coin >> gold >> reputation >> soul >> roleExp >> roleLv;

	Info_Equip onWearEquip[MaxEquipCountForRole];
	out.readRawData((char *)onWearEquip, sizeof(Info_Equip) * MaxEquipCountForRole);

	//加载道具背包信息
	MapItem m_bag_item;
	out >> nTmp;
	for (qint32 i = 0; i < nTmp; i++)
	{
		out >> nItemID >> nItemCount;
		if (nItemID > g_itemID_start_item && nItemID < g_itemID_stop_item
			&&(nItemID < 204001 || nItemID > 204999))
		{
			m_bag_item.insert(nItemID, nItemCount);
		}
	}

	//加载道具仓库信息
	MapItem m_storage_item;
	out >> nTmp;
	for (qint32 i = 0; i < nTmp; i++)
	{
		out >> nItemID >> nItemCount;
		if (nItemID > g_itemID_start_item && nItemID < g_itemID_stop_item)
		{
			m_storage_item.insert(nItemCount, nItemCount);
		}
	}

	//加载装备背包信息
	ListEquip m_bag_equip;
	out >> nTmp;
	for (qint32 i = 0; i < nTmp; i++)
	{
		out.readRawData((char *)&equip, sizeof(Info_Equip));
		if (equip.ID > g_itemID_start_equip && equip.ID < g_itemID_stop_equip)
		{
			m_bag_equip.append(equip);
		}
	}

	//加载装备仓库信息
	ListEquip m_storage_equip;
	out >> nTmp;
	for (qint32 i = 0; i < nTmp; i++)
	{
		out.readRawData((char *)&equip, sizeof(Info_Equip));
		if (equip.ID > g_itemID_start_equip && equip.ID < g_itemID_stop_equip)
		{
			m_storage_equip.append(equip);
		}
	}

	//保存玩家已学会的技能
	MapSkillStudy skill;
	SkillStudy sk2;
	out >> nTmp;
	for (qint32 i = 0; i < nTmp; i++)
	{
		out >> sk2.id >> sk2.level >> sk2.usdIndex;
		skill[sk2.id] = sk2;
	}

	int32_t n1, MaxMap;
	n1 = 0;
	MaxMap = 15;

	PlayerIns.Set_BasicInfo(n1, n1, n1, n1, coin, gold, reputation, soul);
	PlayerIns.Set_ExtraInfo(n1, MaxMap);

	CHuman &edt = PlayerIns.get_edt_role();
	edt.set_BasicInfo(rolename, gender, static_cast<Vocation>(nVoc));
	edt.set_levelInfo(roleLv, roleExp);
	edt.set_resver1(n1, n1, n1, n1, n1);
	edt.set_resver2(n1, n1, n1, n1, n1);
	edt.get_skill_study() = skill;
	Info_Equip *eq = edt.get_onBodyEquip_point();
	for (int i = 0; i < MaxEquipCountForRole; i++)
	{
		eq[i] = onWearEquip[i];
	}
	
	QVector<QString> Names = { QStringLiteral("分身-战"), QStringLiteral("分身-法"), QStringLiteral("分身-道") };
	//角色信息
	for (int i = 1; i < 4; i++)
	{
		PlayerIns.set_edt_current(i);
		CHuman &edt = PlayerIns.get_edt_current();
		edt.set_BasicInfo(Names[i-1], n1, Voc_Warrior);
		edt.set_levelInfo(n1, n1);
		edt.set_resver1(n1, n1, n1, n1, n1);
		edt.set_resver2(n1, n1, n1, n1, n1);

		Info_Equip *eq = edt.get_onBodyEquip_point();
		for (int i = 0; i < MaxEquipCountForRole; i++)
		{
			eq[i] = { 0 };
		}
	}
	PlayerIns.get_bag_item() = m_bag_item;
	PlayerIns.get_bag_equip() = m_bag_equip;
	PlayerIns.get_storage_equip() = m_storage_equip;

	return silentSave();
}

bool login_main::ReadBasicInfo()
{
	QFile file(SaveFileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	QDataStream out(&file);
	out.skipRawData(2000);
	out >> ver_major >> ver_minor >> ver_build >> ver_file;
	out.skipRawData(10 * sizeof(int32_t));
	out.readRawData(m_rolename, 128);
	out >> m_vocation >> m_gender >> m_level;
	file.close();
	return true;
}

bool login_main::CheckSaveFile()
{	
	qint32 SaveVer, ApplicationVer;
	bool res = true;

	SaveVer = ver_major * 1000000 + ver_minor * 1000 + ver_build;
	ApplicationVer = version_major * 1000000 + version_minor * 1000 + version_build;
	if (SaveVer == 0)
	{
		//存档损坏
		if (QFile::exists(SaveFileName))
		{
			QFile::remove(SaveFileName);
		}
		QFile::copy(BackFileName, SaveFileName);

		QString message = QStringLiteral("当前存档出错，已尝试修复，请重新启动游戏。");
		QMessageBox::critical(this, QStringLiteral("存档修复"), message);

		res = false;
	}
	else if (SaveVer > ApplicationVer)
	{
		//存储时的游戏版本高于当前游戏版本
		QString message = QStringLiteral("当前存档文件格式无法识别，请检查是否是因为游戏版本过低。");
		message += QStringLiteral("\n当前游戏版本：%1, 存档所用游戏版本：%2").arg(ApplicationVer).arg(SaveVer);
		QMessageBox::critical(this, QStringLiteral("存档不可识别"), message);

		res = false;
	}
	else if (ver_file != SaveFileVer)
	{
		QString strTitle = QStringLiteral("转换存档");
		if (ver_file == 13)
		{
			//存档转换
			QString message = QStringLiteral("检测到当前存档文件版本过旧，是否转换到最新版本？\n转换过程不可逆！请先备份存档然后按YES。");
			if (QMessageBox::Yes == QMessageBox::question(this, strTitle, message))
			{
				if(!updateSaveFileVersion())
				{
					QString message = QStringLiteral("存档转化失败。");
					QMessageBox::critical(this, strTitle, message);
				}
				else
				{
					QString message = QStringLiteral("存档转化成功,请重新启动游戏。");
					QMessageBox::information(this, strTitle, message);
				}
			}
		}
		else
		{
			//存档太老，不可转换
			QString message = QStringLiteral("系统无法识别当前存档(版本：%1)。").arg(ver_file);
			QMessageBox::critical(this, strTitle, message);
		}
		res = false;
	}

	return res;
}

bool LoadJobSet(QString &str)
{
	str = (QStringLiteral("正在加载:%1").arg(__FUNCTION__));
	char MD5[] = "6d7811724863f3b7cfa33c9d22d0584e";
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

	Info_jobAdd job;
	quint32 count;
	g_JobAddSet.resize(3);
	//只加载本职业的数据库信息
	for (int i = 0; i < 3; i++)
	{
		out >> count;
		while (count--)
		{
			out >> job.level >> job.exp >> job.hp >> job.mp >> job.dc1 >> job.dc2 >> job.mc1 >> job.mc2
				>> job.sc1 >> job.sc2 >> job.ac >> job.mac;

			g_JobAddSet[i].append(job);
		}
	}
	return true;
}
bool LoadSkillBasic(QString &str)
{
	str = (QStringLiteral("正在加载:%1").arg(__FUNCTION__));
	char MD5[] = "27295098671b55206261ff413929d63a";
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
bool LoadSkillDamage(QString &str)
{
	str = (QStringLiteral("正在加载:%1").arg(__FUNCTION__));
	char MD5[] = "f82489f601bca4a0f5087e3ae13bbeb2";
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
bool LoadSkillBuff(QString &str)
{
	str = (QStringLiteral("正在加载:%1").arg(__FUNCTION__));
	char MD5[] = "738ab9445a5bb802d5679e55fce4f5c8";
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
bool LoadSkillSummon(QString &str)
{
	str = (QStringLiteral("正在加载:%1").arg(__FUNCTION__));
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
bool LoadSkillTreat(QString &str)
{
	str = (QStringLiteral("正在加载:%1").arg(__FUNCTION__));
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
bool LoadItemList(QString &str)
{
	str = (QStringLiteral("正在加载:%1").arg(__FUNCTION__));
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
bool LoadEquipList(QString &str)
{
	str = (QStringLiteral("正在加载:%1").arg(__FUNCTION__));
	char MD5[] = "215dbb311585f2beaff99cfb5a06f33c";

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

bool LoadStateEquip(QString &str)
{
	str = (QStringLiteral("正在加载:%1").arg(__FUNCTION__));
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


bool LoadDistribute(QString &str)
{
	str = (QStringLiteral("正在加载:%1").arg(__FUNCTION__));
	char MD5[] = "830a9e0950f4c68d6abfe6bf9ab61e86";

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

bool LoadMonster(QString &str)
{
	str = (QStringLiteral("正在加载:%1").arg(__FUNCTION__));
	char MD5[] = "e8ee7fb76e64de04c5d865832208fa94";

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

bool LoadDropSet(QString &str)
{
	str = (QStringLiteral("正在加载:%1").arg(__FUNCTION__));
	char MD5[] = "8fd2c769e1f11971c68d1720311e185f";
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

bool LoadTaskSet(QString &str)
{
	str = (QStringLiteral("正在加载:%1").arg(__FUNCTION__));
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

bool LoadFormula(QString &str)
{
	str = (QStringLiteral("正在加载:%1").arg(__FUNCTION__));
// 	char MD5[] = "4c4a855da3805a9b5383bf6efb72ace8";
// 	QFile file("./db/formula.db");
// 	if (!file.open(QIODevice::ReadOnly))
// 	{
// 		return false;
// 	}
// 	QByteArray documentContent = file.readAll();
// 	file.close();
// 
// 	if (!cryptography::verifyDB_MD5(MD5, documentContent))
// 	{
// 		LogIns.append(LEVEL_ERROR, __FUNCTION__, mirErr_MD5);
// 		return false;
// 	}
// 
// 	QDataStream out(documentContent);
// 
// 	info_formula f;
// 
// 	while (!out.atEnd())
// 	{
// 		out >> f.ID >> f.npc >> f.m_m >> f.m1_t >> f.m1_c >> f.m2_t >> f.m2_c >> f.m3_t >> f.m3_c >> f.m4_t >> f.m4_c;
// 
// 		g_formula[f.ID] = f;
// 	}

	return true;
}

bool LoadAccount(QString &str)
{
	str = (QStringLiteral("正在加载:%1").arg(__FUNCTION__));
	QFile file(SaveFileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	char name[128];
	int32_t ver_file, ver_major, ver_minor, ver_build, vocation, gender;
	int32_t resver1, resver2, resver3, resver4, resver5;
	int32_t resver6, resver7, resver8, resver9, resver10;
	int32_t coin, gold, reputation, soul, exp, level, accountId_H, accountId_L;
	int32_t FightEdtIndex, MaxMap;
	quint32 nTmp, nItemID, nItemCount;
	Info_Equip equip;
	QByteArray md5Arr_s, cryptData, validData;
	SkillStudy sk2 = { 0, 0, 0 };

	cryptData = file.read(2000);
	validData = file.readAll();
	file.close();

	cryptography::Decrypt(md5Arr_s, cryptData);
	if (!cryptography::verifyDB_MD5(md5Arr_s.data(), validData))
	{
		LogIns.append(LEVEL_ERROR, __FUNCTION__, mirErr_MD5);
		return false;
	}

	PlayerIns.Init();

	QDataStream out(validData);
	out >> ver_major >> ver_minor >> ver_build >> ver_file;

	//账号基本信息
	out >> accountId_H >> accountId_L >> level >> exp;
	out >> coin >> gold >> reputation >> soul;
	out >> FightEdtIndex >> MaxMap;
	PlayerIns.Set_BasicInfo(accountId_H, accountId_L, level, exp, coin, gold, reputation, soul);
	PlayerIns.Set_ExtraInfo(FightEdtIndex, MaxMap);

	//角色信息
	for (int i = 0; i < 4; i++)
	{
		PlayerIns.set_edt_current(i);
		CHuman &edt = PlayerIns.get_edt_current();
		MapSkillStudy &skill = edt.get_skill_study();
		out.readRawData(name, 128);
		out >> vocation >> gender >> level >> exp;
		out >> resver1 >> resver2 >> resver3 >> resver4 >> resver5;
		out >> resver6 >> resver7 >> resver8 >> resver9 >> resver10;
		out.readRawData((char *)edt.get_onBodyEquip_point(), sizeof(Info_Equip) * MaxEquipCountForRole);

		out >> nTmp;
		for (quint32 i = 0; i < nTmp; i++)
		{
			out >> sk2.id >> sk2.level >> sk2.usdIndex;
			skill[sk2.id] = sk2;
		}
		edt.set_BasicInfo(name, gender, static_cast<Vocation>(vocation));
		edt.set_levelInfo(level, exp);
		edt.set_resver1(resver1, resver2, resver3, resver4, resver5);
		edt.set_resver2(resver6, resver7, resver8, resver9, resver10);

		edt.updateEquipInfo();
		edt.updateParameter();
	}
	PlayerIns.set_edt_current(0);

	//加载道具背包信息
	MapItem &bag_item = PlayerIns.get_bag_item();
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		out >> nItemID >> nItemCount;
		if (nItemID > g_itemID_start_item && nItemID < g_itemID_stop_item)
		{
			bag_item.insert(nItemID, nItemCount);
		}
	}

	//加载装备背包信息
	ListEquip &bag_equip = PlayerIns.get_bag_equip();
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		out.readRawData((char *)&equip, sizeof(Info_Equip));
		if (equip.ID > g_itemID_start_equip && equip.ID < g_itemID_stop_equip)
		{
			bag_equip.append(equip);
		}
	}

	//加载装备仓库信息
	ListEquip &storage_equip = PlayerIns.get_storage_equip();
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		out.readRawData((char *)&equip, sizeof(Info_Equip));
		if (equip.ID > g_itemID_start_equip && equip.ID < g_itemID_stop_equip)
		{
			storage_equip.append(equip);
		}
	}

	return true;
}


bool login_main::LoadEnvir(QString &str)
{
	typedef bool(*ptrFun)(QString &);
	ptrFun pf[] = {&LoadJobSet, &LoadSkillBasic, &LoadSkillDamage, &LoadSkillBuff, &LoadSkillSummon, &LoadSkillTreat,
		&LoadItemList, &LoadEquipList, &LoadStateEquip, &LoadTaskSet, &LoadFormula,
		&LoadMonster, &LoadDistribute, &LoadDropSet		
	};

	bool bRes = true;
	for (auto ptr : pf)
	{
		bRes = ptr(str);
		ui.lbl_loadStepInfo->setText(str);
		if (!bRes)
		{
			break;
		}
	}
	return bRes;
}

void login_main::Load()
{
	QString strTmp;
	bLoadSuccess = LoadEnvir(strTmp);
	if (bLoadSuccess && roleCount > 0)
	{
		bLoadSuccess &= LoadAccount(strTmp);
	}

	if (bLoadSuccess) {
		ui.lbl_loadStepInfo->setText(QStringLiteral("加载完成"));
		if (roleCount > 0) {
			ui.btn_1_select->setEnabled(true);
		}		
	} else {
		ui.lbl_loadStepInfo->setText(QStringLiteral("加载失败于:%1").arg(strTmp));
	}
}