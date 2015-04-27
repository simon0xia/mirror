#include "fight_map.h"
#include <QTime>
#include <QFile>
#include <QMessageBox>

extern QWidget *g_widget;

fight_map::fight_map(qint32 mapID, RoleInfo *info, MapItem *bag_item, ListEquip *bag_equip)
	: QWidget(NULL), m_mapID(mapID), myRole(info), m_bag_item(bag_item), m_bag_equip(bag_equip)
{
	ui.setupUi(this);

	QString strTmp;
	QString iconPath = ":/map/Resources/map/";

	const mirror_map *p = map_minor[mapID];
	for (qint32 i = 0; i < mapCount_minor; i++)
	{
		if (!p->name.isEmpty())
		{
			strTmp = iconPath + QString::number(p->imageID) + ".png";
			if (!QFile::exists(strTmp))
			{
				strTmp = ":/map/Resources/map/000.png";
			}
			QListWidgetItem *item = new QListWidgetItem(QIcon(strTmp), p->name);
			ui.listWidget->addItem(item);
			++p;
		}
	}

	connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(itemClicked(QListWidgetItem *)));
}

fight_map::~fight_map()
{

}

void fight_map::itemClicked(QListWidgetItem * item)
{
	if (m_mapID > 5 && m_mapID != 7)
	{
		QString title = QStringLiteral("提示");
		QString message = QStringLiteral("当前地图未开放。");
		QMessageBox msgBox(QMessageBox::Question, title, message);
		msgBox.addButton(QStringLiteral(" 我知道了 "), QMessageBox::AcceptRole);
		msgBox.exec();
		return;
	}
	const mirror_map *p = map_minor[m_mapID];
	for (qint32 i = 0; i < 9; i++)
	{
		if (p->name == item->text())
		{
			break;
		}
		p++;
	}
	
	fight_fight *fightfight = new fight_fight(g_widget, m_mapID * 10 + p->ID, myRole, m_bag_item, m_bag_equip);
	fightfight->setWindowFlags(Qt::SubWindow);
	fightfight->move(g_widget->mapFromGlobal(g_widget->pos()) + QPoint(8,30));
	fightfight->show();
}