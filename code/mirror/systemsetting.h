#ifndef SYSTEMSETTING_H
#define SYSTEMSETTING_H

#include <QDialog>
#include "ui_systemsetting.h"

class SystemSetting : public QDialog
{
	Q_OBJECT

public:
	SystemSetting(QWidget *parent = 0);
	~SystemSetting();

private slots:
	void on_btn_close_clicked(void);

private:
	void ReadSystemConfigure(void);
	void WriteSystemConfigure(void);
	void updateObjectStatus(void);

private:
	Ui::SystemSetting ui;



	qint32 pickFilter, bgAudio, skillAudio;
};

#endif // SYSTEMSETTING_H
