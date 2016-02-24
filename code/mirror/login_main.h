#ifndef LOGIN_MAIN_H
#define LOGIN_MAIN_H

#include <QDialog>
#include <QFile>
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
	bool DisplayBasicInfo(void);
	void ShowUnSelectMovie();
	void ShowSelectMovie();

	bool ReadBasicInfo();
	bool updateSaveFileVersion();
	bool CheckSaveFile();

	void Load();
	bool LoadEnvir(QString &str);

private slots:
	void on_btn_1_select_clicked();
	void on_btn_2_select_clicked();
	void on_btn_start_clicked();
	void on_btn_create_clicked();
	void on_btn_delect_clicked();
	void on_btn_quit_clicked();

private:
	Ui::login_main ui;
	bool bWasLoad, bLoadSuccess;

	qint32 timer_main, timer_Load, nChangeMovieTimer;
	qint32 roleCount;

	qint32 ver_file, ver_major, ver_minor, ver_build;
	char m_rolename[128];
	qint32 m_gender, m_vocation;
	qint32 m_level;

	QMovie *movie;
	qint32 m_roleIndex;

	QMediaPlayer *bgAudio;
	QMediaPlaylist *bgAudioList;
};

#endif // LOGIN_MAIN_H
