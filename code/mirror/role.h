#ifndef ROLE_H
#define ROLE_H

#include <QWidget>
#include "ui_role.h"
#include "mytabframe.h"
#include "bag_item.h"
#include "storage_item.h"

#include "def_item_equip.h"

class role : public myTabFrame
{
	Q_OBJECT

public:
	role(RoleInfo *roleInfo, MapItem *bag_item, MapItem *storage_item);
	~role();
	const static qint32
		FileVer = 3,
		MaxLv = 100;

public:
	virtual void updateRoleInfo(void);

private:
	//加载存档
	void LoadRole(void);
	//创建存档
	bool CreateRole(void);
	//显示角色信息到界面
	void DisplayRoleInfo(void);
	//显示角色身上装备
	void DisplayEquip();
	//计算角色身上装备的属性加成
//	void CaclEquipAdd(Info_equip &e);
	//加载升级经验设置信息数据库
	void LoadExpSetting();

private slots:
	void on_btn_mirror_save_clicked();
	void on_btn_role_strength_clicked();
	void on_btn_role_wisdom_clicked();
	void on_btn_role_spirit_clicked();
	void on_btn_role_life_clicked();
	void on_btn_role_agility_clicked();
	void on_btn_role_lvUp_clicked();

private:
	Ui::role ui;
	QString db_role, db_exp;

	bag_item m_tab_bagItem;
	storage_item m_tab_storageItem;
	
	RoleInfo *myRole;
	MapItem *m_bag_item;
	MapItem *m_storage_item;

	QVector<Info_jobAdd> vecJobAdd;
	QVector<EquitExtra> roleEquip;

	Info_equip equip_add;
};

#endif // ROLE_H
