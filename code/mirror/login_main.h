#ifndef LOGIN_MAIN_H
#define LOGIN_MAIN_H

#include <QDialog>
#include <QMovie>
#include "ui_login_main.h"
#include "RoleDefine.h"


class login_main : public QDialog
{
	Q_OBJECT

public:
	//每段允许最大值为999
	const qint32 version_major = 0,
		version_minor = 2,
		version_build = 0;
public:
	login_main(QWidget *parent = 0);
	~login_main();
	qint32 getSelectRole() { return m_roleIndex; }

private:
	void timerEvent(QTimerEvent *event);
	bool loadAndDisplay_BasicRoleInfo(void);
	void ShowUnSelectMovie();
	void ShowSelectMovie();

private slots:
	void on_btn_1_select_clicked();
	void on_btn_2_select_clicked();
	void on_btn_start_clicked();
	void on_btn_create_clicked();
	void on_btn_quit_clicked();

private:
	Ui::login_main ui;

	RoleInfo roleInfo;

	QMovie *movie;
	qint32 m_roleIndex;
	qint32 nChangeMovieTimer;
};

#endif // LOGIN_MAIN_H
