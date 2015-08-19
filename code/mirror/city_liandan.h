#ifndef CITY_LIANDAN_H
#define CITY_LIANDAN_H

#include <QWidget>
#include "ui_city_liandan.h"
#include "Player.h"

class city_liandan : public QWidget
{
	Q_OBJECT

public:
	city_liandan(QWidget *parent, CPlayer *const w_player);
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

	CPlayer *const player;
	MapItem *m_bag_item;

	info_formula_liandan elixir;
};

#endif // CITY_LIANDAN_H
