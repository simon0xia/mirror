#ifndef MIRROR_H
#define MIRROR_H

#include <QtWidgets/QMainWindow>
#include "ui_mirror.h"

class mirror : public QMainWindow
{
	Q_OBJECT

public:
	mirror(QWidget *parent = 0);
	~mirror();

private:
	Ui::mirrorClass ui;
};

#endif // MIRROR_H
