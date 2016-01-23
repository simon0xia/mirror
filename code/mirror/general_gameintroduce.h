#ifndef GENERAL_GAMEINTRODUCE_H
#define GENERAL_GAMEINTRODUCE_H

#include <QDialog>
#include "ui_general_gameintroduce.h"

class General_GameIntroduce : public QDialog
{
	Q_OBJECT

public:
	General_GameIntroduce(QDialog *parent = 0);
	~General_GameIntroduce();

private:
	Ui::General_GameIntroduce ui;
};

#endif // GENERAL_GAMEINTRODUCE_H
