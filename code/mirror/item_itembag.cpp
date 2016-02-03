#include "item_itembag.h"
#include <QMessageBox>
#include "Player.h"
#include "dlg_detail.h"
#include "dlg_count.h"

extern QWidget *g_widget;
extern Dlg_Detail *g_dlg_detail;
extern QMap<skillID, Info_SkillBasic> g_SkillBasic;

extern QVector<QImage> g_dat_item;
extern QVector<QImage> g_dat_ui;

item_itemBag::item_itemBag(QWidget *parent)
	:Item_Base(parent)
{
	ui.btn_clear->setEnabled(false);
	ui.btn_sort->setEnabled(false);

	CurrentPage = 0;
	pages = 1;
	ui.lbl_page->setText(QStringLiteral("1/1"));

	m_item = &PlayerIns.get_bag_item();

	connect(ui.btn_sale, SIGNAL(clicked()), this, SLOT(on_btn_sale_clicked()));
	connect(ui.bagView, SIGNAL(entered(QModelIndex)), this, SLOT(ShowItemInfo(QModelIndex)));
	connect(ui.bagView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ShowContextMenu(QPoint)));
}

item_itemBag::~item_itemBag()
{

}

void item_itemBag::DisplayItems()
{
	qint32 row_cur = 0;
	qint32 col_cur = 0;

	//必须先清除背包显示，否则当前道具种类小于之前道具种类时会在最尾显示原道具的假像。
	model->clear();
	for (auto iter = m_item->constBegin(); iter != m_item->constEnd(); iter++)
	{
		const Info_Item *itemItem = FindItem_Item(iter.key());
		if (itemItem != nullptr)
		{
			MiItem item;
			item.id = itemItem->ID;
			item.count = *iter;
			item.intensify = 0;
			item.image = g_dat_item.at(itemItem->icon);			
			item.quality = g_dat_ui.at(1);
			
			model->setData(row_cur, col_cur, item);

			++col_cur;
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
	if (CurrentPage > 0)
	{
		--CurrentPage;
		DisplayItems();
	}
}
void item_itemBag::on_btn_pgDn_clicked()
{
	if (CurrentPage < pages-1)
	{
		++CurrentPage;
		DisplayItems();
	}
}

void item_itemBag::ShowItemInfo(const QModelIndex &index)
{
	g_dlg_detail->hide();

	qint32 row = index.row();
	qint32 column = index.column();
	ShowItemInfo_item(row, column, CurrentPage, m_item);
}

void item_itemBag::ShowContextMenu(QPoint pos)
{
	g_dlg_detail->hide();

	qint32 index = GetCurrentCellIndex(CurrentPage);
	if (index >= 0 && index < m_item->count())
	{
		on_action_use(index);
	}
}

void item_itemBag::on_action_use(qint32 index)
{
	itemID ID = GetItemID(index, m_item);

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

void item_itemBag::on_btn_sale_clicked()
{
	qint32 index = GetCurrentCellIndex(CurrentPage);
	if (index < 0 || index >= m_item->count())
	{
		return;
	}

	itemID ID = GetItemID(index, m_item);

	//道具只卖1金币。
	PlayerIns.add_coin(m_item->value(ID));
	m_item->remove(ID);
	DisplayItems();
	emit UpdateCoin();
}
