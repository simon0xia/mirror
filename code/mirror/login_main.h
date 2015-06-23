#ifndef LOGIN_MAIN_H
#define LOGIN_MAIN_H

#include <QDialog>
#include <QMovie>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "ui_login_main.h"
#include "RoleDefine.h"


class login_main : public QDialog
{
	Q_OBJECT

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

	qint32 timer_main;
	qint32 roleCount;
	RoleInfo roleInfo;

	QMovie *movie;
	qint32 m_roleIndex;
	qint32 nChangeMovieTimer;

	QMediaPlayer *bgAudio;
	QMediaPlaylist *bgAudioList;
};

#endif // LOGIN_MAIN_H
