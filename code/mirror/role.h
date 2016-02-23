#ifndef ROLE_H
#define ROLE_H

#include <QWidget>
#include "ui_role.h"
#include "player.h"

class role : public QWidget
{
	Q_OBJECT

public:
	role(QWidget *parent = 0);
	~role();
public:
	void ChangedEmbodiment(int32_t which);
	//显示信息到界面
	void DisplayInfo(void);

	//显示装备
	void DisplayEquip();
	void DisplayEquip(qint32 index);

protected:
	//QLabel本身不响应clicked, rightClicked等事件，需要用eventFilter来做。
	bool eventFilter(QObject *obj, QEvent *ev);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

private:
	//显示单件装备的详细属性
	//void DisplayEquipInfo(QPoint pos, const Info_basic_equip *BasicInfo, const Info_Equip *Equip);
	
private slots:
	//穿戴装备
	void on_btn_changeFightState_clicked();
	void on_btn_edt_body_clicked();
	void on_btn_edt_warrior_clicked();
	void on_btn_edt_magic_clicked();
	void on_btn_edt_taoshi_clicked();

	void on_btn_xiulian_clicked();

signals:
	void UpdateBag_BagEquip(void);
	void UpdateCoin(void);

private:
	Ui::role ui;
	bool bShifePress;
	QPoint EquipPos[3];

	CHuman *Embodiment;
	int32_t whichEdt;

	QVector<QLabel *> EquipmentGrid;
};

#endif // ROLE_H
