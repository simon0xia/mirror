#include "mirror.h"
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QMenu>
#include "MirrorVersion.h"
#include "mirrorlog.h"
#include "systemsetting.h"
#include "MonsterDefine.h"
#include "cryptography.h"
#include "dlg_detail.h"
#include "role_skill.h"
#include "fight.h"
#include "fight_fight.h"
#include "dlg_about.h"
#include "dlg_task.h"
#include "dlg_sign.h"
#include "gamemanager.h"

QWidget *g_widget;
Dlg_Detail *g_dlg_detail;

extern QMap<mapID, Info_Distribute> g_MonsterDistribute;

bool silentSave();
bool verifyXSpeed(QDateTime time_c);

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
	g_widget = this;
	bFirstMinimum = false;

#ifdef _DEBUG
	GiveSomeItem();	//_test
#endif

	//此dlg必须在tab_role之前，否则会导致访问错误。
	g_dlg_detail = new Dlg_Detail(this);
	g_dlg_detail->setWindowFlags(Qt::WindowStaysOnTopHint);

	m_tab_role = new role;
	m_tab_smithy = new smithy;
	ui.stackedWidget_left->addWidget(m_tab_role);
	ui.stackedWidget_left->addWidget(m_tab_smithy);
	lw = LW_role;
	ChangeLeftWindow(lw);

	CHuman &body = PlayerIns.get_edt_role();

	QIcon mainIcon = QIcon(":/mirror/Resources/mainIcon.png");
	setWindowIcon(mainIcon);
	//设置通知区域图标
	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(mainIcon);
	trayIcon->setToolTip(body.get_name() + QStringLiteral("  Level:") + QString::number(body.get_lv()));

	nDelayUpdateTimer = startTimer(100);
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

	QObject::connect(tab_equipBag, &Item_Base::UpdateCoin, this, &mirror::DisplayCoin);
	QObject::connect(tab_equipBag, &Item_Base::UpdateBag_StorageEquip, &tab_equipStorage, &Item_equipStorage::updateInfo);
	QObject::connect(tab_equipBag, &item_equipBag::UpdateDisplayEquip, this, &mirror::on_DisplayEquip);
	QObject::connect(tab_equipBag, &Item_Base::SmithyEquip, m_tab_smithy, &smithy::updateInfo);

	QObject::connect(tab_itemBag, &Item_Base::UpdateCoin, this, &mirror::DisplayCoin);
	QObject::connect(tab_itemBag, &Item_Base::UpdateRep, this, &mirror::DisplayRep);
	QObject::connect(tab_itemBag, &Item_Base::SmithyEquip, m_tab_smithy, &smithy::updateInfo);

	QObject::connect(&tab_equipStorage, &Item_Base::UpdateBag_BagEquip, tab_equipBag, &item_equipBag::updateInfo);

	QObject::connect(m_tab_role, &role::UpdateBag_BagEquip, tab_equipBag, &item_equipBag::updateInfo);
	QObject::connect(m_tab_role, &role::UpdateCoin, this, &mirror::DisplayCoin);

	QObject::connect(m_tab_smithy, &smithy::UpdateBag_BagEquip, tab_equipBag, &item_equipBag::updateInfo);
	QObject::connect(m_tab_smithy, &smithy::UpdateCoin, this, &mirror::DisplayCoin);
}

void mirror::on_DisplayEquip(qint32 index)
{
	m_tab_role->DisplayEquip(index);
	m_tab_role->DisplayInfo();
}

mirror::~mirror()
{
	delete m_tab_role;
	delete trayIcon;

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

	QString strTitle = QStringLiteral("mirror(镜像传奇)_%1.%2.%3").arg(version_major).arg(version_minor).arg(version_build);
	setWindowTitle(strTitle);

	popMenu = new QMenu();
	action_setting = new QAction(QStringLiteral("设置"), this);
	action_about = new QAction(QStringLiteral("关于"), this);
	action_help = new QAction(QStringLiteral("帮助"), this);
	popMenu->addAction(action_setting);
	popMenu->addSeparator();
	popMenu->addAction(action_about);
	popMenu->addSeparator();
	popMenu->addAction(action_help);
	ui.btn_system->setMenu(popMenu);

	tab_equipBag = new item_equipBag(lw);
	ui.stackedWidget_bag->addWidget(tab_equipBag);

	tab_itemBag = new item_itemBag(lw);
	ui.stackedWidget_bag->addWidget(tab_itemBag);
	ui.stackedWidget_bag->addWidget(&tab_equipStorage);
	ui.stackedWidget_bag->setCurrentIndex(0);

	connect(action_setting, SIGNAL(triggered(bool)), this, SLOT(on_action_setting()));
	connect(action_about, SIGNAL(triggered(bool)), this, SLOT(on_action_about()));
	connect(action_help, SIGNAL(triggered(bool)), this, SLOT(on_action_help()));
}

void mirror::changeEvent(QEvent *e)
{
	if ((e->type() == QEvent::WindowStateChange) && this->isMinimized())
	{
		hide();

		trayIcon->show();

		CHuman &edt_role = PlayerIns.get_edt_role();
		QString strMsg = edt_role.get_name() + QStringLiteral("  Lv:") + QString::number(edt_role.get_lv());
		trayIcon->setToolTip(strMsg);
		if (!bFirstMinimum)
		{
			trayIcon->showMessage(QStringLiteral("镜像传奇"), strMsg, QSystemTrayIcon::Information, 500);
			bFirstMinimum = true;
		}
	}
}

void mirror::on_btn_role_clicked()
{
	lw = LW_role;
	ChangeLeftWindow(lw);
}
void mirror::on_btn_smithy_clicked()
{
	lw = LW_smithy;
	ChangeLeftWindow(lw);
}

void mirror::on_action_setting()
{
	SystemSetting *dlg = new SystemSetting;
	dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
	dlg->exec();

	delete dlg;
}
void mirror::on_action_help()
{
	QString message = QStringLiteral("详细帮助请查看文件夹内的《新手指导》");
	QMessageBox::information(this, QStringLiteral("帮助"), message);
}
void mirror::on_action_about()
{
	dlg_about Dlg(this);
	Dlg.exec();
}
void mirror::on_btn_achievement_clicked()
{
	QString message = QStringLiteral("暂未开放，敬请期待");
	QMessageBox::information(this, QStringLiteral("成就"), message);
}

void mirror::on_btn_bag_equip_clicked()
{
	g_dlg_detail->hide();
	ui.stackedWidget_bag->setCurrentIndex(0);
	ui.label_bag_back->setPixmap(QPixmap(":/mirror/Resources/ui/r_0_2.png"));
}
void mirror::on_btn_bag_item_clicked()
{
	g_dlg_detail->hide();
	ui.stackedWidget_bag->setCurrentIndex(1);
	ui.label_bag_back->setPixmap(QPixmap(":/mirror/Resources/ui/r_0_3.png"));
}
void mirror::on_btn_storage_equip_clicked()
{
	g_dlg_detail->hide();
	ui.stackedWidget_bag->setCurrentIndex(2);
	ui.label_bag_back->setPixmap(QPixmap(":/mirror/Resources/ui/r_0_4.png"));
}

void mirror::GiveSomeItem()
{
}

bool verifyXSpeed(QDateTime time_c)
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

bool silentSave()
{
	QDateTime time_c = QDateTime::currentDateTime();
	if (!verifyXSpeed(time_c))
	{
		LogIns.append(LEVEL_FATAL, __FUNCTION__, mirErr_XSpeed);
		exit(0);
	}
	QByteArray save_plain, save_cryptograph;
	const CPlayer &accout = PlayerIns;
	const GameManager &gm = GameMgrIns;

	QDataStream DsIn(&save_plain, QIODevice::WriteOnly);
	DsIn << version_major << version_minor << version_build << SaveFileVer;

	//保存基本信息
	DsIn << accout.get_id_H() << accout.get_id_L() << accout.get_lv() << accout.get_exp();
	DsIn << accout.get_coin() << accout.get_gold() << accout.get_rep() << accout.get_soul() << qint32(0);
	DsIn << accout.get_edt_Fight_index() << gm.get_maxMapID() << gm.get_keepSign() << gm.get_preSignTime();
	DsIn << gm.get_RemainDaysTaskCount(0) << gm.get_RemainDaysTaskCount(1) << gm.get_RemainDaysTaskCount(2);

	auto lambda = [](QDataStream &s, CHuman &body)
	{
		qint32 resver = 0;
		s.writeRawData(body.get_name().toStdString().c_str(), 128);
		s << body.get_voc() << body.get_gender() << body.get_lv() << body.get_exp();
		s << body.get_xiulian() << body.get_yuanli() << resver << resver << resver;
		s << resver << resver << resver << resver << resver;
		s.writeRawData((const char *)body.get_onBodyEquip_point(), sizeof(Info_Equip) * MaxEquipCountForRole);

		MapSkillStudy skill = body.get_skill_study();
		s << skill.size();
		foreach(const SkillStudy &sk2, skill)
		{
			s << sk2.id << sk2.level << sk2.usdIndex;
		}
	};
	lambda(DsIn, PlayerIns.get_edt_role());
	lambda(DsIn, PlayerIns.get_edt_warrior());
	lambda(DsIn, PlayerIns.get_edt_magic());
	lambda(DsIn, PlayerIns.get_edt_taoshi());
	
	//保存道具背包信息
	const MapItem &bag_item = PlayerIns.get_bag_item();
	DsIn << bag_item.size();
	for (auto iter = bag_item.begin(); iter != bag_item.end(); iter++)
	{
		DsIn << iter.key() << iter.value();
	}

	//保存装备背包信息
	const ListEquip &bag_equip = PlayerIns.get_bag_equip();
	DsIn << bag_equip.size();
	for (auto iter = bag_equip.begin(); iter != bag_equip.end(); iter++)
	{
		DsIn.writeRawData((char *)&*iter, sizeof(Info_Equip));
	}

	//保存装备仓库信息
	const ListEquip &storage_equip = PlayerIns.get_storage_equip();
	DsIn << storage_equip.size();
	for (auto iter = storage_equip.begin(); iter != storage_equip.end(); iter++)
	{
		DsIn.writeRawData((char *)&*iter, sizeof(Info_Equip));
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
	QDataStream ds(&file);
	ds.writeRawData(save_cryptograph.data(), save_cryptograph.size());
	ds.writeRawData(save_plain.data(), save_plain.size());
	file.close();
	return true;
}

void mirror::on_btn_fight_clicked(void)
{
	fight dlg_f(this);
	dlg_f.setWindowFlags(Qt::SubWindow);
	dlg_f.resize(960, 640);
	dlg_f.move(0, 0);
	dlg_f.exec();

	Fighting(dlg_f.get_SelectMap());

	DisplayCoin();
	DisplayRep();
	tab_equipBag->updateInfo();
	tab_itemBag->DisplayItems();
	m_tab_role->DisplayInfo();
}

void mirror::on_btn_skill_clicked()
{
	role_skill dlg_skill(this, ui.lbl_coin);
	dlg_skill.setWindowFlags(Qt::Tool);
	dlg_skill.setWindowTitle(QStringLiteral("%1的技能配置").arg(PlayerIns.get_edt_current().get_name()));
	dlg_skill.exec();
}

void mirror::on_btn_task_clicked()
{
	dlg_task *taskDlg = new dlg_task(this);
	taskDlg->exec();
	delete taskDlg;

	DisplayCoin();
	DisplayRep();
	tab_equipBag->updateInfo();
	tab_itemBag->DisplayItems();
	m_tab_role->DisplayInfo();
}

void mirror::on_btn_sign_clicked()
{
	dlg_sign *dlg = new dlg_sign(this);
	dlg->exec();
	delete dlg;

	DisplayCoin();
}

void mirror::timerEvent(QTimerEvent *event)
{	
	const int id = event->timerId();
	if (id == nSaveTimer)
	{
		if (!silentSave())
		{
			QString message = QStringLiteral("无法保存，存档文件无法访问。");
			QMessageBox::critical(this, QStringLiteral("自动保存"), message);
		}
	}
	else if (id == nDelayUpdateTimer)
	{
		killTimer(nDelayUpdateTimer);
		DelayUpdate();
	}
}

void mirror::DelayUpdate()
{
	DisplayCoin();
	DisplayRep();
	tab_equipBag->updateInfo();
	tab_itemBag->DisplayItems();
	tab_equipStorage.updateInfo();

	PlayerIns.get_edt_role().InitFightSkill();
	PlayerIns.get_edt_warrior().InitFightSkill();
	PlayerIns.get_edt_magic().InitFightSkill();
	PlayerIns.get_edt_taoshi().InitFightSkill();
}

void mirror::Fighting(mapID id)
{
	const Info_Distribute &dis = g_MonsterDistribute.value(id);
	if (dis.ID == id && id != 0)
	{
		fight_fight *dlg_fighting = new fight_fight(this, dis);
		dlg_fighting->setWindowFlags(Qt::SubWindow);
		dlg_fighting->move(0, 0);
		dlg_fighting->exec();

		delete dlg_fighting;
		dlg_fighting = nullptr;
	}
}

void mirror::ChangeLeftWindow(LeftWindow lw)
{
	ui.stackedWidget_left->setCurrentIndex(lw);
}