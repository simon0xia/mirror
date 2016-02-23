#ifndef _DEF_TYPE_H
#define _DEF_TYPE_H

#include <stdint.h>
#include <QMap>

typedef int32_t mapID;
typedef int32_t skillID;
typedef int32_t itemID;
typedef int32_t monsterID;
typedef int32_t playerID;

enum Vocation
{
	Voc_Unknow,
	Voc_Warrior,
	Voc_Magic,
	Voc_Taoist
};

enum LeftWindow
{
	LW_role,
	LW_smithy
};

struct FightInfoStatistics
{
	mapID whatsMap;						//上一次挂机所在地图
	int32_t minutes;					//···挂机总时长
	int32_t nCount_victory;				//···胜利次数
	int32_t nCount_StraightVictory;		//···最大连胜次数
	int32_t nCount_fail;				//···失败次数
	int32_t nCount_draw;				//···超时次数
	QMap<monsterID, int32_t> killMonster;	//需要记录的杀击怪物的数量
};

#endif	//#ifndef _DEF_TYPE_H