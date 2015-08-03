#ifndef DLG_DETAIL_H
#define DLG_DETAIL_H

#include <QDialog>
#include "ui_dlg_detail.h"
#include "def_item_equip.h"
#include "ItemDefine.h"
#include "RoleDefine.h"
#include "CommonComponents.h"

class Dlg_Detail : public QDialog
{
	Q_OBJECT

public:
	Dlg_Detail(QWidget *parent = 0);
	~Dlg_Detail();
	//显示装备的详细信息
	void DisplayEquipInfo(QPoint pos, const Info_basic_equip *BasicInfo, const Info_Equip *Equip, const RoleInfo *roleInfo);
	void DisplayItemInfo(QPoint pos, const Info_Item *item, quint32 no, quint32 role_voc, quint32 role_lvl);

private slots:
	void on_btn_quit_clicked();

private:
	Ui::dlg_detail ui;
};

#endif // DLG_DETAIL_H
