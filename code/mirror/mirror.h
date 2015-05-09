#ifndef MIRROR_H
#define MIRROR_H

#include <QtWidgets/QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "ui_mirror.h"

#include "role.h"
#include "fight.h"
#include "city.h"

class mirror : public QMainWindow
{
	Q_OBJECT

public:
	mirror(QWidget *parent = 0);
	~mirror();

	//加角色一些道具，此函数主要用来测试道具。
	void GiveSomeItem(void);

protected:
	void closeEvent(QCloseEvent *event);

private:
	//加载存档
	bool LoadRole();
	//加载职业加成信息数据库
	bool LoadJobSet();
	//加载升级经验设置信息数据库
	bool LoadExpSetting();
	//加载技能设置
	bool LoadSkill();

	//加载道具信息数据库
	bool LoadItemList();
	//加载装备信息数据库
	bool LoadEquipList();

	//加载怪物分布信息数据库
	bool LoadDistribute();
	//加载普通怪信息数据库
	bool LoadMonster();
	//加载BOSS怪信息数据库
	bool LoadBoss();
	//加载怪物暴率设定数据库
	bool LoadDropSet();

	bool silentSave();
	bool updateSaveFileVersion();

	void timerEvent(QTimerEvent *event);

private slots:
	void tabChanged(int index);

	void on_mirror_save();
	void enable_autoSave(bool);
	void enable_bkSound(bool);

private:
	Ui::mirrorClass ui;

	RoleInfo roleInfo;			//角色基本信息
	VecRoleSkill m_skill_study;	//角色已学会的技能
	MapItem m_bag_item;			//道具背包
	MapItem m_storage_item;		//道具仓库
	ListEquip m_bag_equip;		//装备背包
	ListEquip m_storage_equip;	//装备仓库

	role *m_tab_role;
	fight *m_tab_fight;
	city *m_tab_city;

	quint32 nSaveTimer;

	QMediaPlayer *bgAudio;
	QMediaPlaylist *bgAudioList;
};

#endif // MIRROR_H
