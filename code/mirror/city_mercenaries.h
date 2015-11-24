#ifndef CITY_MERCENARIES_H
#define CITY_MERCENARIES_H

#include <QWidget>
#include "ui_city_mercenaries.h"
#include "dlg_detail.h"

class city_Mercenaries : public QWidget
{
	Q_OBJECT
public:

public:
	city_Mercenaries(QWidget *parent, CPlayer *const w_player);
	~city_Mercenaries();
	void updateInfo(void);

protected:
	void mousePressEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent * ev);
	void mouseReleaseEvent(QMouseEvent *ev);

private slots:
	void on_btn_close_clicked(void);
	void on_btn_buy_clicked(void);

private:
	Ui::city_Mercenaries ui;

	CPlayer *const player;
	ListEquip *m_bag_equip;

	QPoint mouseMovePos;
};

#endif // CITY_MERCENARIES_H
