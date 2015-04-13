#ifndef STORAGE_ITEM_H
#define STORAGE_ITEM_H

#include <QWidget>
#include "ui_storage_item.h"

class storage_item : public QWidget
{
	Q_OBJECT

public:
	storage_item(QWidget *parent = 0);
	~storage_item();

private:
	Ui::storage_item ui;
};

#endif // STORAGE_ITEM_H
