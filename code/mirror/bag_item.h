#ifndef BAG_ITEM_H
#define BAG_ITEM_H

#include <QWidget>
#include "ui_bag_item.h"
#include "ItemDefine.h"

class bag_item : public QWidget
{
	Q_OBJECT

public:
	bag_item(QMap<quint32,quint32> *item);
	~bag_item();

private:
	void testBag();

private:
	Ui::bag_item ui;

	QMap<quint32, quint32> *m_item;
};

#endif // BAG_ITEM_H
