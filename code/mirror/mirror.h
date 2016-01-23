#ifndef MIRROR_H
#define MIRROR_H

#include <QtWidgets/QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSystemTrayIcon>
#include <QtWinExtras/QtWinExtras>
#include "ui_mirror.h"

#include "role.h"
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

private slots:
	void on_btn_fight_clicked(void);
	void on_btn_achievement_clicked();
	void on_btn_skill_clicked();
	void on_btn_task_clicked();

	void on_action_setting();
	void on_action_about();
	void on_action_help();

	void on_btn_bag_equip_clicked();
	void on_btn_bag_item_clicked();
	void on_btn_storage_equip_clicked();
	
//	void on_mirror_save();

	void DisplayCoin(void){
		ui.lbl_coin->setText(QString::number(PlayerIns.get_coin()));
	}
	void DisplayRep(void){
		ui.lbl_rep->setText(QString::number(PlayerIns.get_rep()));
	}
	void UpdateStorageEquip(void) {
		tab_equipStorage.updateInfo();
	}
	void UpdateBagEquip(void) {
		tab_equipBag.updateInfo();
	}
	void on_DisplayEquip(qint32 index);

	void iconActivated(QSystemTrayIcon::ActivationReason);

private:
	Ui::mirrorClass ui;
	bool bFirstMinimum;

	role *m_tab_role;
	item_equipBag tab_equipBag;
	item_itemBag tab_itemBag;
	Item_equipStorage tab_equipStorage;

	qint32 nDelayUpdateTimer, nSaveTimer;

	QSystemTrayIcon *trayIcon;

	QMenu *popMenu;
	QAction *action_setting, *action_about, *action_help;
};

#endif // MIRROR_H
