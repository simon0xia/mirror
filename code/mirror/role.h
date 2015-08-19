#ifndef ROLE_H
#define ROLE_H

#include <QWidget>
#include "ui_role.h"
#include "item_equipbag.h"
#include "item_equipstorage.h"
#include "item_itembag.h"

class role : public QWidget
{
	Q_OBJECT

public:
	role(CPlayer *w_player);
	~role();
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

	CPlayer *player;
	item_itemBag m_tab_itemBag;
	item_equipBag m_tab_equipBag;
	Item_equipStorage m_tab_equipStorage;

	QVector<QLabel *> EquipmentGrid;
};

#endif // ROLE_H
