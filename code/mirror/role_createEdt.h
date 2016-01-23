#ifndef ROLE_CREATEEDT_H
#define ROLE_CREATEEDT_H

#include <QDialog>
#include <QMouseEvent>
#include "ui_role_createedt.h"
#include "def_DataType.h"

class role_createEdt : public QDialog
{
	Q_OBJECT

public:
	role_createEdt(QWidget *parent, Vocation w_voc);
	~role_createEdt();

private slots:
	void on_btn_ok_clicked(void);
	void on_btn_quit_clicked(void);

private:
	Ui::role_createEdt ui;

	Vocation voc;
};

#endif // ROLE_CREATEEDT_H
