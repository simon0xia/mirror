#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include "ui_about.h"

class about : public QDialog
{
	Q_OBJECT

public:
	about(QWidget *parent = 0);
	~about();

private:
	Ui::about ui;
};

#endif // ABOUT_H
