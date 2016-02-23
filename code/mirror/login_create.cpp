#include "login_create.h"
#include <QMessageBox>
#include <QFile>
#include <QMovie>
#include "RoleDefine.h"
#include "def_System_para.h"
#include "cryptography.h"
#include "MirrorVersion.h"

login_create::login_create(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.lbl_role_photo->setAttribute(Qt::WA_TranslucentBackground, true);

	m_vocation = Voc_Warrior;
	m_gender = 1;
	on_btn_vocation_1_clicked();
	bCreate = false;
}

login_create::~login_create()
{

}

void login_create::changePhoto()
{
	qint32 headNo = ((m_vocation - 1) * 2 + m_gender) * 10;
	QString path = (":/mirror/Resources/role/") + QString::number(headNo) + ".gif";

	QPoint pos;
	switch (headNo)
	{
	case 10: pos = QPoint(100, 40); break;
	case 20: pos = QPoint(50, 60); break;
	case 30: pos = QPoint(90, 40); break;
	case 40: pos = QPoint(60, 50); break;
	case 50: pos = QPoint(80, 40); break;
	case 60: pos = QPoint(70, 50); break;
	default: pos = QPoint(100, 40); break;
	}
	ui.lbl_role_photo->move(pos);

	QMovie *movie = new QMovie(path);
	ui.lbl_role_photo->setMovie(movie);
	movie->setSpeed(50);
	movie->start();
}
void login_create::on_btn_vocation_1_clicked()
{
	m_vocation = Voc_Warrior;
	changePhoto();
}
void login_create::on_btn_vocation_2_clicked()
{
	m_vocation = Voc_Magic;
	changePhoto();
}
void login_create::on_btn_vocation_3_clicked()
{
	m_vocation = Voc_Taoist;
	changePhoto();
}
void login_create::on_btn_gender_m_clicked()
{
	m_gender = 1;
	changePhoto();
}
void login_create::on_btn_gender_f_clicked()
{
	m_gender = 2;
	changePhoto();
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
		bCreate = CreateAccount(strTmp);
		if (bCreate)
		{
			done(QDialog::Accepted);
		}
	}
}
void login_create::on_btn_quit_clicked()
{
	done(Rejected);
}

bool login_create::CreateAccount(const QString &name)
{
	int32_t accountId_H, accountId_L;
	int32_t level, edtLv, coin, gold, reputation, soul, yuanli, exp, keepSign, taskCount;
	uint32_t preSignTime;
	int32_t FightEdtIndex, MaxMap;
	qint32 skill_study_count = 0;
	Info_Equip onWearEquip[MaxEquipCountForRole] = { 0 };
	qint32 resver = 0;
	QString strTmp;

	accountId_H = accountId_L = 0;
	reputation = soul = yuanli = exp = 0;
	FightEdtIndex  = keepSign = taskCount = 0;
	preSignTime = 0;
	MaxMap = 15;
	edtLv = 0;
	level = 1;
	coin = 50000;
	gold = 1000;
#ifdef _DEBUG
	exp = 900000000;
	coin = 10000000;
#endif

	QByteArray save_plain, save_cryptograph;
	QDataStream out(&save_plain, QIODevice::WriteOnly);
	out << version_major << version_minor << version_build << SaveFileVer;
	//基本信息
	out << accountId_H << accountId_L << level << exp;
	out << coin << gold << reputation << soul << yuanli;
	out << FightEdtIndex << MaxMap << keepSign << preSignTime;
	out << taskCount << taskCount << taskCount;

	//角色本尊基本信息、装备、技能。
	out.writeRawData(name.toStdString().c_str(), 128);
	out << m_vocation << m_gender << level << exp;
	out << resver << resver << resver << resver << resver;
	out << resver << resver << resver << resver << resver;
	out.writeRawData((char *)&onWearEquip, sizeof(onWearEquip));
	out << skill_study_count;

	//分身战基本信息、装备、技能。
	strTmp = QStringLiteral("战士分身");
	out.writeRawData(strTmp.toStdString().c_str(), 128);
	out << (int32_t)Voc_Warrior << m_gender << edtLv << exp;
	out << resver << resver << resver << resver << resver;
	out << resver << resver << resver << resver << resver;
	out.writeRawData((char *)&onWearEquip, sizeof(onWearEquip));
	out << skill_study_count;

	//分身法基本信息、装备、技能。
	strTmp = QStringLiteral("法师分身");
	out.writeRawData(strTmp.toStdString().c_str(), 128);
	out << (int32_t)Voc_Magic << m_gender << edtLv << exp;
	out << resver << resver << resver << resver << resver;
	out << resver << resver << resver << resver << resver;
	out.writeRawData((char *)&onWearEquip, sizeof(onWearEquip));
	out << skill_study_count;

	//分身道基本信息、装备、技能。
	strTmp = QStringLiteral("道士分身");
	out.writeRawData(strTmp.toStdString().c_str(), 128);
	out << (int32_t)Voc_Taoist << m_gender << edtLv << exp;
	out << resver << resver << resver << resver << resver;
	out << resver << resver << resver << resver << resver;
	out.writeRawData((char *)&onWearEquip, sizeof(onWearEquip));
	out << skill_study_count;

	//道具背包、道具仓库、装备背包、装备仓库皆为空。
	qint32 bag_item_size = 3;
	out << bag_item_size << 220001 << 1 << 220003 << 1 << 220004 << 1;

	//背包内放置基本装备。
	Info_Equip equip = { 0 };
	QVector<itemID> VecEquip = { 301001, 302001, 303001 };
	out << VecEquip.size();
	for (qint32 i = 0; i < VecEquip.size(); i++)
	{
		equip.ID = VecEquip[i];
		out.writeRawData((char *)&equip, sizeof(Info_Equip));
	}

	qint32 store_equip_size = 0;
	out << store_equip_size;

	if (!cryptography::Encrypt(save_cryptograph, save_plain))
	{
		return false;
	}

	QFile file(SaveFileName);
	if (!file.open(QIODevice::WriteOnly))
	{
		return false;
	}
	QDataStream dfs(&file);
	dfs.writeRawData(save_cryptograph.data(), save_cryptograph.size());
	dfs.writeRawData(save_plain.data(), save_plain.size());
	file.close();
	return true;
}