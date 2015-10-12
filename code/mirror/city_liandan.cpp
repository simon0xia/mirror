#include "city_liandan.h"
#include <QMouseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "Item_Base.h"

city_liandan::city_liandan(QWidget *parent, CPlayer *const w_player)
	: QWidget(parent), player(w_player)
{
	ui.setupUi(this);
	m_parrent = parent;
	m_bag_item = player->get_bag_item();

	elixir = {
		204002, 
		299027, 1, 
		299024, 2, 
		299026, 2, 
		204001, 2
	};
	updateInfo();
}

city_liandan::~city_liandan()
{

}

void city_liandan::on_btn_close_clicked(void)
{
	close();
}

void city_liandan::mousePressEvent(QMouseEvent *ev)
{
	mouseMovePos = ev->globalPos();
}
void city_liandan::mouseMoveEvent(QMouseEvent * ev)
{
	if (mouseMovePos != QPoint(0, 0))
	{
		move(this->pos() + ev->globalPos() - mouseMovePos);
		mouseMovePos = ev->globalPos();
	}
}
void city_liandan::mouseReleaseEvent(QMouseEvent *ev)
{
	mouseMovePos = QPoint(0, 0);
}

void city_liandan::updateInfo(void)
{
	bool bsafity = true;
	bool bTmp;

	const Info_Item *item = Item_Base::FindItem_Item(elixir.ID);
	if (item != nullptr)
	{
		ui.btn_mm->setIcon(item->icon);
		ui.btn_mm->setToolTip(item->name);
		ui.btn_mm->setWhatsThis(QString::number(elixir.ID));
	}

	bTmp = DisplayMaterialInfo(elixir.m1_t, elixir.m1_c, ui.btn_m1, ui.lbl_m1);
	bsafity = bsafity && bTmp;

	bTmp = DisplayMaterialInfo(elixir.m2_t, elixir.m2_c, ui.btn_m2, ui.lbl_m2);
	bsafity = bsafity && bTmp;

	bTmp = DisplayMaterialInfo(elixir.m3_t, elixir.m3_c, ui.btn_m3, ui.lbl_m3);
	bsafity = bsafity && bTmp;

	bTmp = DisplayMaterialInfo(elixir.m4_t, elixir.m4_c, ui.btn_m4, ui.lbl_m4);
	bsafity = bsafity && bTmp;

	bTmp = DisplayMaterialInfo(elixir.m5_t, elixir.m5_c, ui.btn_m5, ui.lbl_m5);
	bsafity = bsafity && bTmp;

	ui.btn_make->setEnabled(bsafity);
}
void city_liandan::on_btn_make_clicked(void)
{
	itemID id = ui.btn_mm->whatsThis().toUInt();
	quint32 nDay, nMinute;
	bool bSuccess;
	QString strMsg, strTitle;

	//扣除相应材料。
	m_bag_item->insert(elixir.m1_t, m_bag_item->value(elixir.m1_t) - elixir.m1_c);
	if (m_bag_item->value(elixir.m1_t) <= 0)
	{
		m_bag_item->remove(elixir.m1_t);
	}
	m_bag_item->insert(elixir.m2_t, m_bag_item->value(elixir.m2_t) - elixir.m2_c);
	if (m_bag_item->value(elixir.m2_t) <= 0)
	{
		m_bag_item->remove(elixir.m2_t);
	}
	m_bag_item->insert(elixir.m3_t, m_bag_item->value(elixir.m3_t) - elixir.m3_c);
	if (m_bag_item->value(elixir.m3_t) <= 0)
	{
		m_bag_item->remove(elixir.m3_t);
	}
	m_bag_item->insert(elixir.m4_t, m_bag_item->value(elixir.m4_t) - elixir.m4_c);
	if (m_bag_item->value(elixir.m4_t) <= 0)
	{
		m_bag_item->remove(elixir.m4_t);
	}
	m_bag_item->insert(elixir.m5_t, m_bag_item->value(elixir.m5_t) - elixir.m5_c);
	if (m_bag_item->value(elixir.m5_t) <= 0)
	{
		m_bag_item->remove(elixir.m5_t);
	}

	//如果日期数字==分钟数字，则给予丹药。
	QDateTime time = QDateTime::currentDateTime();
	if (time.date().day() == time.time().minute())
	{
		m_bag_item->insert(id, m_bag_item->value(id) + 1);
		bSuccess = true;

		strMsg = QStringLiteral("恭喜你成功熔炼出《%1》").arg(ui.btn_mm->toolTip());
	}
	else
	{
		bSuccess = false;

		strMsg = QStringLiteral("非常遗憾，你熔炼《%1》失败了。").arg(ui.btn_mm->toolTip());
	}
	QMessageBox::information(this, QStringLiteral("熔炼"), strMsg);

	updateInfo();
}

bool city_liandan::DisplayMaterialInfo(itemID id, qint32 itemCount, QPushButton *btn, QLabel *lbl)
{
	if (id == 0 || itemCount <= 0)
	{
		btn->setEnabled(false);
		btn->setIcon(QIcon(""));
		lbl->setText(QString(""));
		return true;
	}
	btn->setEnabled(true);

	qint32 nTmp = m_bag_item->value(id);
	bool bsafity = false;

	const Info_Item *item = Item_Base::FindItem_Item(id);
	if (item != nullptr)
	{
		btn->setIcon(item->icon);
		btn->setToolTip(item->name);

		if (itemCount <= nTmp)
		{
			bsafity = true;
			lbl->setText(QString("<font color = green>%1 / %2</font>").arg(nTmp).arg(itemCount));
		}
		else
		{
			lbl->setText(QString("<font color = red>%1 / %2</font>").arg(nTmp).arg(itemCount));
		}
	}
	return bsafity;
}