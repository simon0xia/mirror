#include "city_smithy.h"

city_smithy::city_smithy(RoleInfo *roleInfo)
	: QWidget(NULL), myRole(roleInfo)
{
	ui.setupUi(this);
}

city_smithy::~city_smithy()
{

}
