#ifndef TASK_H
#define TASK_H

#include <QDialog>
#include "ui_task.h"

class task : public QDialog
{
	Q_OBJECT

public:
	task(QWidget *parent = 0);
	~task();

private slots:
	void itemClicked(QTreeWidgetItem * item, int column);

private:
	Ui::task ui;
};

#endif // TASK_H
