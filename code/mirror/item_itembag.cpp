#include "item_itembag.h"
#include <QMessageBox>
#include "Player.h"
#include "dlg_detail.h"
#include "dlg_count.h"

extern QWidget *g_widget;
extern Dlg_Detail *g_dlg_detail;
extern QMap<skillID, Info_SkillBasic> g_SkillBasic;

item_itemBag::item_itemBag(QWidget *parent)
	:Item_Base(parent)
{
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	CurrentPage = 1;

	m_item = &PlayerIns.get_bag_item();

	popMenu = new QMenu();
	action_use = new QAction(QStringLiteral("使用"), this);
	action_sale = new QAction(QStringLiteral("销售"), this);
	popMenu->addAction(action_use);
	popMenu->addAction(action_sale);

	connect(ui.tableWidget, SIGNAL(cellEntered(int, int)), this, SLOT(ShowItemInfo(int, int)));
	connect(ui.tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowContextMenu(QPoint)));

	connect(action_use, SIGNAL(triggered(bool)), this, SLOT(on_action_use()));
	connect(action_sale, SIGNAL(triggered(bool)), this, SLOT(on_action_sale()));
}

item_itemBag::~item_itemBag()
{

}

void item_itemBag::DisplayItems()
{
	quint32 row_Count = ui.tableWidget->rowCount();
	quint32 Col_Count = ui.tableWidget->columnCount();
	quint32 row_cur = 0;
	quint32 col_cur = 0;

	QString strTmp = "";

	pages = (m_item->size() + row_Count * Col_Count - 1) / (row_Count * Col_Count);
	if (pages == 0)
		pages = 1;
	if (CurrentPage > pages)
		CurrentPage = pages;

	ui.edit_page_cur->setText(QString::number(CurrentPage));
	ui.edit_page_all->setText(QString::number(pages));

	//必须先清除背包显示，否则当前道具种类小于之前道具种类时会在最尾显示原道具的假像。
	ui.tableWidget->clear();
	auto iter = m_item->constBegin();
	for (quint32 i = 0; i < (CurrentPage - 1) * (row_Count * Col_Count); i++, iter++) { ; }

	for (; iter != m_item->constEnd(); iter++)
	{
		const Info_Item *itemItem = FindItem_Item(iter.key());
		if (itemItem != nullptr)
		{
			if (*iter > 99) {
				strTmp = QStringLiteral("99+");
			} else {
				strTmp = QStringLiteral("x%1").arg(*iter);
			}
			
			QTableWidgetItem *twItem = new QTableWidgetItem(strTmp);
			twItem->setTextAlignment(Qt::AlignRight | Qt::AlignBottom);
			twItem->setBackground(QBrush(itemItem->icon));
			twItem->setTextColor(QColor("cyan"));
			twItem->setWhatsThis(QString::number(itemItem->ID));

			QFont font = twItem->font();
			font.setPointSize(7);
			twItem->setFont(font);

			ui.tableWidget->setItem(row_cur, col_cur++, twItem);
			if (col_cur >= Col_Count)
			{
				++row_cur;
				col_cur = 0;
			}
		}
	}
}

void item_itemBag::on_btn_pgUp_clicked()
{
	if (CurrentPage > 1)
	{
		--CurrentPage;
		DisplayItems();
	}
}
void item_itemBag::on_btn_pgDn_clicked()
{
	if (CurrentPage < pages)
	{
		++CurrentPage;
		DisplayItems();
	}
}

void item_itemBag::ShowItemInfo(int row, int column)
{
	ShowItemInfo_item(row, column, CurrentPage, m_item);
}

void item_itemBag::ShowContextMenu(QPoint pos)
{
	g_dlg_detail->hide();

	//如果右击空白单元格，不弹出右键菜单。
	if (m_item->size() > GetCurrentCellIndex(CurrentPage))
	{
		popMenu->exec(ui.tableWidget->mapToGlobal(pos));
	}
}

void item_itemBag::on_action_use()
{
	itemID ID = ui.tableWidget->currentItem()->whatsThis().toInt();
	const CHuman &edt = PlayerIns.get_edt_current();

	const Info_Item* item = FindItem_Item(ID);
	if (edt.get_lv() < item->level)
	{
		QString message = QStringLiteral("等级不足！");
		QMessageBox::critical(g_widget, QStringLiteral("提示"), message);
		return;
	}
	if (item->vocation != 0 && item->vocation != edt.get_voc())
	{
		QString message = QStringLiteral("职业不符合！");
		QMessageBox::critical(g_widget, QStringLiteral("提示"), message);
		return;
	}
	if (item->type == et_NoEffect)
	{
		QString message = QStringLiteral("任务道具，不可右键使用！");
		QMessageBox::critical(g_widget, QStringLiteral("提示"), message);
		return;
	}

	//弹出对话框询问使用数量。
	int32_t nTmp = getUsedCount(ID);
	if (nTmp > 0)
	{
		UsedItem(item, nTmp);
	}
}

void item_itemBag::UsedItem(const Info_Item* itemItem, int32_t usedCount)
{
	qint32 nTmp;
	QString strTmp;

	nTmp = itemItem->value * usedCount;
	switch (itemItem->type)
	{
	case et_immediate_coin:
		PlayerIns.add_coin(nTmp);
		strTmp = QStringLiteral("金币增加：") + QString::number(nTmp);
		emit UpdateCoin();
		break;
	case et_immediate_reputation:
		PlayerIns.add_rep(nTmp);
		strTmp = QStringLiteral("声望增加：") + QString::number(nTmp);
		emit UpdateRep();
		break;
	case et_Skill:
		nTmp = UsedItem_skill(itemItem->ID, usedCount);
		strTmp = PlayerIns.get_edt_current().get_name();
		if (nTmp == 1) {
			strTmp += QStringLiteral("提升技能《%1》的等级。").arg(itemItem->name);
		} else if (nTmp == 2) {
			strTmp += QStringLiteral("学会技能《%1》").arg(itemItem->name);
		} else if (nTmp == 3) {
			strTmp += QStringLiteral("的《%1》技能等级已达上限。").arg(itemItem->name);
		} else {
			strTmp = QStringLiteral("学习技能错误。当前技能ID：%1").arg(itemItem->ID);
		}
		break;
	default:
		break;
	}

	updateItem(itemItem->ID, 0 - usedCount);

	if (!strTmp.isEmpty())
	{
		QMessageBox *msgBox = new QMessageBox;
		msgBox->setText(strTmp);
		msgBox->setWindowTitle(QStringLiteral("提示"));
		msgBox->addButton(QStringLiteral(" 我知道了 "), QMessageBox::AcceptRole);
		msgBox->exec();
	}
}

int32_t item_itemBag::UsedItem_skill(itemID book, int32_t &usedCount)
{
	CHuman &edt = PlayerIns.get_edt_current();
	MapSkillStudy &skill_study = edt.get_skill_study();

	SkillStudy skill;
	bool WasStudy = false;
	int nTmp = 0;
	if (skill_study.contains(book))	{
		skill = skill_study.value(book);
		WasStudy = true;
	}
	else
	{
		skill.id = book;
		skill.level = 0;
		skill.usdIndex = 0;
	}

	usedCount = qMin(usedCount, g_SkillBasic.value(book).level - skill.level);
	if (usedCount > 0)
	{
		skill.level += usedCount;
		skill_study.insert(skill.id, skill);

		if (WasStudy) {
			nTmp = 1;	
		} else	{
			nTmp = 2;		
		}
	} else {
		nTmp = 3;		
	}
	return nTmp;
}

void item_itemBag::updateItem(itemID id, int32_t count)
{
	int32_t HowMany = m_item->value(id);
	HowMany += count;
	if (HowMany <= 0)
		m_item->remove(id);
	else
		m_item->insert(id, HowMany);

	DisplayItems();
}

int32_t item_itemBag::getUsedCount(itemID id)
{
	int32_t usedCount = 0;
	dlg_count *dlg = new dlg_count(g_widget, QStringLiteral("使用量"), m_item->value(id));
	if (QDialog::Accepted == dlg->exec())
	{
		usedCount = dlg->getCount();
	}
	delete dlg;

	return usedCount;
}

void item_itemBag::on_action_sale()
{
	itemID ID = ui.tableWidget->currentItem()->whatsThis().toInt();

	//道具只卖1金币。
	PlayerIns.add_coin(m_item->value(ID));
	m_item->remove(ID);
	DisplayItems();
	emit UpdateCoin();
}
