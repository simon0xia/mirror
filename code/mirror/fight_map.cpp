#include "fight_map.h"
#include <QTime>
#include <QFile>
#include <QMessageBox>

extern QWidget *g_widget;
extern QMap<mapID, Info_Distribute> g_MonsterDistribute;

fight_map::fight_map(qint32 mapID, RoleInfo *info, MapItem *bag_item, ListEquip *bag_equip)
	: QWidget(NULL), m_mapID(mapID), myRole(info), m_bag_item(bag_item), m_bag_equip(bag_equip)
{
	ui.setupUi(this);
	ui.listWidget->setMovement(QListView::Static);
	m_dlg_fightfight = nullptr;

	deleyTimer = startTimer(1000);
	connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(itemClicked(QListWidgetItem *)));
}

fight_map::~fight_map()
{

}

void fight_map::timerEvent(QTimerEvent *event)
{
	killTimer(deleyTimer);

	quint32 Role_Lvl = (myRole->level >> 1) - 1;
	qint32 nStart = (m_mapID) * 1000;
	qint32 nStop = (m_mapID + 1) * 1000;
	QListWidgetItem *item;
	foreach(const Info_Distribute &dis, g_MonsterDistribute)
	{
		if (dis.ID < nStart)
		{
			continue;
		}
		else if (dis.ID < nStop)
		{
			if ((dis.need_lv < Role_Lvl + 50))
			{
				item = new QListWidgetItem(dis.img, dis.name);
				item->setWhatsThis(QString::number(dis.ID));
				item->setToolTip(QStringLiteral("建议您至少达到 %1 级再进入").arg(dis.need_lv));
				ui.listWidget->addItem(item);
			}	
		}
		else
		{
			break;
		}
	}
}

void fight_map::itemClicked(QListWidgetItem * item)
{
	mapID id = item->whatsThis().toUInt();
	
	m_dlg_fightfight = new fight_fight(g_widget, id, myRole, m_bag_item, m_bag_equip);
	m_dlg_fightfight->setWindowFlags(Qt::SubWindow);
	m_dlg_fightfight->move(g_widget->mapFromGlobal(g_widget->pos()) + QPoint(8, 30));
	m_dlg_fightfight->exec();

	delete m_dlg_fightfight;
	m_dlg_fightfight = nullptr;
}