#include "city_smithy.h"

city_smithy::city_smithy(RoleInfo *roleInfo)
	: QWidget(NULL), myRole(roleInfo)
{
	ui.setupUi(this);

	ui.pushButton_6->setVisible(false);
	ui.pushButton_7->setVisible(false);
	ui.pushButton_8->setVisible(false);
	ui.pushButton_9->setVisible(false);
}

city_smithy::~city_smithy()
{

}
