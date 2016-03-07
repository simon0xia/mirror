#include "fight_map.h"
#include <QMessageBox>
#include "MonsterDefine.h"
#include "gamemanager.h"

extern QMap<mapID, Info_Distribute> g_MonsterDistribute;
extern QVector<QPixmap> g_dat_map;

fight_map::fight_map(qint32 mapType)
	: QWidget(NULL), mapType(mapType)
{
	ui.setupUi(this);

	DisplayMap();
	ui.listWidget->setMovement(QListView::Static);
	connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(itemClicked(QListWidgetItem *)));
}

fight_map::~fight_map()
{

}

void fight_map::DisplayMap()
{
	qint32 nStart = (mapType) * 1000;
	qint32 nStop = (mapType + 1) * 1000;
	QListWidgetItem *item;

	foreach(const Info_Distribute &dis, g_MonsterDistribute)
	{
		if (dis.ID < nStart)
		{
			continue;
		}
		else if (dis.ID < nStop && dis.need_lv <= 90)
		{
			item = new QListWidgetItem(g_dat_map.at(dis.photo), dis.name);
			item->setWhatsThis(QString::number(dis.ID));
			
			ui.listWidget->addItem(item);
		}
		else
		{
			break;
		}
	}
}

void fight_map::itemClicked(QListWidgetItem * item)
{
	qint32 id= item->whatsThis().toInt();
	if (id > 1000 || GameMgrIns.get_maxMapID() >= id - 1)
	{
		emit SelectMap(id);
	}
	else
	{
		QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("请击败前一地图的BOSS"));
	}	
}