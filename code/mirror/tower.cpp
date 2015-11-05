#include "tower.h"
#include <QFile>
#include <QMessageBox>
#include "cryptography.h"
#include "mirrorlog.h"

extern QWidget *g_widget;
extern QMap<monsterID, MonsterInfo> g_MonsterInfo;		//普通怪物列表

Tower::Tower(QWidget *parent, CPlayer *const w_player)
	: QWidget(parent), player(w_player)
{
	ui.setupUi(this);

	ui.listWidget->setMovement(QListView::Static);
	ui.listWidget->setViewMode(QListView::IconMode);
	ui.listWidget->setIconSize(QSize(100, 100));
	ui.listWidget->setAutoScroll(false);

	init();
	MaxLevel = 0;

	connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(itemClicked(QListWidgetItem *)));
}

Tower::~Tower()
{
	player->ClearBuff();
}

bool Tower::init()
{
	if (!loadDistrubute())
	{
		return false;
	}

	for (int i = 0; i < dis.size(); i++)
	{
		QListWidgetItem * item = new QListWidgetItem(QStringLiteral("第 %1 层").arg(i+1));
		item->setWhatsThis(QString::number(i+1));

		const MonsterInfo &mon = g_MonsterInfo.value(dis[i].monster1);
		item->setIcon(QPixmap::fromImage(mon.Head));		
		ui.listWidget->addItem(item);
	}

	return true;
}
bool Tower::loadDistrubute()
{
	char MD5[] = "78c60cba7af1fa18287b20067ef2a062";
	QFile file("./db/TowerDistribute.db");
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	QByteArray documentContent = file.readAll();
	file.close();

	if (!cryptography::verifyDB_MD5(MD5, documentContent))
	{
		LogIns.append(LEVEL_ERROR, __FUNCTION__, mirErr_MD5);
		return false;
	}

	QDataStream out(documentContent);

	TowerDistributeInfo info;

	while (!out.atEnd())
	{
		out >> info.id >> info.cost >> info.monster1 >> info.monster2 >> info.monster3;
		dis.append(info);
	}

	return true;
}

void Tower::itemClicked(QListWidgetItem * item)
{
	int towerLv = item->whatsThis().toUInt() - 1;
	int cost = dis[towerLv].cost;
	itemID id = 299028;

#ifdef _DEBUG
	MaxLevel = 100;
#endif

	if (towerLv > MaxLevel)
	{
		QString message = QStringLiteral("你好，请你先挑战第%1层。").arg(MaxLevel + 1);
		QMessageBox::critical(this, QStringLiteral("挑战"), message);
		return;
	}

	MapItem *bag_item = player->get_bag_item();
	if (bag_item->value(id) >= cost)
	{
		if (bag_item->value(id) > cost)
		{
			bag_item->insert(id, bag_item->value(id) - cost);
		}
		else
		{
			bag_item->remove(299028);
		}
		
		m_dlg_fightfight = new tower_fight(g_widget, towerLv, dis[towerLv], player);
		m_dlg_fightfight->setWindowFlags(Qt::SubWindow);
		m_dlg_fightfight->move(g_widget->mapFromGlobal(g_widget->pos()) + QPoint(8, 30));
		m_dlg_fightfight->exec();

		if (m_dlg_fightfight->getFightRes())
		{
			MaxLevel = qMax(towerLv+1, MaxLevel);
		}

		delete m_dlg_fightfight;
		m_dlg_fightfight = nullptr;
	}
	else
	{
		QString message = QStringLiteral("你的勇士卡不足，进入需要%1张勇士卡。").arg(cost);
		QMessageBox::critical(this, QStringLiteral("门票"), message);
	}
}