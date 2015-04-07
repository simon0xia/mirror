#include "city.h"

city::city(RoleInfo *roleInfo)
: myTabFrame(NULL), myRole(roleInfo)
{
	ui.setupUi(this);
}

city::~city()
{

}

void city::updateRoleInfo(void)
{

}