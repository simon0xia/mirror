#include "login_main.h"
#include <QFile>

#include <QMessageBox>
#include "login_create.h"
#include "MirrorVersion.h"
#include "mirrorlog.h"

#include "cryptography.h"
#include "MirrorVersion.h"
#include "ItemDefine.h"

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

	roleCount = 0;

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
		loadAndDisplay_BasicRoleInfo();
		ui.lbl_1_role->setVisible(true);
	}
	ui.btn_quit->setEnabled(true);

	delete lc;
}

void login_main::on_btn_delect_clicked()
{
	QString strTmp = QStringLiteral("你是否确定删除角色:%1？\n注意：删除之后无法恢复.").arg(rolename);
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

bool login_main::loadAndDisplay_BasicRoleInfo(void)
{
	QFile file(SaveFileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	qint32 ver_file, ver_major, ver_minor, ver_build, nTmp;

	QDataStream out(&file);
	out.skipRawData(2000);
	out >> ver_major >> ver_minor >> ver_build >> ver_file;
	out.readRawData(rolename, 128);
	out >> nTmp >> gender;
	out >> coin >> gold >> reputation >> soul >> exp >> level;
	file.close();

	ui.lbl_1_name->setText(rolename);
	ui.lbl_1_level->setText(QString::number(level));
	ui.lbl_1_voc->setText(def_vocation[nTmp]);
	vocation = static_cast<RoleVoc>(nTmp);

	ShowUnSelectMovie();
	movie->start();
	movie->setPaused(true);
	ui.btn_1_select->setEnabled(true);
	++roleCount;
	return true;
}

void login_main::ShowUnSelectMovie()
{
	qint32 headNo = ((vocation - 1) * 2 + gender) * 10 + 1;
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
	qint32 headNo = ((vocation - 1) * 2 + gender) * 10 + 0;
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
	else if (event->timerId() == timer_main)
	{
		killTimer(timer_main);
		if (QFile::exists(SaveFileName))
		{
			if (!CheckSaveFile())
			{
				exit(0);
			}
			loadAndDisplay_BasicRoleInfo();
			on_btn_1_select_clicked();
		}
	}
}

bool login_main::updateSaveFileVersion()
{
	QFile file(SaveFileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	qint32 ver_file, ver_major, ver_minor, ver_build, nTmp, nVoc;
	quint32 nItemID, nItemCount;
	Info_Equip equip;
	char rolename[128] = { '\0' };

	QDataStream out(&file);
	out.skipRawData(2000);
	out >> ver_major >> ver_minor >> ver_build >> ver_file;
	out.readRawData(rolename, 128);
	out >> nVoc >> gender >> coin >> gold >> reputation >> exp >> level;
	soul = 0;

	Info_Equip onWearEquip[MaxEquipCountForRole];
	out.readRawData((char *)onWearEquip, sizeof(Info_Equip) * MaxEquipCountForRole);

	//加载道具背包信息
	MapItem m_bag_item;
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
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
	for (quint32 i = 0; i < nTmp; i++)
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
	for (quint32 i = 0; i < nTmp; i++)
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
	for (quint32 i = 0; i < nTmp; i++)
	{
		out.readRawData((char *)&equip, sizeof(Info_Equip));
		if (equip.ID > g_itemID_start_equip && equip.ID < g_itemID_stop_equip)
		{
			m_storage_equip.append(equip);
		}
	}

	//加载技能
	MapRoleSkill m_skill_study;
	roleSkill sk2 = { 0, 0, 0 };
	out >> nTmp;
	for (quint32 i = 0; i < nTmp; i++)
	{
		out >> sk2.id >> sk2.level >> sk2.usdIndex;
		m_skill_study[sk2.id] = sk2;
	}
	file.close();

	QByteArray save_plain, save_cryptograph;
	QDataStream in(&save_plain, QIODevice::WriteOnly);
	in << version_major << version_minor << version_build << SaveFileVer;
	//基本信息
	in.writeRawData(rolename, 128);
	in << nVoc << gender << coin << gold << reputation << soul << exp << level;

	//保存身上装备
	in.writeRawData((char *)onWearEquip, sizeof(Info_Equip) * MaxEquipCountForRole);

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

bool login_main::CheckSaveFile()
{	
	QFile file(SaveFileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	qint32 ver_file, ver_major, ver_minor, ver_build, SaveVer, ApplicationVer;
	bool res = true;

	QDataStream out(&file);
	out.skipRawData(2000);
	out >> ver_major >> ver_minor >> ver_build >> ver_file;
	file.close();

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
//		if (ver_file == 10)
//		{
// 			//存档转换
// 			QString message = QStringLiteral("检测到当前存档文件版本过旧，是否转换到最新版本？\n转换过程不可逆！请先备份存档然后按YES。");
// 			if (QMessageBox::Yes == QMessageBox::question(this, QStringLiteral("转换存档"), message))
// 			{
// 				if(!updateSaveFileVersion())
// 				{
// 					QString message = QStringLiteral("存档转化失败。");
// 					QMessageBox::critical(this, QStringLiteral("转换存档"), message);
// 				}
// 				else
// 				{
// 					QString message = QStringLiteral("存档转化成功,请重新启动游戏。");
// 					QMessageBox::information(this, QStringLiteral("转换存档"), message);
// 				}
// 			}
//		}
//		else
		{
			//存档太老，不可转换
			QString message = QStringLiteral("系统无法识别当前存档(版本：%1)。").arg(ver_file);
			QMessageBox::critical(this, QStringLiteral("加载"), message);
		}
		res = false;
	}

	return res;
}