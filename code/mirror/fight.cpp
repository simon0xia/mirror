#include "fight.h"


fight::fight(CPlayer *const w_player)
	: QWidget(NULL)
{
	ui.setupUi(this);

	QStringList mapTypeName = { QStringLiteral("普通地图"), QStringLiteral("特殊地图"), QStringLiteral("练功师") };

	CountOfMapType = mapTypeName.size();
	for (qint32 i = 0; i < CountOfMapType; i++)
	{
		fightmap[i] = new fight_map(i, w_player);
		if (fightmap[i] != nullptr)
		{
			ui.tabWidget_fight->addTab(fightmap[i], mapTypeName.at(i));
		}	
	}
}

fight::~fight()
{
	for (qint32 i = 0; i < CountOfMapType; i++)
	{
		delete fightmap[i];
	}
}
