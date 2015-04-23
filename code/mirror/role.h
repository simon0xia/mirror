#ifndef ROLE_H
#define ROLE_H

#include <QWidget>
#include "ui_role.h"
#include "mytabframe.h"
#include "bag_item.h"
#include "storage_item.h"

#include "def_item_equip.h"
#include "equipinfo.h"

class role : public myTabFrame
{
	Q_OBJECT

public:
	role(RoleInfo *roleInfo, MapItem *bag_item, MapItem *storage_item);
	~role();
	const static qint32		MaxLv = 100;

public:
	virtual void updateRoleInfo(void);

protected:
	//QLabel本身不响应clicked, rightClicked等事件，需要用eventFilter来做。
	bool eventFilter(QObject *obj, QEvent *ev);

private:
	//加载存档
	void LoadRole(void);
	//创建存档
//	bool CreateRole(void);
	//显示角色信息到界面
	void DisplayRoleInfo(void);
	//显示角色身上装备
	void DisplayEquip();
	//显示单件装备的详细属性
	void DisplayEquipInfo(QPoint pos, const Info_equip &equip);
	//根据装备ID在全局装备列表中查询并指定装备的详细属性
	const Info_equip *FineEquip(quint32 id);
	//加载升级经验设置信息数据库
	void LoadExpSetting();
	//累加当前装备的属性加成到总属性加成信息。
	void Add_EquipAddPara(const Info_equip &equip);
	void Sub_EquipAddPara(const Info_equip &equip);

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

	bag_item m_tab_bagItem;
	storage_item m_tab_storageItem;
	EquipInfo *m_dlg_equipInfo;

	QVector<QLabel *> EquipmentGrid;
	
	RoleInfo *myRole;
	MapItem *m_bag_item;
	MapItem *m_storage_item;

	Info_equip equip_add;
};

#endif // ROLE_H
