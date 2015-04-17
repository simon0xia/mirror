#include "smithy.h"

smithy::smithy(RoleInfo *roleInfo)
	: QWidget(NULL), myRole(roleInfo)
{
	ui.setupUi(this);
}

smithy::~smithy()
{

}
