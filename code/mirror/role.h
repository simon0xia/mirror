#ifndef ROLE_H
#define ROLE_H

#include <QWidget>
#include "ui_role.h"

class role : public QWidget
{
	Q_OBJECT

public:
	role(QWidget *parent = 0);
	~role();
	const static qint32
		FileVer = 1,
		MaxLv = 1000;

private:
	void LoadRole(void);
	bool CreateRole(void);
	void DisplayRoleValue();

	void LoadSettings(QString fileName);

private slots:
	void on_btn_mirror_save_clicked();
	void on_btn_role_wisdom_clicked();
	void on_btn_role_spirit_clicked();
	void on_btn_role_life_clicked();
	void on_btn_role_agility_clicked();
	void on_btn_role_lvUp_clicked();

private:
	Ui::role ui;
	QString db_role, db_exp;
	QString name;
	qint64 coin, gold, reputation, exp;
	qint32 level, strength, wisdom, spirit, life, agility, potential;

	//以下为相关设置变量，不可以程序运行时更改。
	qint32 lvExp[MaxLv];
};

#endif // ROLE_H
