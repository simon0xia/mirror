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

	void TryConnect();

private:
	void Sign(QDateTime dt);
	void timerEvent(QTimerEvent *event);
	void GiveExtraReward(QDateTime curDate);
	QString extarRewardInfo(void);

private:
	Ui::dlg_sign ui;
	qint32 trys;
	QStringList sl;
	qint32 retryTimer;

	QUdpSocket *udpsocket;
};

#endif // DLG_SIGN_H
