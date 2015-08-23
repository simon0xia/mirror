#include "def_skill.h"


skill_fight::skill_fight(const Info_skill &skillInfo, quint32 studyLv)
{
	id = skillInfo.ID;
	name = skillInfo.name;
	icon = skillInfo.icon;
	type = skillInfo.type;
	level = studyLv;
	spell = skillInfo.spell_basic + skillInfo.spell_add * level;
	cd = skillInfo.cd;
	cd_c = 0;
	times = skillInfo.times;
	basic = skillInfo.basic;
	damage = skillInfo.damage_basic + skillInfo.damage_add * level;
	buff = skillInfo.buff;
	stiff = skillInfo.stiff;
}