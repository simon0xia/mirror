#ifndef CITY_LIANDAN_H
#define CITY_LIANDAN_H

#include <QWidget>
#include "ui_city_liandan.h"
#include "RoleDefine.h"
#include "ItemDefine.h"

class city_liandan : public QWidget
{
	Q_OBJECT

public:
	city_liandan(QWidget *parent, RoleInfo *roleInfo, MapItem *bag_item);
	~city_liandan();
	void updateInfo(void);

protected:
	void mouseMoveEvent(QMouseEvent * ev);

private slots:
	void on_btn_close_clicked(void);
	void on_btn_make_clicked(void);

private:
	bool DisplayMaterialInfo(itemID id, qint32 itemCount, QPushButton *btn, QLabel *lbl);

private:
	Ui::city_liandan ui;
	QWidget *m_parrent;

	RoleInfo *myRole;
	MapItem *m_bag_item;

	info_formula_liandan elixir;
};

#endif // CITY_LIANDAN_H
