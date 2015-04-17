/********************************************************************************
** Form generated from reading UI file 'dlg_count.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLG_COUNT_H
#define UI_DLG_COUNT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_dlg_count
{
public:
    QLineEdit *lineEdit;
    QPushButton *btn_max;
    QPushButton *btn_ok;
    QPushButton *btn_cancel;

    void setupUi(QWidget *dlg_count)
    {
        if (dlg_count->objectName().isEmpty())
            dlg_count->setObjectName(QStringLiteral("dlg_count"));
        dlg_count->resize(251, 113);
        lineEdit = new QLineEdit(dlg_count);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(20, 20, 211, 31));
        QFont font;
        font.setPointSize(12);
        lineEdit->setFont(font);
        lineEdit->setMaxLength(4);
        btn_max = new QPushButton(dlg_count);
        btn_max->setObjectName(QStringLiteral("btn_max"));
        btn_max->setGeometry(QRect(10, 70, 75, 23));
        btn_ok = new QPushButton(dlg_count);
        btn_ok->setObjectName(QStringLiteral("btn_ok"));
        btn_ok->setGeometry(QRect(90, 70, 75, 23));
        btn_ok->setDefault(true);
        btn_cancel = new QPushButton(dlg_count);
        btn_cancel->setObjectName(QStringLiteral("btn_cancel"));
        btn_cancel->setGeometry(QRect(170, 70, 75, 23));

        retranslateUi(dlg_count);

        QMetaObject::connectSlotsByName(dlg_count);
    } // setupUi

    void retranslateUi(QWidget *dlg_count)
    {
        dlg_count->setWindowTitle(QApplication::translate("dlg_count", "dlg_count", 0));
        lineEdit->setInputMask(QApplication::translate("dlg_count", "[0000]", 0));
        btn_max->setText(QApplication::translate("dlg_count", "\346\234\200\345\244\247", 0));
        btn_ok->setText(QApplication::translate("dlg_count", "\347\241\256\345\256\232\350\276\223\345\205\245", 0));
        btn_cancel->setText(QApplication::translate("dlg_count", "\345\217\226\346\266\210", 0));
    } // retranslateUi

};

namespace Ui {
    class dlg_count: public Ui_dlg_count {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLG_COUNT_H
