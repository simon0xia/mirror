#include "login_create.h"
#include "RoleDefine.h"
#include <QMessageBox>
#include <QFile>

login_create::login_create(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	vocation = 1;
	gender = 1;
	bCreate = false;
}

login_create::~login_create()
{

}

void login_create::on_btn_role_1_clicked()
{
	ui.edit_display->setText(QStringLiteral("男战士"));
	vocation = 1;
	gender = 1;
}
void login_create::on_btn_role_2_clicked()
{
	ui.edit_display->setText(QStringLiteral("女战士"));
	vocation = 1;
	gender = 2;
}
void login_create::on_btn_role_3_clicked()
{
	ui.edit_display->setText(QStringLiteral("男法师"));
	vocation = 2;
	gender = 1;
}
void login_create::on_btn_role_4_clicked()
{
	ui.edit_display->setText(QStringLiteral("女法师"));
	vocation = 2;
	gender = 2;
}
void login_create::on_btn_role_5_clicked()
{
	ui.edit_display->setText(QStringLiteral("男道士"));
	vocation = 3;
	gender = 1;
}
void login_create::on_btn_role_6_clicked()
{
	ui.edit_display->setText(QStringLiteral("女道士"));
	vocation = 3;
	gender = 2;
}
void login_create::on_btn_ok_clicked()
{
	QString strTmp = ui.edit_role_name->text();
	if (strTmp.isEmpty())
	{
		QString title = QStringLiteral("提示");
		QString message = QStringLiteral("用户名不可为空。");
		QMessageBox msgBox(QMessageBox::Question, title, message);
		msgBox.addButton(QStringLiteral(" 是 "), QMessageBox::AcceptRole);
		msgBox.exec();
	}
	else
	{
		bCreate = CreateRole(strTmp);
		if (!bCreate)
		{
			
		}
		else
		{
			done(QDialog::Accepted);
		}
	}
}

bool login_create::CreateRole(const QString &name)
{
	RoleInfo myRole;

	myRole.reputation = myRole.exp = 0;
	myRole.strength = myRole.wisdom = myRole.spirit = myRole.life = myRole.agility = myRole.potential = 0;

	myRole.vocation = vocation;
	myRole.gender = gender;
	myRole.level = 1;
	myRole.coin = 20000;
	myRole.gold = 1000;
#ifdef _DEBUG
	myRole.exp = 9000000;
#endif

	QFile file(SaveFileName);
	if (!file.open(QIODevice::WriteOnly))
	{
		return false;
	}

	QDataStream out(&file);
	out << SaveFileVer;
	//基本信息
	out << name << myRole.vocation << myRole.gender;
	out << myRole.coin << myRole.gold << myRole.reputation << myRole.exp << myRole.level;
	out << myRole.strength << myRole.wisdom << myRole.spirit << myRole.life << myRole.agility << myRole.potential;

	//道具背包、道具仓库、装备背包、装备仓库皆为空。
	quint32 bag_item_size, bag_equip_size, store_item_size, store_equip_size;
	bag_item_size = bag_equip_size = store_item_size = store_equip_size = 0;
	out << bag_item_size << bag_equip_size << store_item_size << store_equip_size;
	file.close();
	return true;
}