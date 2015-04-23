#ifndef EQUIPINFO_H
#define EQUIPINFO_H

#include <QWidget>
#include "ui_equipinfo.h"
#include "def_item_equip.h"
#include "RoleDefine.h"

class EquipInfo : public QWidget
{
	Q_OBJECT

public:
	EquipInfo(QWidget *parent = 0);
	~EquipInfo();
	//显示装备的详细信息
	void updateInfo(QPoint pos, const Info_equip &equip, const RoleInfo &Role);

private:
	Ui::EquipInfo ui;
};

#endif // EQUIPINFO_H
