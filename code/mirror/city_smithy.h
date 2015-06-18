#ifndef SMITHY_H
#define SMITHY_H

#include <QWidget>
#include "ui_city_smithy.h"
#include "RoleDefine.h"

class city_smithy : public QWidget
{
	Q_OBJECT

public:
	city_smithy(QWidget *parrent, RoleInfo *roleInfo);
	~city_smithy();

protected:
	void mouseMoveEvent(QMouseEvent * ev);

private slots:
	void on_btn_close_clicked(void);
	void itemClicked(QTreeWidgetItem * item, int column);
// 	void on_btn_m_m_clicked(void);
// 	void on_btn_m1_clicked(void);
// 	void on_btn_m2_clicked(void);
// 	void on_btn_m3_clicked(void);
// 	void on_btn_m4_clicked(void);

private:
	bool DisplayMaterialInfo(itemID id, qint32 itemCount, QPushButton *btn, QLabel *lbl);

private:
	Ui::smithy ui;
	QWidget *m_parrent;

	RoleInfo *myRole;
};

#endif // SMITHY_H
