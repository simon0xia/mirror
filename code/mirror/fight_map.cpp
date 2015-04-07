#include "fight_map.h"

extern QWidget *g_widget;

fight_map::fight_map(qint32 mapID, RoleInfo *info)
: QWidget(NULL), m_mapID(mapID), myRole(info)
{
	ui.setupUi(this);

	QString strTmp;
	QString iconPath = ":/map/Resources/map/";
	if (m_mapID < 10)
	{
		iconPath += "0";
	}
	iconPath += QString::number(0);

	const mirror_map *p = map_minor[mapID];
	for (qint32 i = 0; i < mapCount_minor; i++)
	{
		if (!p->name.isEmpty())
		{
			strTmp = iconPath + QString::number(0) + ".png";
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
	QString strTmp = item->text();
	strTmp += " ";
	if (m_mapID < 10)
	{
		strTmp += "0";
	}
	strTmp += QString::number(m_mapID);

	const mirror_map *p = map_minor[m_mapID];
	for (qint32 i = 0; i < 9; i++)
	{
		if (p->name == item->text())
		{
			strTmp += QString::number(p->ID);
			break;
		}
		p++;
	}
	
	fight_fight *fightfight = new fight_fight(g_widget, m_mapID, p->ID, myRole);
	fightfight->setWindowFlags(Qt::SubWindow);
	fightfight->move(g_widget->mapFromGlobal(g_widget->pos()) + QPoint(8,30));
	fightfight->show();
}