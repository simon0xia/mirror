#ifndef _DEF_SYSTEM_PARA_H
#define _DEF_SYSTEM_PARA_H

//定义系统控制变量
const qint32 MaxEquipCountForRole = 14;				//角色身上可装备数量
const qint32 MaxLevel = 35;							//等级上限
const quint32 g_bag_maxSize = 288;
const quint32 g_storage_maxSize = 72;

const quint32 g_itemID_start_item = 200000;
const quint32 g_itemID_stop_item = 299999;
const quint32 g_itemID_start_equip = 300000;
const quint32 g_itemID_stop_equip = 399999;
const quint32 g_equipType_weapon = 1;				//武器
const quint32 g_equipType_clothes_m = 2;			//男衣服
const quint32 g_equipType_clothes_f = 3;			//女衣服
const quint32 g_equipType_helmet = 4;				//头盔
const quint32 g_equipType_necklace = 5;				//项链
const quint32 g_equipType_bracelet = 6;				//手镯
const quint32 g_equipType_ring = 7;					//戒指
const quint32 g_equipType_medal = 8;				//勋章
const quint32 g_equipType_belt = 9;					//腰带
const quint32 g_equipType_boots = 10;				//靴子
const quint32 g_equipType_gem = 11;					//宝石
const quint32 g_equipType_shield = 12;				//盾牌
const quint32 g_equipType_runes = 13;				//符文

const double g_fight_boss_probability = 0.975;		//战斗中出现BOSS的机率, 1/40
const quint32 g_boss_after_monster = 10;			//必须先击退X个小怪后才有可能遇到BOSS

const quint32 g_specialEquip_MaxExtra = 4;			//极品装备的附加属性上限
const quint32 g_equip_MaxLvUp = 12;					//装备最大强化等级




#endif //#ifndef _DEF_SYSTEM_PARA_H