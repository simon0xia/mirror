#ifndef GENERAL_GAMEINTRODUCE_H
#define GENERAL_GAMEINTRODUCE_H

#include <QWidget>
#include "ui_general_gameintroduce.h"

class General_GameIntroduce : public QWidget
{
	Q_OBJECT

public:
	General_GameIntroduce(QWidget *parent = 0);
	~General_GameIntroduce();

private:
	Ui::General_GameIntroduce ui;
};

#endif // GENERAL_GAMEINTRODUCE_H
