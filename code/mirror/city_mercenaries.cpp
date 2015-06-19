#include "city_mercenaries.h"
#include <QMouseEvent>
#include "Item_Base.h"

city_Mercenaries::city_Mercenaries(QWidget *parent, RoleInfo *roleInfo)
	: QWidget(parent), myRole(roleInfo)
{
	ui.setupUi(this);
	m_parrent = parent;

	ui.tableWidget->setColumnWidth(0, 97);
	ui.tableWidget->setColumnWidth(1, 83);
	ui.tableWidget->setColumnWidth(2, 78);

	const qint32 count = 12;
	const itemID id[count] = { 308022, 308023, 308024, 308025, 308026, 308027, 308028, 308029, 308030, 308031, 308032, 308033 };
	const quint32 NeedRep[count] = { 100000, 400000, 1150000, 2650000, 5350000, 9670000, 1615000, 25550000, 38700000, 56850000, 81540000, 114620000 };
	const quint32 coin[count] = { 10000, 20000, 40000, 70000, 110000, 160000, 220000, 290000, 370000, 460000, 560000, 670000 };

	for (qint32 i = 0; i < count + 1; i++)
	{
		const Info_basic_equip *equip = Item_Base::GetEquipBasicInfo(id[i]);
		if (equip != nullptr)
		{
			ui.tableWidget->setItem(i, 0, new QTableWidgetItem(equip->name));
			ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(NeedRep[i])));
			ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(coin[i])));
		}
	}
}

city_Mercenaries::~city_Mercenaries()
{

}

void city_Mercenaries::mouseMoveEvent(QMouseEvent * ev)
{
	QPoint point = m_parrent->mapFromGlobal(ev->globalPos()) - QPoint(230, 30);
	move(point);
}

void city_Mercenaries::on_btn_close_clicked(void)
{
	close();
}