#include "role.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

extern QList<ItemInfo> g_ItemList;

role::role(RoleInfo *roleInfo, MapItem &bag_item, MapItem &storage_item)
: myTabFrame(NULL)
, myRole(roleInfo)
, m_bag_item(bag_item)
, m_storage_item(storage_item)
, m_tab_bagItem(bag_item)
, m_tab_storageItem(NULL)
{
	ui.setupUi(this);

	LoadSettings("./expSetting.db");

	db_role = "./save.s";
	db_role += QString::number(FileVer) + "v";
	LoadRole();

// 	ui.tabWidget_bag->addTab(&bag_equip, QString::fromLocal8Bit("装备"));
 	ui.tabWidget_bag->addTab(&m_tab_bagItem, QString::fromLocal8Bit("道具"));
// 	ui.tabWidget_bag->addTab(&storage_equip, QString::fromLocal8Bit("装备仓库"));
 	ui.tabWidget_bag->addTab(&m_tab_storageItem, QString::fromLocal8Bit("道具仓库"));
}

role::~role()
{

}

void role::updateRoleInfo(void)
{
	DisplayRoleValue();
	
	m_tab_bagItem.updateItemInfo(g_ItemList, m_bag_item);
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
	if (!file.open(QIODevice::ReadOnly))
	{
		QString message = QString::fromLocal8Bit("无法打开存档文件，存档可能已损坏或版本不匹配。");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);

		exit(0);
	}

	qint32 ver, len;
	QDataStream out(file.readAll());
	out >> ver;
	out >> myRole->name >> myRole->coin >> myRole->gold >> myRole->reputation >> myRole->level >> myRole->exp;
	out >> myRole->strength >> myRole->wisdom >> myRole->spirit >> myRole->life >> myRole->agility >> myRole->potential;
	file.close();

	if (ver != FileVer)
	{
		QString message = QString::fromLocal8Bit("无法打开存档文件，存档可能已损坏或版本不匹配。");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);

		exit(0);
	}
	myRole->lvExp = lvExp[myRole->level];
	DisplayRoleValue();
}
void role::DisplayRoleValue(void)
{
	QString strTmp;
	qint32 nTmp;
	double dTmp;

	ui.edit_role_name->setText(myRole->name);
	ui.edit_role_coin->setText(QString::number(myRole->coin));
	ui.edit_role_reputation->setText(QString::number(myRole->reputation));
	ui.edit_role_level->setText(QString::number(myRole->level));

	strTmp = QString::number(myRole->exp) + "/" + QString::number(myRole->lvExp);
	ui.edit_role_exp->setText(strTmp);
	ui.edit_role_strength->setText(QString::number(myRole->strength));
	ui.edit_role_wisdom->setText(QString::number(myRole->wisdom));
	ui.edit_role_spirit->setText(QString::number(myRole->spirit));
	ui.edit_role_life->setText(QString::number(myRole->life));
	ui.edit_role_agility->setText(QString::number(myRole->agility));
	ui.edit_role_potential->setText(QString::number(myRole->potential));

	dTmp = qMin(2.5, 1 + myRole->agility * 0.01);
	ui.edit_role_AttackSpeed->setText(QString::number(dTmp));

	nTmp = myRole->level * 1.0 + myRole->strength * 1;
	ui.edit_role_PhysicsAttack->setText(QString::number(nTmp));

	nTmp = myRole->level * 0.9 + myRole->wisdom * 1;
	ui.edit_role_MagicAttack->setText(QString::number(nTmp));

	nTmp = myRole->level * 0.8 + myRole->spirit * 1;
	ui.edit_role_SpellAttack->setText(QString::number(nTmp));

	nTmp = 0;
	ui.edit_role_PhysicsGuard->setText(QString::number(nTmp));

	nTmp = 0;
	ui.edit_role_ElementGuard->setText(QString::number(nTmp));

	nTmp = 0;
	ui.edit_role_extrarate->setText(QString::number(nTmp));

	nTmp = 0;
	ui.edit_role_extrahurt->setText(QString::number(nTmp));

	myRole->hp_m = 50 + myRole->level * 5 + myRole->life * 25;
	ui.edit_role_hp->setText(QString::number(myRole->hp_m));

	myRole->mp_m = myRole->level * 3 + myRole->life * 25;
	ui.edit_role_mp->setText(QString::number(myRole->mp_m));

	myRole->ap_m = 10;
	ui.edit_role_ap->setText(QString::number(myRole->ap_m));

	nTmp = 0;
	ui.edit_role_rhp->setText(QString::number(nTmp));

	nTmp = 0;
	ui.edit_role_rmp->setText(QString::number(nTmp));

	nTmp = 0;
	ui.edit_role_rap->setText(QString::number(nTmp));

	if (myRole->potential <= 0)
	{
		ui.btn_role_strength->setDisabled(true);
		ui.btn_role_wisdom->setDisabled(true);
		ui.btn_role_spirit->setDisabled(true);
		ui.btn_role_life->setDisabled(true);
		ui.btn_role_agility->setDisabled(true);
	}
	else
	{
		ui.btn_role_strength->setDisabled(false);
		ui.btn_role_wisdom->setDisabled(false);
		ui.btn_role_spirit->setDisabled(false);
		ui.btn_role_life->setDisabled(false);
		ui.btn_role_agility->setDisabled(false);
	}

	if (myRole->exp < myRole->lvExp)
	{
		ui.btn_role_lvUp->setDisabled(true);
	}
	else
	{
		ui.btn_role_lvUp->setDisabled(false);
	}
}

bool role::CreateRole()
{
	QString name = QString::fromLocal8Bit("mirror传奇");
	myRole->level = 1;
	myRole->coin = myRole->gold = myRole->reputation = myRole->exp = 0;
	myRole->strength = myRole->wisdom = myRole->spirit = myRole->life = myRole->agility = myRole->potential = 0;

	myRole->coin = 100000;
	myRole->gold = 1000;

	QFile file(db_role);
	if (!file.open(QIODevice::WriteOnly))
	{
		return false;
	}

	QDataStream out(&file);
	out << FileVer;
	out << name << myRole->coin << myRole->gold << myRole->reputation << myRole->level << myRole->exp;
	out << myRole->strength << myRole->wisdom << myRole->spirit << myRole->life << myRole->agility << myRole->potential;
	file.close();
	return true;
}

void role::LoadSettings(QString fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
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
	QFile file(db_role);
	if (!file.open(QIODevice::WriteOnly))
	{
		QString message = QString::fromLocal8Bit("无法保存，存档可能已损坏或不存在。");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);
	}

	QDataStream out(&file);
	out << FileVer;
	out << myRole->name << myRole->coin << myRole->gold << myRole->reputation << myRole->level << myRole->exp;
	out << myRole->strength << myRole->wisdom << myRole->spirit << myRole->life << myRole->agility << myRole->potential;
	file.close();
}
void role::on_btn_role_strength_clicked()
{
	--myRole->potential;
	++myRole->strength;
	DisplayRoleValue();
}
void role::on_btn_role_wisdom_clicked()
{
	--myRole->potential;
	++myRole->wisdom;
	DisplayRoleValue();
}
void role::on_btn_role_spirit_clicked()
{
	--myRole->potential;
	++myRole->spirit;
	DisplayRoleValue();
}
void role::on_btn_role_life_clicked()
{
	--myRole->potential;
	++myRole->life;
	DisplayRoleValue();
}
void role::on_btn_role_agility_clicked()
{
	--myRole->potential;
	++myRole->agility;
	DisplayRoleValue();
}
void role::on_btn_role_lvUp_clicked()
{
//	myRole->lvExp = lvExp[myRole->level];
	myRole->exp -= myRole->lvExp;
	myRole->lvExp = lvExp[myRole->level];
	++myRole->level;
	myRole->potential += 5;

	DisplayRoleValue();
}