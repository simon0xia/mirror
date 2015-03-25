#include "role.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

role::role(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);


	LoadSettings("./expSetting.db");

	db_role = "./save.s";
	db_role += QString::number(FileVer) + "v";
	LoadRole();
}

role::~role()
{

}

void role::LoadRole()
{
	if (!QFile::exists(db_role))
	{
		if (!CreateRole())
		{
			QString message = QString::fromLocal8Bit("无法创建存档文件，请确定你是否拥有更改磁盘的权限。");
			QMessageBox::critical(this, tr("QMessageBox::critical()"), message);
			
			exit(0);
		}
	}

	QFile file(db_role);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString message = QString::fromLocal8Bit("无法打开存档文件，存档可能已损坏或版本不匹配。");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);

		exit(0);
	}
	qint32 ver;
	QDataStream out(file.readAll());
	out >> ver;
	out >> name >> coin >> gold >> reputation >> level >> exp;
	out >> strength >> wisdom >> spirit >> life >> agility >> potential;
	file.close();

	if (ver != FileVer)
	{
		QString message = QString::fromLocal8Bit("无法打开存档文件，存档可能已损坏或版本不匹配。");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);

		exit(0);
	}
	DisplayRoleValue();
}
void role::DisplayRoleValue()
{
	QString strTmp;

	ui.edit_role_name->setText(name);
	ui.edit_role_coin->setText(QString::number(coin));
//	ui.edit_role_gold->setText(name);
	ui.edit_role_reputation->setText(QString::number(reputation));
	ui.edit_role_level->setText(QString::number(level));

	strTmp = QString::number(exp) + "/" + QString::number(lvExp[level]);
	ui.edit_role_exp->setText(strTmp);
	ui.edit_role_strength->setText(QString::number(strength));
	ui.edit_role_wisdom->setText(QString::number(wisdom));
	ui.edit_role_spirit->setText(QString::number(spirit));
	ui.edit_role_life->setText(QString::number(life));
	ui.edit_role_agility->setText(QString::number(agility));
	ui.edit_role_potential->setText(QString::number(potential));
}

bool role::CreateRole()
{
	QFile file(db_role);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		return false;
	}

	name = QString::fromLocal8Bit("mirror传奇");
	level = 1;
	coin = gold = reputation = exp = 0;
	strength = wisdom = spirit = life = agility = potential = 0;

	QDataStream out(&file);	
	out << FileVer;
	out << name << coin << gold << reputation << level << exp;
	out << strength << wisdom << spirit << life << agility << potential;
	file.close();

	return true;
}

void role::LoadSettings(QString fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QString message = QString::fromLocal8Bit("加载失败，请重新运行游戏。");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);

		exit(0);
	}
	QDataStream out(file.readAll());
	for (qint32 i = 0; i < MaxLv && !out.atEnd(); i++)
	{
		out >> lvExp[i];
	}
}

void role::on_btn_mirror_save_clicked()
{
	QString strTmp = __FUNCTION__;
	ui.edit_display->setText(strTmp);
}
void role::on_btn_role_wisdom_clicked()
{
	QString strTmp = __FUNCTION__;
	ui.edit_display->setText(strTmp);
}
void role::on_btn_role_spirit_clicked()
{
	QString strTmp = __FUNCTION__;
	ui.edit_display->setText(strTmp);
}
void role::on_btn_role_life_clicked()
{
	QString strTmp = __FUNCTION__;
	ui.edit_display->setText(strTmp);
}
void role::on_btn_role_agility_clicked()
{
	QString strTmp = __FUNCTION__;
	ui.edit_display->setText(strTmp);
}
void role::on_btn_role_lvUp_clicked()
{
	QString strTmp = __FUNCTION__;
	ui.edit_display->setText(strTmp);
}