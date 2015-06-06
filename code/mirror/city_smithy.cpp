#include "city_smithy.h"

city_smithy::city_smithy(QWidget *parrent, RoleInfo *roleInfo)
	: QWidget(parrent), myRole(roleInfo)
{
	ui.setupUi(this);

}

city_smithy::~city_smithy()
{

}

void city_smithy::on_btn_close_clicked(void)
{
	close();
}