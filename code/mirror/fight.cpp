#include "fight.h"


fight::fight(RoleInfo *roleInfo, MapItem *bag_item, ListEquip *bag_equip)
	: myTabFrame(NULL)
	, myRole(roleInfo)
	, m_bag_item(bag_item)
	, m_bag_equip(bag_equip)
{
	ui.setupUi(this);

	QStringList mapTypeName = { QStringLiteral("普通地图"), QStringLiteral("特殊地图") };

	for (qint32 i = 0; i < mapTypeName.size(); i++)
	{
		fightmap[i] = new fight_map(i, myRole, m_bag_item, m_bag_equip);
		if (fightmap[i] != NULL)
		{
			ui.tabWidget_fight->addTab(fightmap[i], mapTypeName.at(i));
		}	
	}
	ui.tabWidget_fight->setCurrentIndex(1);
}

fight::~fight()
{
	for (qint32 i = 0; i < mapCount_major; i++)
	{
		delete fightmap[i];
	}
}

void fight::updateRoleInfo(void)
{

}
