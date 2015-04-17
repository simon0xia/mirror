/********************************************************************************
** Form generated from reading UI file 'storage_item.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STORAGE_ITEM_H
#define UI_STORAGE_ITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_storage_item
{
public:

    void setupUi(QWidget *storage_item)
    {
        if (storage_item->objectName().isEmpty())
            storage_item->setObjectName(QStringLiteral("storage_item"));
        storage_item->resize(400, 300);

        retranslateUi(storage_item);

        QMetaObject::connectSlotsByName(storage_item);
    } // setupUi

    void retranslateUi(QWidget *storage_item)
    {
        storage_item->setWindowTitle(QApplication::translate("storage_item", "storage_item", 0));
    } // retranslateUi

};

namespace Ui {
    class storage_item: public Ui_storage_item {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STORAGE_ITEM_H
