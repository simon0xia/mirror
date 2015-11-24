#include "city_mercenaries.h"
#include <QMessageBox>
#include "Item_Base.h"

city_Mercenaries::city_Mercenaries(QWidget *parent, CPlayer *const w_player)
	: QWidget(parent), player(w_player)
{
	ui.setupUi(this);
	m_bag_equip = player->get_bag_equip();

	ui.tableWidget->setColumnWidth(0, 97);
	ui.tableWidget->setColumnWidth(2, 83);
	ui.tableWidget->setColumnWidth(3, 78);
	ui.tableWidget->hideColumn(1);

	const qint32 count = 12;
	const itemID id[count] = { 308022, 308023, 308024, 308025, 308026, 308027, 308028, 308029, 308030, 308031, 308032, 308033 };
	const quint32 NeedRep[count] = { 100000, 400000, 1150000, 2650000, 5350000, 9670000, 16150000, 25550000, 38700000, 56850000, 81540000, 114620000 };
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
	
	ui.tableWidget->setCurrentCell(0, 0);
	updateInfo();

// 	Info_Equip equip = { 308022 };
// 	const Info_basic_equip *EquipBasicInfo = Item_Base::GetEquipBasicInfo(equip.ID);
// 	detailDlg = new Dlg_Detail(this);
// 	detailDlg->DisplayEquipInfo(QCursor::pos(), EquipBasicInfo, &equip, roleInfo);
// 	detailDlg->show();
}

city_Mercenaries::~city_Mercenaries()
{

}

void city_Mercenaries::mousePressEvent(QMouseEvent *ev)
{
	mouseMovePos = ev->globalPos();
}
void city_Mercenaries::mouseMoveEvent(QMouseEvent * ev)
{
	if (mouseMovePos != QPoint(0, 0))
	{
		move(this->pos() + ev->globalPos() - mouseMovePos);
		mouseMovePos = ev->globalPos();
	}
}
void city_Mercenaries::mouseReleaseEvent(QMouseEvent *ev)
{
	mouseMovePos = QPoint(0, 0);
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

	if (player->get_rep() < needRep)
	{
		msg = QStringLiteral("你当前的声望不足以领取此勋章。");
		title = QStringLiteral("声望未达到");
	}
	else
	{
		if (player->get_coin() < needCoin)
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
		player->sub_coin(needCoin);
		
		equip.ID = id;
		m_bag_equip->append(equip);

		updateInfo();
		QMessageBox::information(this, title, msg);
	}
	else
	{
		QMessageBox::critical(this, title, msg);
	}
}

void city_Mercenaries::updateInfo(void)
{
	ui.lbl_rep->setText(QString::number(player->get_rep()));
	ui.lbl_coin->setText(QString::number(player->get_coin()));
}