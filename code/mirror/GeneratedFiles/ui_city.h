/********************************************************************************
** Form generated from reading UI file 'city.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CITY_H
#define UI_CITY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_city
{
public:
    QLabel *lbl_img;
    QLabel *lbl_LT;
    QLabel *lbl_LB;
    QLabel *lbl_RB;
    QLabel *lbl_RT;
    QTextEdit *edit_display;
    QTabWidget *tabWidget;

    void setupUi(QWidget *city)
    {
        if (city->objectName().isEmpty())
            city->setObjectName(QStringLiteral("city"));
        city->resize(872, 600);
        lbl_img = new QLabel(city);
        lbl_img->setObjectName(QStringLiteral("lbl_img"));
        lbl_img->setGeometry(QRect(10, 16, 250, 250));
        lbl_LT = new QLabel(city);
        lbl_LT->setObjectName(QStringLiteral("lbl_LT"));
        lbl_LT->setGeometry(QRect(0, 0, 12, 12));
        lbl_LB = new QLabel(city);
        lbl_LB->setObjectName(QStringLiteral("lbl_LB"));
        lbl_LB->setGeometry(QRect(0, 584, 12, 12));
        lbl_RB = new QLabel(city);
        lbl_RB->setObjectName(QStringLiteral("lbl_RB"));
        lbl_RB->setGeometry(QRect(860, 584, 12, 12));
        lbl_RT = new QLabel(city);
        lbl_RT->setObjectName(QStringLiteral("lbl_RT"));
        lbl_RT->setGeometry(QRect(860, 0, 12, 12));
        edit_display = new QTextEdit(city);
        edit_display->setObjectName(QStringLiteral("edit_display"));
        edit_display->setGeometry(QRect(10, 280, 251, 297));
        edit_display->setReadOnly(true);
        tabWidget = new QTabWidget(city);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(270, 8, 591, 569));

        retranslateUi(city);

        QMetaObject::connectSlotsByName(city);
    } // setupUi

    void retranslateUi(QWidget *city)
    {
        city->setWindowTitle(QApplication::translate("city", "city", 0));
        lbl_img->setText(QApplication::translate("city", "\345\237\216\345\270\202", 0));
        lbl_LT->setText(QApplication::translate("city", "LT", 0));
        lbl_LB->setText(QApplication::translate("city", "LB", 0));
        lbl_RB->setText(QApplication::translate("city", "RB", 0));
        lbl_RT->setText(QApplication::translate("city", "RT", 0));
    } // retranslateUi

};

namespace Ui {
    class city: public Ui_city {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CITY_H
