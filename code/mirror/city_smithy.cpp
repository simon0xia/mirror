#include "city_smithy.h"
#include <QTreeWidgetItem>
#include <QMouseEvent>
#include <qt_windows.h>
#include "ItemDefine.h"
#include "Item_Base.h"

extern QVector<Info_Item> g_ItemList;
extern QVector<Info_basic_equip> g_EquipList;
extern QMap<itemID, info_formula> g_formula;

city_smithy::city_smithy(QWidget *parrent, RoleInfo *roleInfo)
	: QWidget(parrent), myRole(roleInfo)
{
	ui.setupUi(this);
	m_parrent = parrent;

	QString str[3] = { QStringLiteral("±¶ Ø"), QStringLiteral("ª§∂‹"), QStringLiteral("ªÍ÷È") };
	qint32 typeDef[3] = { 11, 12, 13 };

	qint32 ItemType;
	for (qint32 i = 0; i < 3; i++)
	{
		QTreeWidgetItem *propType = new QTreeWidgetItem(ui.treeWidget);
		propType->setText(0, str[i]);

		foreach(const info_formula &f, g_formula)
		{
			ItemType = (f.ID - g_itemID_start_equip) / 1000;

			if (ItemType == typeDef[i] && f.npc == 1)
			{
				const Info_basic_equip *equip = Item_Base::GetEquipBasicInfo(f.ID);
				if (equip != nullptr)
				{
					QTreeWidgetItem *propItem = new QTreeWidgetItem(propType);
					propItem->setIcon(0, equip->icon);
					propItem->setText(0, equip->name);
					propItem->setText(1, QString::number(f.ID));
				}
			}
		}
	}	

	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(itemClicked(QTreeWidgetItem *, int)));
}

city_smithy::~city_smithy()
{

}

void city_smithy::on_btn_close_clicked(void)
{
	close();
}

void city_smithy::mouseMoveEvent(QMouseEvent * ev)
{
	QPoint point = m_parrent->mapFromGlobal(ev->globalPos()) - QPoint(230, 30);
	move(point);
}

void city_smithy::itemClicked(QTreeWidgetItem * item, int column)
{
	itemID id = item->text(1).toUInt();
	if (!g_formula.contains(id))
	{
		//has error;
		return;
	}
	const info_formula &f = g_formula[id];

	ui.btn_m_m->setIcon(item->icon(0));
	ui.lbl_name->setText(item->text(0));

	ui.btn_m1->setEnabled(false);
	ui.btn_m2->setEnabled(false);
	ui.btn_m3->setEnabled(false);
	ui.btn_m4->setEnabled(false);
	
	DisplayMaterialInfo(f.m1_t, f.m1_c, ui.btn_m1, ui.lbl_m1);
	if (f.m2_t != 0)
	{
		DisplayMaterialInfo(f.m2_t, f.m2_c, ui.btn_m2, ui.lbl_m2);
	}
	
	if (f.m3_t != 0)
	{
		DisplayMaterialInfo(f.m3_t, f.m3_c, ui.btn_m3, ui.lbl_m3);
	}
	
	if (f.m4_t != 0)
	{
		DisplayMaterialInfo(f.m4_t, f.m4_c, ui.btn_m4, ui.lbl_m4);
	}
}

bool city_smithy::DisplayMaterialInfo(itemID id, qint32 itemCount, QPushButton *btn, QLabel *lbl)
{
	if (id > g_itemID_start_equip && id <= g_itemID_stop_equip)
	{
		const Info_basic_equip *equip = Item_Base::GetEquipBasicInfo(id);
		if (equip != nullptr)
		{
			btn->setIcon(equip->icon);
			btn->setToolTip(equip->name);
		}
	}
	else
	{
		const Info_Item *item = Item_Base::FindItem_Item(id);
		if (item != nullptr)
		{
			btn->setIcon(item->icon);
			btn->setToolTip(item->name);
		}
	}

	btn->setEnabled(true);
	lbl->setText(QString("<font color = red>%1</font>").arg(itemCount));
	return true;
}

// void city_smithy::on_btn_m_m_clicked(void)
// {
// 
// }
// void city_smithy::on_btn_m1_clicked(void)
// {
// 
// }
// void city_smithy::on_btn_m2_clicked(void)
// {
// 
// }
// void city_smithy::on_btn_m3_clicked(void)
// {
// 
// }
// void city_smithy::on_btn_m4_clicked(void)
// {
// 
// }