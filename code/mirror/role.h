#ifndef ROLE_H
#define ROLE_H

#include <QWidget>
#include "ui_role.h"
#include "mytabframe.h"
#include "item_itembag.h"
#include "item_equipbag.h"
#include "item_equipstorage.h"

class role : public myTabFrame
{
	Q_OBJECT

public:
	role(RoleInfo *roleInfo, MapItem *bag_item, MapItem *storage_item, ListEquip *bag_equip, ListEquip *storage_equip);
	~role();
	const static qint32		MaxLv = 100;

public:
	virtual void updateRoleInfo(void);
	void UpdateEquipInfo(void);
	void UpdateItemInfo(void);

protected:
	//QLabel本身不响应clicked, rightClicked等事件，需要用eventFilter来做。
	bool eventFilter(QObject *obj, QEvent *ev);

private:
	//显示角色信息到界面
	void DisplayRoleInfo(void);
	//显示角色身上装备
	void DisplayEquip();
	//显示单件装备的详细属性
	void DisplayEquipInfo(QPoint pos, const Info_equip &equip);
	
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

	//穿戴装备
	void on_wearEquip(quint32 ID_for_new, quint32 index);
	void on_usedItem(quint32 ID);

private:
	Ui::role ui;

	item_itemBag m_tab_itemBag;
	item_equipBag m_tab_equipBag;
	Item_equipStorage m_tab_equipStorage;

//	Dlg_Detail *m_dlg_detail;

	QVector<QLabel *> EquipmentGrid;
	
	RoleInfo *myRole;
	MapItem *m_bag_item;
	MapItem *m_storage_item;
	ListEquip *m_bag_equip;
	ListEquip *m_storage_equip;

	Info_equip equip_add;
};

#endif // ROLE_H
