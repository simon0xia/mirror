/********************************************************************************
** Form generated from reading UI file 'city_smithy.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CITY_SMITHY_H
#define UI_CITY_SMITHY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_smithy
{
public:

    void setupUi(QWidget *smithy)
    {
        if (smithy->objectName().isEmpty())
            smithy->setObjectName(QStringLiteral("smithy"));
        smithy->resize(400, 300);

        retranslateUi(smithy);

        QMetaObject::connectSlotsByName(smithy);
    } // setupUi

    void retranslateUi(QWidget *smithy)
    {
        smithy->setWindowTitle(QApplication::translate("smithy", "smithy", 0));
    } // retranslateUi

};

namespace Ui {
    class smithy: public Ui_smithy {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CITY_SMITHY_H
