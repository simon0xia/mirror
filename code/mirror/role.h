#ifndef ROLE_H
#define ROLE_H

#include <QWidget>
#include "ui_role.h"
#include "item_itembag.h"
#include "item_equipbag.h"
#include "item_equipstorage.h"

class role : public QWidget
{
	Q_OBJECT

public:
	role(RoleInfo *roleInfo, MapRoleSkill *skill, MapItem *bag_item, MapItem *storage_item, ListEquip *bag_equip, ListEquip *storage_equip);
	~role();
	const qint32		MaxLv = 1350;

public:
	void updateRoleInfo(void);
	void UpdateEquipInfo(void);
	void UpdateItemInfo(void);

protected:
	//QLabel本身不响应clicked, rightClicked等事件，需要用eventFilter来做。
	bool eventFilter(QObject *obj, QEvent *ev);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

private:
	//显示角色信息到界面
	void DisplayRoleInfo(void);
	//显示角色身上装备
	void DisplayEquip();
	//显示单件装备的详细属性
	void DisplayEquipInfo(QPoint pos, const Info_basic_equip *BasicInfo, const Info_Equip *Equip);
	
	//累加当前装备的属性加成到总属性加成信息。
	void EquipAddPara_Add(const Info_basic_equip &equip, const EquipExtra &extra, quint32 lvUp);
	void EquipAddPara_Sub(const Info_basic_equip &equip, const EquipExtra &extra, quint32 lvUp);

	void Broken32Bit(quint32 nSrc, quint8 &n1, quint8 &n2, quint8 &n3, quint8 &n4);

	void AdjustLevel(qint32 lvl);

private slots:
	void on_btn_mirror_save_clicked();

	void on_btn_bag_equip_clicked();
	void on_btn_bag_item_clicked();
	void on_btn_storage_equip_clicked();

	void on_btn_test_clicked();
	void on_checkBox_bkSound_clicked(void) { emit bkSound(ui.checkBox_bkSound->isChecked()); }
	
	//穿戴装备
	void on_wearEquip(quint32 ID_for_new, quint32 index);
	void on_usedItem(quint32 ID);

signals:
	void mirrorSave(void);
	void bkSound(bool);

private:
	Ui::role ui;
	bool bShifePress;
	QPoint EquipPos[3];

	item_itemBag m_tab_itemBag;
	item_equipBag m_tab_equipBag;
	Item_equipStorage m_tab_equipStorage;

	item_equipBag *m_ptab_equipBag;

	QVector<QLabel *> EquipmentGrid;
	
	RoleInfo *myRole;
	MapRoleSkill *m_skill_study;
	MapItem *m_bag_item;
	MapItem *m_storage_item;
	ListEquip *m_bag_equip;
	ListEquip *m_storage_equip;

	quint32 Role_Lvl;
};

#endif // ROLE_H
