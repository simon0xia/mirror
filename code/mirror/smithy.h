#ifndef SMITHY_H
#define SMITHY_H

#include <QWidget>
#include "ui_smithy.h"
#include "def_item_equip.h"

class smithy : public QWidget
{
	Q_OBJECT

public:
	smithy(QWidget *parent = 0);
	~smithy();

	void updateInfo(void);

	bool CheckAndDisplayPrice(qint32 equipLv, qint32 equipLvUp);
	bool CheckAndDisplayEquip();
	bool CheckAndDisplayM1(qint32 equipLvUp);

private slots :
	void RemoveEquip();
	void on_btn_make_clicked(void);

protected:
	bool eventFilter(QObject *obj, QEvent *ev);

signals:
	void UpdateBag_BagEquip(void);
	void UpdateCoin(void);

private:
	Ui::smithy ui;

	qint32 price;
	Info_Equip onMakeEquip;
};

#endif // SMITHY_H
