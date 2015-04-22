#ifndef LOGIN_CREATE_H
#define LOGIN_CREATE_H

#include <QDialog>
#include "ui_login_create.h"

class login_create : public QDialog
{
	Q_OBJECT

public:
	login_create(QWidget *parent = 0);
	~login_create();
	bool getCreateRes() { return bCreate; }

private:
	//´´½¨´æµµ
	bool CreateRole(const QString &name);

private slots:
	void on_btn_role_1_clicked();
	void on_btn_role_2_clicked();
	void on_btn_role_3_clicked();
	void on_btn_role_4_clicked();
	void on_btn_role_5_clicked();
	void on_btn_role_6_clicked();
	void on_btn_ok_clicked();

private:
	Ui::login_create ui;
	quint32 vocation;
	quint32 gender;

	bool bCreate;
};

#endif // LOGIN_CREATE_H
