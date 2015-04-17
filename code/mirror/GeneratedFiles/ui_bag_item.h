/********************************************************************************
** Form generated from reading UI file 'bag_item.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BAG_ITEM_H
#define UI_BAG_ITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_bag_item
{
public:
    QGridLayout *gridLayout_2;
    QTableWidget *tableWidget;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_pgDn;
    QPushButton *btn_pgUp;
    QLineEdit *edit_page_all;
    QLabel *label_2;
    QLineEdit *edit_page_cur;
    QLabel *label;
    QPushButton *btn_sale;
    QPushButton *btn_storage;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QWidget *bag_item)
    {
        if (bag_item->objectName().isEmpty())
            bag_item->setObjectName(QStringLiteral("bag_item"));
        bag_item->resize(590, 280);
        gridLayout_2 = new QGridLayout(bag_item);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        tableWidget = new QTableWidget(bag_item);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->horizontalHeader()->setVisible(false);
        tableWidget->verticalHeader()->setVisible(false);

        gridLayout_2->addWidget(tableWidget, 0, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalSpacer = new QSpacerItem(75, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        btn_pgDn = new QPushButton(bag_item);
        btn_pgDn->setObjectName(QStringLiteral("btn_pgDn"));
        btn_pgDn->setEnabled(false);

        gridLayout->addWidget(btn_pgDn, 0, 9, 1, 1);

        btn_pgUp = new QPushButton(bag_item);
        btn_pgUp->setObjectName(QStringLiteral("btn_pgUp"));
        btn_pgUp->setEnabled(false);

        gridLayout->addWidget(btn_pgUp, 0, 8, 1, 1);

        edit_page_all = new QLineEdit(bag_item);
        edit_page_all->setObjectName(QStringLiteral("edit_page_all"));
        edit_page_all->setMaximumSize(QSize(15, 16777215));

        gridLayout->addWidget(edit_page_all, 0, 7, 1, 1);

        label_2 = new QLabel(bag_item);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 6, 1, 1);

        edit_page_cur = new QLineEdit(bag_item);
        edit_page_cur->setObjectName(QStringLiteral("edit_page_cur"));
        edit_page_cur->setMaximumSize(QSize(15, 16777215));

        gridLayout->addWidget(edit_page_cur, 0, 5, 1, 1);

        label = new QLabel(bag_item);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 4, 1, 1);

        btn_sale = new QPushButton(bag_item);
        btn_sale->setObjectName(QStringLiteral("btn_sale"));
        btn_sale->setEnabled(false);

        gridLayout->addWidget(btn_sale, 0, 2, 1, 1);

        btn_storage = new QPushButton(bag_item);
        btn_storage->setObjectName(QStringLiteral("btn_storage"));
        btn_storage->setEnabled(false);

        gridLayout->addWidget(btn_storage, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 3, 1, 1);


        gridLayout_2->addLayout(gridLayout, 1, 0, 1, 1);


        retranslateUi(bag_item);

        QMetaObject::connectSlotsByName(bag_item);
    } // setupUi

    void retranslateUi(QWidget *bag_item)
    {
        bag_item->setWindowTitle(QApplication::translate("bag_item", "bag_item", 0));
        btn_pgDn->setText(QApplication::translate("bag_item", "\344\270\213\344\270\200\351\241\265", 0));
        btn_pgUp->setText(QApplication::translate("bag_item", "\344\270\212\344\270\200\351\241\265", 0));
        label_2->setText(QApplication::translate("bag_item", "/", 0));
        label->setText(QApplication::translate("bag_item", "\351\241\265", 0));
        btn_sale->setText(QApplication::translate("bag_item", "\344\270\200\351\224\256\345\215\226\345\207\272", 0));
        btn_storage->setText(QApplication::translate("bag_item", "\344\270\200\351\224\256\345\205\245\345\272\223", 0));
    } // retranslateUi

};

namespace Ui {
    class bag_item: public Ui_bag_item {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BAG_ITEM_H
