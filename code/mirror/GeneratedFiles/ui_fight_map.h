/********************************************************************************
** Form generated from reading UI file 'fight_map.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIGHT_MAP_H
#define UI_FIGHT_MAP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_fight_map
{
public:
    QGridLayout *gridLayout;
    QListWidget *listWidget;

    void setupUi(QWidget *fight_map)
    {
        if (fight_map->objectName().isEmpty())
            fight_map->setObjectName(QStringLiteral("fight_map"));
        fight_map->resize(880, 600);
        gridLayout = new QGridLayout(fight_map);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        listWidget = new QListWidget(fight_map);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setIconSize(QSize(120, 120));
        listWidget->setViewMode(QListView::IconMode);

        gridLayout->addWidget(listWidget, 0, 0, 1, 1);


        retranslateUi(fight_map);

        QMetaObject::connectSlotsByName(fight_map);
    } // setupUi

    void retranslateUi(QWidget *fight_map)
    {
        fight_map->setWindowTitle(QApplication::translate("fight_map", "fight_map", 0));
    } // retranslateUi

};

namespace Ui {
    class fight_map: public Ui_fight_map {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIGHT_MAP_H
