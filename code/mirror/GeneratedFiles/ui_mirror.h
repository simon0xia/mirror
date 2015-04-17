/********************************************************************************
** Form generated from reading UI file 'mirror.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MIRROR_H
#define UI_MIRROR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mirrorClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget_main;

    void setupUi(QMainWindow *mirrorClass)
    {
        if (mirrorClass->objectName().isEmpty())
            mirrorClass->setObjectName(QStringLiteral("mirrorClass"));
        mirrorClass->resize(880, 625);
        centralWidget = new QWidget(mirrorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        tabWidget_main = new QTabWidget(centralWidget);
        tabWidget_main->setObjectName(QStringLiteral("tabWidget_main"));

        gridLayout->addWidget(tabWidget_main, 0, 0, 1, 1);

        mirrorClass->setCentralWidget(centralWidget);

        retranslateUi(mirrorClass);

        tabWidget_main->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(mirrorClass);
    } // setupUi

    void retranslateUi(QMainWindow *mirrorClass)
    {
        mirrorClass->setWindowTitle(QApplication::translate("mirrorClass", "mirror", 0));
    } // retranslateUi

};

namespace Ui {
    class mirrorClass: public Ui_mirrorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MIRROR_H
