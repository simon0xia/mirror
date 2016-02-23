#ifndef MIRROR_H
#define MIRROR_H

#include <QtWidgets/QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSystemTrayIcon>
#include <QtWinExtras/QtWinExtras>
#include "ui_mirror.h"

#include "role.h"
#include "smithy.h"
#include "item_equipbag.h"
#include "item_itembag.h"
#include "item_equipstorage.h"



class mirror : public QMainWindow
{
	Q_OBJECT

public:
	quint8 markCount = 60;

public:
	mirror(QWidget *parent = 0);
	~mirror();

	//加角色一些道具，此函数主要用来测试道具。
	void GiveSomeItem(void);

protected:
	void closeEvent(QCloseEvent *event);
	void changeEvent(QEvent *e);

private:
	void initUi(void);
	void DelayUpdate(void);

	
	void timerEvent(QTimerEvent *event);

	void Fighting(mapID id);

	void ChangeLeftWindow(LeftWindow lw);

private slots:
	void on_btn_role_clicked();
	void on_btn_smithy_clicked();
	void on_btn_fight_clicked(void);
	void on_btn_achievement_clicked();
	void on_btn_skill_clicked();
	void on_btn_task_clicked();
	void on_btn_sign_clicked();

	void on_action_setting();
	void on_action_about();
	void on_action_help();

	void on_btn_bag_equip_clicked();
	void on_btn_bag_item_clicked();
	void on_btn_storage_equip_clicked();
	
//	void on_mirror_save();

	void DisplayCoin(void){
		QString strTmp;
		qint32 nTmp = PlayerIns.get_coin();
		if (nTmp > 100000) {
			strTmp = QStringLiteral("%1 万").arg(nTmp / 10000);
		} else {
			strTmp = QString::number(nTmp);
		}
		ui.lbl_coin->setText(strTmp);
	}
	void DisplayRep(void){
		QString strTmp;
		qint32 nTmp = PlayerIns.get_rep();
		if (nTmp > 100000) {
			strTmp = QStringLiteral("%1 万").arg(nTmp / 10000);
		} else {
			strTmp = QString::number(nTmp);
		}
		ui.lbl_rep->setText(strTmp);
	}

	void on_DisplayEquip(qint32 index);

	void iconActivated(QSystemTrayIcon::ActivationReason);

private:
	Ui::mirrorClass ui;
	bool bFirstMinimum;
	LeftWindow lw;

	role *m_tab_role;
	smithy *m_tab_smithy;
	item_equipBag *tab_equipBag;
	item_itemBag *tab_itemBag;
	Item_equipStorage tab_equipStorage;

	qint32 nDelayUpdateTimer, nSaveTimer;

	QSystemTrayIcon *trayIcon;

	QMenu *popMenu;
	QAction *action_setting, *action_about, *action_help;
};

#endif // MIRROR_H
