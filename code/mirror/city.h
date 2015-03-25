#ifndef CITY_H
#define CITY_H

#include <QWidget>
#include "ui_city.h"

class city : public QWidget
{
	Q_OBJECT

public:
	city(QWidget *parent = 0);
	~city();

private:
	Ui::city ui;
};

#endif // CITY_H
