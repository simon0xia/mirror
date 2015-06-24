#include "city_mercenaries.h"
#include <QMouseEvent>
#include <QMessageBox>
#include "Item_Base.h"

extern RoleInfo_False g_falseRole;

city_Mercenaries::city_Mercenaries(QWidget *parent, RoleInfo *roleInfo, ListEquip *bag_equip)
	: QWidget(parent), myRole(roleInfo), m_bag_equip(bag_equip)
{
	ui.setupUi(this);
	m_parrent = parent;

	ui.tableWidget->setColumnWidth(0, 97);
	ui.tableWidget->setColumnWidth(2, 83);
	ui.tableWidget->setColumnWidth(3, 78);
	ui.tableWidget->hideColumn(1);

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
			ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(id[i])));
			ui.tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(NeedRep[i])));
			ui.tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(coin[i])));
		}
	}

	ui.lbl_rep->setText(QString::number((myRole->reputation >> 1) -1));
	ui.lbl_coin->setText(QString::number((myRole->coin >> 1) -1));
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

void city_Mercenaries::on_btn_buy_clicked(void)
{
	qint32 row = ui.tableWidget->currentRow();
	QString itemName = ui.tableWidget->item(row, 0)->text();
	itemID id = ui.tableWidget->item(row, 1)->text().toUInt();
	qint32 needRep = ui.tableWidget->item(row, 2)->text().toUInt();
	qint32 needCoin = ui.tableWidget->item(row, 3)->text().toUInt();
	QString msg, title;
	bool bSatisfy = false;
	Info_Equip equip = { 0 };

	quint64 role_coin = (myRole->coin >> 1) - 1;
	quint64 role_rep = (myRole->reputation >> 1) - 1;

	if (role_rep < needRep)
	{
		msg = QStringLiteral("你当前的声望不足以领取此勋章。");
		title = QStringLiteral("声望未达到");
	}
	else
	{
		if (role_coin < needCoin)
		{
			msg = QStringLiteral("没钱不要乱点！");
			title = QStringLiteral("金币不足");
		}
		else
		{
			bSatisfy = true;
			msg = QStringLiteral("恭喜你成功领取勋章：%1 \n我们仅收取勋章的制作成本:%2金币").arg(itemName).arg(needCoin);
			title = QStringLiteral("恭喜");
		}
	}

	if (bSatisfy)
	{
		myRole->coin -= needCoin << 1;
		ui.lbl_coin->setText(QString::number((myRole->coin >> 1) -1 ));

		g_falseRole.coin -= needCoin;
		
		equip.ID = id;
		m_bag_equip->append(equip);

		QMessageBox::information(this, title, msg);
	}
	else
	{
		QMessageBox::critical(this, title, msg);
	}

	
}