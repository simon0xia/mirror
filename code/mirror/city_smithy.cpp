#include "city_smithy.h"
#include <QTreeWidgetItem>
#include <QMouseEvent>
#include <qt_windows.h>
#include "QMessageBox"
#include "ItemDefine.h"
#include "Item_Base.h"
#include "mirrorlog.h"

extern QMap<itemID, info_formula> g_formula;

city_smithy::city_smithy(QWidget *parrent, RoleInfo *roleInfo, MapItem *bag_item, ListEquip *bag_equip)
	: QWidget(parrent), myRole(roleInfo), m_bag_item(bag_item), m_bag_equip(bag_equip)
{
	ui.setupUi(this);
	m_parrent = parrent;

	QString str[3] = { QStringLiteral("宝石"), QStringLiteral("护盾"), QStringLiteral("魂珠") };
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

//	ui.btn_m_m->installEventFilter(this);

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
	bool bsafity = true;
	bool bTmp;
	itemID id = item->text(1).toUInt();
	if (!g_formula.contains(id))
	{
		//has error;
		return;
	}
	const info_formula &f = g_formula[id];

	ui.lbl_name->setText(item->text(0));
	ui.lbl_name->setWhatsThis(QString::number(id));
	
	bsafity = DisplayMaterialInfo(f.m_m, ui.btn_m_m);

	bTmp = DisplayMaterialInfo(f.m1_t, f.m1_c, ui.btn_m1, ui.lbl_m1);
	bsafity = bsafity && bTmp;

	bTmp = DisplayMaterialInfo(f.m2_t, f.m2_c, ui.btn_m2, ui.lbl_m2);
	bsafity = bsafity && bTmp;

	bTmp = DisplayMaterialInfo(f.m3_t, f.m3_c, ui.btn_m3, ui.lbl_m3);
	bsafity = bsafity && bTmp;

	bTmp = DisplayMaterialInfo(f.m4_t, f.m4_c, ui.btn_m4, ui.lbl_m4);
	bsafity = bsafity && bTmp;

	ui.btn_make->setEnabled(bsafity);
}

bool city_smithy::DisplayMaterialInfo(itemID id, QPushButton *btn)
{
	if (id <= g_itemID_start_equip || id > g_itemID_stop_equip)
	{
		btn->setIcon(QIcon(""));
		ui.lbl_mm->setText("");
		return true;
	}

	qint32 nTmp;
	bool bsafity = false;

	const Info_basic_equip *equip = Item_Base::GetEquipBasicInfo(id);
	if (equip != nullptr)
	{
		btn->setIcon(equip->icon);
		btn->setToolTip(equip->name);
	}

	for (ListEquip::const_iterator iter = m_bag_equip->begin(); iter != m_bag_equip->end(); iter++)
	{
		if (iter->ID == id)
		{
			bsafity = true;
			break;
		}
	}

	if (bsafity)
	{
		ui.lbl_mm->setText(QStringLiteral("<font color = green>满足</font>"));
	}
	else
	{
		ui.lbl_mm->setText(QStringLiteral("<font color = red>不满足</font>"));
	}

	return bsafity;
}
bool city_smithy::DisplayMaterialInfo(itemID id, qint32 itemCount, QPushButton *btn, QLabel *lbl)
{
	if (id == 0 || itemCount <= 0)
	{
		btn->setIcon(QIcon(""));
		lbl->setText(QString(""));
		return true;
	}

	qint32 nTmp;
	bool bsafity = false;

	const Info_Item *item = Item_Base::FindItem_Item(id);
	if (item != nullptr)
	{
		btn->setIcon(item->icon);
		btn->setToolTip(item->name);

		if (itemCount <= m_bag_item->value(id))
		{
			bsafity = true;
			lbl->setText(QString("<font color = green>%1</font>").arg(itemCount));
		}
		else
		{
			lbl->setText(QString("<font color = red>%1</font>").arg(itemCount));
		}
	}
	return bsafity;
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

void city_smithy::on_btn_make_clicked(void)
{
	itemID id = ui.lbl_name->whatsThis().toUInt();
	Info_Equip equip = { 0 };
	equip.ID = id;

	//扣除相应主材料。
	const info_formula &f = g_formula[id];
	if (f.m_m != 0)
	{
		for (qint32 i = 0; i < m_bag_equip->size(); i++)
		{
			if (m_bag_equip->at(i).ID == f.m_m)
			{
				m_bag_equip->removeAt(i);
				break;
			}
		}	
	}
	//扣除相应辅助材料。
	m_bag_item->insert(f.m1_t, m_bag_item->value(f.m1_t) - f.m1_c);
	if (m_bag_item->value(f.m1_t) <= 0)
	{
		m_bag_item->remove(f.m1_t);
	}
	m_bag_item->insert(f.m2_t, m_bag_item->value(f.m2_t) - f.m2_c);
	if (m_bag_item->value(f.m2_t) <= 0)
	{
		m_bag_item->remove(f.m2_t);
	}
	m_bag_item->insert(f.m3_t, m_bag_item->value(f.m3_t) - f.m3_c);
	if (m_bag_item->value(f.m3_t) <= 0)
	{
		m_bag_item->remove(f.m3_t);
	}
	m_bag_item->insert(f.m4_t, m_bag_item->value(f.m4_t) - f.m4_c);
	if (m_bag_item->value(f.m4_t) <= 0)
	{
		m_bag_item->remove(f.m4_t);
	}

	//给予装备。
	m_bag_equip->append(equip);

	QString msg = QStringLiteral("恭喜你成功铸造：%1 ").arg(ui.lbl_name->text());
	QMessageBox::information(this, QStringLiteral("恭喜"), msg);

	itemClicked(ui.treeWidget->currentItem(), 0);
}

bool city_smithy::eventFilter(QObject *obj, QEvent *ev)
{
	QMouseEvent *mouseEvent = (QMouseEvent *)(ev);
	QPoint pos = QCursor::pos();

	if (ev->type() == QEvent::Enter)
	{

	}
	else if (ev->type() == QEvent::Leave)
	{

	}

	// pass the event on to the parent class
	return QWidget::eventFilter(obj, ev);
}