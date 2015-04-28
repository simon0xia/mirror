#ifndef LOGIN_CREATE_H
#define LOGIN_CREATE_H

#include <QDialog>
#include <QMediaPlayer>
#include <QMediaPlaylist>
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

	void changePhoto();

private slots:
	void on_btn_vocation_1_clicked();
	void on_btn_vocation_2_clicked();
	void on_btn_vocation_3_clicked();
	void on_btn_gender_m_clicked();
	void on_btn_gender_f_clicked();
	void on_btn_ok_clicked();

private:
	Ui::login_create ui;
	quint32 vocation;
	quint32 gender;

	bool bCreate;

	QMediaPlayer *bgAudio;
	QMediaPlaylist *bgAudioList;
};

#endif // LOGIN_CREATE_H
