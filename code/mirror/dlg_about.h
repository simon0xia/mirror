#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include "ui_about.h"

class dlg_about : public QDialog
{
	Q_OBJECT

public:
	dlg_about(QWidget *parent = 0);
	~dlg_about();

private:
	Ui::about ui;
};

#endif // ABOUT_H
