#ifndef DLG_SIGN_H
#define DLG_SIGN_H

#include <QDialog>
#include "ui_dlg_sign.h"
#include <QUdpSocket>
#include <QDateTime>

class dlg_sign : public QDialog
{
	Q_OBJECT

public:
	dlg_sign(QWidget *parent = 0);
	~dlg_sign();

private slots:
	void readingDataGrams();
	void connectsucess();
	void socketError();

	void on_btn_ok_clicked(void);
	void on_btn_retry_clicked(void);

private:
	void Sign(QDateTime dt);

private:
	Ui::dlg_sign ui;
	qint32 trys;
	QStringList sl;

	QUdpSocket *udpsocket;
};

#endif // DLG_SIGN_H
