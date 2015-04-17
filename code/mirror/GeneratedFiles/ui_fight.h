/********************************************************************************
** Form generated from reading UI file 'fight.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIGHT_H
#define UI_FIGHT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_fight
{
public:
    QGridLayout *gridLayout;
    QTabWidget *tabWidget_fight;

    void setupUi(QWidget *fight)
    {
        if (fight->objectName().isEmpty())
            fight->setObjectName(QStringLiteral("fight"));
        fight->resize(880, 600);
        gridLayout = new QGridLayout(fight);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        tabWidget_fight = new QTabWidget(fight);
        tabWidget_fight->setObjectName(QStringLiteral("tabWidget_fight"));

        gridLayout->addWidget(tabWidget_fight, 0, 0, 1, 1);


        retranslateUi(fight);

        tabWidget_fight->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(fight);
    } // setupUi

    void retranslateUi(QWidget *fight)
    {
        fight->setWindowTitle(QApplication::translate("fight", "fight", 0));
    } // retranslateUi

};

namespace Ui {
    class fight: public Ui_fight {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIGHT_H
