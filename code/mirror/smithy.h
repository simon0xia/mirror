#ifndef SMITHY_H
#define SMITHY_H

#include <QWidget>
#include "ui_city_smithy.h"

class smithy : public QWidget
{
	Q_OBJECT

public:
	smithy(QWidget *parent = 0);
	~smithy();

private:
	Ui::smithy ui;
};

#endif // SMITHY_H
