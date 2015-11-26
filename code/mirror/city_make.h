#ifndef CITY_MAKE_H
#define CITY_MAKE_H

#include <QWidget>
#include "ui_city_make.h"
#include "dlg_detail.h"

class city_make : public QWidget
{
	Q_OBJECT

public:
	city_make(QWidget *parent, CPlayer *const w_player);
	~city_make();
	void updateInfo(void);

protected:
	void mousePressEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent * ev);
	void mouseReleaseEvent(QMouseEvent *ev);

private slots:
	void on_btn_close_clicked(void);

private:
	Ui::city_make ui;

	CPlayer *const player;
	ListEquip *m_bag_equip;
	MapItem *m_bag_item;

	QPoint mouseMovePos;
};

#endif // CITY_MAKE_H
