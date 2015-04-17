#ifndef DRUGS_H
#define DRUGS_H

#include <QWidget>
#include "ui_city_drugs.h"
#include "ItemDefine.h"
#include "RoleDefine.h"

class drugs : public QWidget
{
	Q_OBJECT

public:
	drugs(RoleInfo *roleInfo,MapItem *bag_item);
	~drugs();

private:
	void AdjustTableWidget(void);
	bool DisplayItemList(void);

	

private slots:
	void cellClicked(int row, int column);

private:
	Ui::drugs ui;

	RoleInfo *myRole;
	MapItem *m_bag_item;
};

#endif // DRUGS_H
