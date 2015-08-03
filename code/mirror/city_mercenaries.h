#ifndef CITY_MERCENARIES_H
#define CITY_MERCENARIES_H

#include <QWidget>
#include "ui_city_mercenaries.h"
#include "RoleDefine.h"
#include "dlg_detail.h"

class city_Mercenaries : public QWidget
{
	Q_OBJECT
public:

public:
	city_Mercenaries(QWidget *parent, RoleInfo *roleInfo, ListEquip *bag_equip);
	~city_Mercenaries();
	void updateInfo(void);

protected:
	void mouseMoveEvent(QMouseEvent * ev);

private slots:
	void on_btn_close_clicked(void);
	void on_btn_buy_clicked(void);

private:
	Ui::city_Mercenaries ui;
	QWidget *m_parrent;

	RoleInfo *myRole;
	ListEquip *m_bag_equip;

	Dlg_Detail *detailDlg;
};

#endif // CITY_MERCENARIES_H
