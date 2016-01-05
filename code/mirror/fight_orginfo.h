#ifndef FIGHT_ORGINFO_H
#define FIGHT_ORGINFO_H

#include <QWidget>
#include "ui_fight_orginfo.h"
#include "def_DataType.h"

class fight_OrgInfo : public QWidget
{
	Q_OBJECT

public:
	fight_OrgInfo(QWidget *parent = 0);
	~fight_OrgInfo();
	void DisplayMonsterInfo(QPoint pos, monsterID id);

private:
	Ui::fight_OrgInfo ui;

	qint32 lineSpacing;
};

#endif // FIGHT_ORGINFO_H
