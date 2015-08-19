#ifndef SMITHY_H
#define SMITHY_H

#include <QWidget>
#include "ui_city_smithy.h"
#include "Player.h"

class city_smithy : public QWidget
{
	Q_OBJECT

public:
	city_smithy(QWidget *parrent, CPlayer *const w_player);
	~city_smithy();

protected:
	void mouseMoveEvent(QMouseEvent * ev);
	bool eventFilter(QObject *obj, QEvent *ev);

private slots:
	void on_btn_close_clicked(void);
	void itemClicked(QTreeWidgetItem * item, int column);
// 	void on_btn_m_m_clicked(void);
// 	void on_btn_m1_clicked(void);
// 	void on_btn_m2_clicked(void);
// 	void on_btn_m3_clicked(void);
// 	void on_btn_m4_clicked(void);
	void on_btn_make_clicked(void);

private:
	bool DisplayMaterialInfo(itemID id, qint32 itemCount, QPushButton *btn, QLabel *lbl);
	bool DisplayMaterialInfo(itemID id, QPushButton *btn);

private:
	Ui::smithy ui;
	QWidget *m_parrent;

	CPlayer *const player;
	MapItem *m_bag_item;
	ListEquip *m_bag_equip;
};

#endif // SMITHY_H
