#include "login_main.h"
#include <QFile>

#include <QMessageBox>
#include "login_create.h"
#include "MirrorVersion.h"

login_main::login_main(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_roleIndex = 0;
	setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);

	ui.lbl_2_name->setVisible(false);
	ui.lbl_2_level->setVisible(false);
	ui.lbl_2_voc->setVisible(false);
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

	QString strTitle = QStringLiteral("mirror传奇_beta_%1.%2.%3").arg(version_major).arg(version_minor).arg(version_build);
	this->setWindowTitle(strTitle);

	timer_main = startTimer(100);
}

login_main::~login_main()
{
	if (bgAudio != nullptr)
	{
		bgAudio->stop();
		delete bgAudio;
	}
	if (bgAudioList != nullptr)
	{
		delete bgAudioList;
	}
}

void login_main::on_btn_1_select_clicked()
{
	m_roleIndex = 1;
	ui.btn_start->setEnabled(true);

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
//	lc->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
	lc->move(QPoint(50, 20));
	if (QDialog::Accepted == lc->exec())
	{
		loadAndDisplay_BasicRoleInfo();
	}
	ui.btn_quit->setEnabled(true);

	delete lc;
	lc = nullptr;
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

	qint32 ver_file, ver_major, ver_minor, ver_build, nTmpVer1, nTmpVer2;
	quint32 nTmp, nItemID, nItemCount;
	QByteArray md5Arr_s, TmpArr1, TmpArr2;

	TmpArr1 = file.read(2000);
	TmpArr2 = file.readAll();
	file.close();

	QDataStream out(TmpArr2);
	out >> ver_major >> ver_minor >> ver_build >> ver_file;
	nTmpVer1 = ver_major * 1000000 + ver_minor * 1000 + ver_build;
	nTmpVer2 = version_major * 1000000 + version_minor * 1000 + version_build;
	if (nTmpVer1 > nTmpVer2)
	{
		//存档存储时的游戏版本高于当前游戏版本
		QString message = QStringLiteral("当前存档文件格式无法识别，请检查是否是因为游戏版本过低。");
		message += QStringLiteral("\n当前游戏版本：%1, 存档所用游戏版本：%2").arg(nTmpVer2).arg(nTmpVer1);
		QMessageBox::critical(this, QStringLiteral("存档不可识别"), message);
		exit(0);
	}
	if (ver_file != SaveFileVer)
	{
// 		if (ver_file == 3)
// 		{
// 			//存档转换
// 			QString message = QStringLiteral("检测到当前存档文件版本过旧，是否转换到最新版本？\n请注意，此转换不可逆！请先备份存档然后按YES。");
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
// 		}
// 		else
		{
			//存档太老，不可转换
			QString message = QStringLiteral("当前存档文件太古老，系统无法识别。");
			QMessageBox::critical(this, QStringLiteral("转换存档"), message);
		}
		exit(0);
	}

	out.readRawData(roleInfo.name, 128);
	out >> roleInfo.vocation >> roleInfo.gender;
	out >> roleInfo.coin >> roleInfo.gold >> roleInfo.reputation >> roleInfo.exp >> roleInfo.level;

	ui.lbl_1_name->setText(roleInfo.name);
	ui.lbl_1_level->setText(QString::number(roleInfo.level));
	ui.lbl_1_voc->setText(def_vocation[roleInfo.vocation]);
	ShowUnSelectMovie();
	movie->start();
	movie->setPaused(true);
	ui.btn_1_select->setEnabled(true);
	++roleCount;
	return true;
}

void login_main::ShowUnSelectMovie()
{
	qint32 headNo = ((roleInfo.vocation - 1) * 2 + roleInfo.gender) * 10 + 1;
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
	qint32 headNo = ((roleInfo.vocation - 1) * 2 + roleInfo.gender) * 10 + 0;
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
			loadAndDisplay_BasicRoleInfo();
		}
	}
}