#include <QMessageBox>
#include "item_equipstorage.h"
#include "Player.h"
#include "dlg_detail.h"

extern QWidget *g_widget;
extern Dlg_Detail *g_dlg_detail;

extern QVector<QImage> g_dat_item;
extern QVector<QImage> g_dat_icon;

Item_equipStorage::Item_equipStorage(QWidget *parent)
	:Item_Base(parent)
{
	ui.btn_sale->setEnabled(false);
	ui.btn_sort->setEnabled(false);
	ui.btn_clear->setEnabled(false);
	ui.btn_storage->setText(QStringLiteral("取出"));

	CurrentPage = 0;
	ui.lbl_page->setText(QStringLiteral("1/1"));

	m_item = &PlayerIns.get_bag_equip();
	m_storageItem = &PlayerIns.get_storage_equip();

	connect(ui.btn_storage, SIGNAL(clicked()), this, SLOT(TakeOutEquip()));
	connect(ui.bagView, SIGNAL(entered(QModelIndex)), this, SLOT(ShowItemInfo(QModelIndex)));
	connect(ui.bagView, SIGNAL(customContextMenuRequested(QPoint)), ui.btn_storage, SIGNAL(clicked()));
}

Item_equipStorage::~Item_equipStorage()
{

}


void Item_equipStorage::updateInfo()
{
	qint32 col_cur, row_cur;
	col_cur = row_cur = 0;
	//必须先清除背包显示，否则当前装备数量小于之前数量时会在最尾显示原装备的假像。
	model->clear();
	for (auto iter = m_storageItem->constBegin(); iter != m_storageItem->constEnd(); iter++)
	{
		const Info_basic_equip *EquipBasicInfo = GetEquipBasicInfo(iter->ID);
		if (EquipBasicInfo != nullptr)
		{
			MiItem item;
			item.id = EquipBasicInfo->ID;
			item.count = 1;
			item.intensify = iter->lvUp;
			item.image = g_dat_item.at(EquipBasicInfo->icon);
			item.quality = g_dat_icon.at(iter->extraAmount + 200);

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

void Item_equipStorage::ShowItemInfo(const QModelIndex &index)
{
	g_dlg_detail->hide();

	qint32 row = index.row();
	qint32 column = index.column();
	ShowItemInfo_equip(row, column, CurrentPage, m_storageItem);
}

void Item_equipStorage::TakeOutEquip(void)
{
	g_dlg_detail->hide();

	qint32 index = GetCurrentCellIndex(CurrentPage);
	if (m_storageItem->size() != 0 && m_storageItem->size() >= index)
	{
		if (m_item->size() >= g_bag_maxSize)
		{
			QString message = QStringLiteral("背包已满！");
			QMessageBox::critical(g_widget, QStringLiteral("提示"), message);
		}
		else
		{
			m_item->append(m_storageItem->at(index));
			m_storageItem->removeAt(index);

			updateInfo();
			emit UpdateBag_BagEquip();
		}
	}
}