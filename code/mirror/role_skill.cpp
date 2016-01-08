#include "role_skill.h"
#include "Item_Base.h"

extern QMap<skillID, Info_SkillBasic> g_SkillBasic;
extern QMap<qint32, Info_SkillDamage> g_SkillDamage;
extern QMap<qint32, Info_SkillBuff> g_SkillBuff;
extern QMap<qint32, Info_SkillTreat> g_SkillTreat;

QString GeneralDamageInfo(qint32 DamageNo, qint32 studyLevel);
QString GeneralBuffInfo(qint32 BuffNo, qint32 studyLevel);
QString GeneralTreatInfo(qint32 TreatNo, qint32 studyLevel);

role_skill::role_skill(QWidget *parent, CPlayer *w_player, role *w_widget_role)
	: QDialog(parent)
	, player(w_player), widget_role(w_widget_role)
{
	ui.setupUi(this);

	m_voc = player->get_voc();
	m_skill_study = player->get_skill();
	InitUI(m_voc);
	btnIndex = 0;
	si = 0;
	for (auto iter = m_skill_study->begin(); iter != m_skill_study->end(); iter++)
	{
		if (iter->usdIndex > si)
		{
			si = iter->usdIndex;
		}
	}
	++si;
	
	for (qint32 i = 0; i < skillBtn.size(); i++)
	{
		connect(skillBtn[i], &QPushButton::clicked, [=]() {
			process_btn_Tree(i);});
	}
	connect(ui.checkBox_use, SIGNAL(stateChanged(int)), this, SLOT(on_checkBox_used_stateChanged(int)));

	process_btn_Tree(btnIndex);
}

role_skill::~role_skill()
{

}

void role_skill::closeEvent(QCloseEvent *event)
{
	on_btn_close_clicked();
}

void role_skill::on_btn_ok_clicked(void)
{
	qint32 id;
	roleSkill rs;

	for (int i = 0; i < lbl_SI.size(); i++)
	{
		id = lbl_SI[i]->whatsThis().toUInt();
		if (m_skill_study->contains(id))
		{
			rs = m_skill_study->value(id);
			rs.usdIndex = lbl_SI[i]->text().toUInt();
			m_skill_study->insert(id, rs);
		}	
	}
	done(QDialog::Accepted);
}
void role_skill::on_btn_close_clicked(void)
{
	done(QDialog::Rejected);
}

void role_skill::on_btn_reset_clicked(void)
{
	roleSkill sk;
	skillID id;

	foreach(roleSkill var , *m_skill_study)
	{
		var.usdIndex = 0;

		m_skill_study->insert(var.id, var);
	}
	foreach(QLabel *lbl, lbl_SI)
	{
		id = lbl->whatsThis().toUInt();
		if (m_skill_study->contains(id))
		{
			lbl->setText(QString::number(m_skill_study->value(id).usdIndex));
		}	
	}
	si = 1;
}

void role_skill::on_btn_study_clicked(void)
{
	player->sub_coin(spend);

	if (m_skill_study->contains(currentSkillID))
	{
		roleSkill skill = m_skill_study->value(currentSkillID);
		skill.level += 1;
		m_skill_study->insert(currentSkillID, skill);
	}
	else
	{
		roleSkill skill = { currentSkillID, 1, 0 };
		m_skill_study->insert(currentSkillID, skill);

		lbl_SI[btnIndex]->setText("0");
	}

	widget_role->updateRoleInfo();
	process_btn_Tree(btnIndex);
}
void role_skill::on_btn_help_clicked(void)
{
	dlg_help = new General_GameIntroduce(this);
	dlg_help->setWindowFlags(Qt::WindowStaysOnTopHint);
	dlg_help->show();
}

inline const Info_SkillBasic *role_skill::FindSkill(skillID id)
{
	foreach(const Info_SkillBasic &info, g_SkillBasic)
	{
		if (info.ID == id)
		{
			return &info;
		}
	}
	return nullptr;
}

bool role_skill::InitUI(RoleVoc voc)
{
	bool bRes = true;
	QStringList skillNaseList;

	//不显示第一个技能。
	auto iter = g_SkillBasic.constBegin();
	iter++;
	for (; iter != g_SkillBasic.constEnd(); iter++)
	{
		if (iter->type != 0 && voc == Item_Base::FindItem_Item(iter->ID)->vocation)
		{
			QPushButton *btn = new QPushButton(this);
			btn->setIconSize(QSize(34, 34));
			btn->setIcon(iter->icon);
			btn->setWhatsThis(QString::number(iter->ID));
			skillBtn.append(btn);

			QLabel *check = new QLabel(this);
			check->setWhatsThis(QString::number(iter->ID));
			lbl_SI.append(check);

			if (m_skill_study->contains(iter->ID))
			{
				check->setText(QString::number(m_skill_study->value(iter->ID).usdIndex));
			}
		}
	}

	QSize BtnSize(42, 42);
	QSize EditSize(18, 12);
	if (voc == Voc_Warrior) {
		bRes = InitSkillTree_Warrior(BtnSize, EditSize);
	} else if (voc == Voc_Magic) {
		bRes = InitSkillTree_Magic(BtnSize, EditSize);
	} else if (voc == Voc_Taoist) {
		bRes = InitSkillTree_Taoist(BtnSize, EditSize);
	}

	return bRes;
}

bool role_skill::InitSkillTree_Warrior(const QSize& btnSize, const QSize& CheckSize)
{
	if (skillBtn.size() != 13)
	{
		return false;
	}

	//连接线的位置及大小。
	QRect rtLine_V[10] = { QRect(103, 62, 36, 28), QRect(103, 132, 36, 48), QRect(103, 222, 36, 48), QRect(23, 242, 36, 58), QRect(183, 262, 36, 58),
		QRect(263, 92, 36, 28), QRect(263, 262, 36, 28), QRect(263, 332, 36, 28), QRect(23, 157, 36, 43), QRect(183, 157, 36, 63) };
	QRect rtLine_H[1] = { QRect(37, 142, 168, 36) };

	CreateLine_H(rtLine_H, 1);
	CreateLine_V(rtLine_V, 10);

	//技能格子的位置。
	QPoint point[13] = { QPoint(100, 20), QPoint(100, 90), QPoint(100, 180), QPoint(20, 200), QPoint(180, 220),
		QPoint(260, 50), QPoint(260, 220), QPoint(100, 270), QPoint(180, 320), QPoint(20, 300), QPoint(260, 290), QPoint(260, 360), QPoint(260, 120) };

	QPoint ptOffset = QPoint(btnSize.width(), 2);
	CreateSkillBtn(btnSize, CheckSize, point, ptOffset);
	return true;
}
bool role_skill::InitSkillTree_Magic(const QSize& btnSize, const QSize& CheckSize)
{
	if (skillBtn.size() != 13)
	{
		return false;
	}

	//连接线的位置及大小。
	QRect rtLine_V[8] = { QRect(23, 145, 36, 25), QRect(23, 302, 36, 28), QRect(103, 62, 36, 28), QRect(103, 132, 36, 28), 
		QRect(103, 202, 36, 148), QRect(183, 112, 36, 68), QRect(183, 222, 36, 28),QRect(183, 292, 36, 28) };
	QRect rtLine_H[1] = { QRect(37, 125, 80, 36) };

	CreateLine_H(rtLine_H, 1);
	CreateLine_V(rtLine_V, 8);

	QPoint point[13] = { QPoint(100, 20), QPoint(100, 90), QPoint(20, 330), QPoint(180, 70), QPoint(180, 180),
		QPoint(260, 110), QPoint(20, 170), QPoint(100, 160) ,QPoint(180, 250), QPoint(260, 220), QPoint(20, 260), 
		QPoint(180, 320) , QPoint(100, 350) };

	QPoint ptOffset = QPoint(btnSize.width(), 2);
	CreateSkillBtn(btnSize, CheckSize, point, ptOffset);
	
	return true;
}
bool role_skill::InitSkillTree_Taoist(const QSize& btnSize, const QSize& CheckSize)
{
	if (skillBtn.size() != 17)
	{
		return false;
	}

	//连接线的位置及大小。
	QRect rtLine_V[9] = { QRect(23, 102, 36, 28), QRect(23, 252, 36, 28), QRect(103, 52, 36, 28), QRect(103, 122, 36, 28),
		QRect(103, 272, 36, 28), QRect(173, 102, 36, 28), QRect(253, 122, 36, 28), QRect(253, 192, 36, 28), QRect(253, 262, 36, 28) };

	CreateLine_V(rtLine_V, 9);

	QPoint point[17] = { QPoint(20, 60), QPoint(100, 10), QPoint(100, 230), QPoint(100, 80), QPoint(20, 210), QPoint(20, 280),
		QPoint(170, 240), QPoint(250, 80), QPoint(170, 60), QPoint(170, 130), QPoint(100, 300), QPoint(20, 130),
		QPoint(250, 150), QPoint(100, 150), QPoint(250, 220), QPoint(250, 290), QPoint(170, 320) };

	QPoint ptOffset = QPoint(btnSize.width(), 2);
	CreateSkillBtn(btnSize, CheckSize, point, ptOffset);

	return true;
}

bool role_skill::CreateSkillBtn(const QSize& btnSize, const QSize& CheckSize, const QPoint *point, const QPoint &ptOffset)
{
	for (int i = 0; i < skillBtn.size(); i++)
	{
		skillBtn[i]->setGeometry(QRect(point[i], btnSize));
		lbl_SI[i]->setGeometry(QRect(point[i] + ptOffset, CheckSize));
	}
	return true;
}

bool role_skill::CreateLine_V(const QRect *rtLine, qint32 nCount)
{
	for (qint32 i = 0; i < nCount; i++)
	{
		QLabel *lbl = new QLabel(this);
		lbl->setGeometry(rtLine[i]);
		lbl->setPixmap(QPixmap(":/mirror/Resources/Line_V.png"));
		lbl->setScaledContents(true);
	}
	return true;
}

bool role_skill::CreateLine_H(const QRect *rtLine, qint32 nCount)
{
	for (qint32 i = 0; i < nCount; i++)
	{
		QLabel *lbl = new QLabel(this);
		lbl->setGeometry(rtLine[i]);
		lbl->setPixmap(QPixmap(":/mirror/Resources/Line_H.png"));
		lbl->setScaledContents(true);
	}
	return true;
}

void role_skill::process_btn_Tree(quint32 nIndex)
{
	btnIndex = nIndex;
	currentSkillID = skillBtn[nIndex]->whatsThis().toUInt();
	const Info_SkillBasic *skill = FindSkill(currentSkillID);

	//技能学习等级
	qint32 needLv = -1;
	qint32 studyLevel = m_skill_study->value(currentSkillID).level;
	const Info_Item *item = Item_Base::FindItem_Item(currentSkillID);
	if (item != nullptr)
	{
		needLv = item->level;
	}

	if (skill == nullptr)
	{
		return;
	}
	ui.btn_skill->setIcon(skill->icon);

	ui.lbl_name->setText(skill->name);
	ui.lbl_name->setWhatsThis(QString::number(nIndex));
	ui.lbl_lv->setText(QString("%1 / %2").arg(studyLevel).arg(skill->level));
	ui.checkBox_use->setChecked(lbl_SI[nIndex]->text().toUInt() > 0);
	ui.checkBox_use->setEnabled(studyLevel > 0);

	ui.edit_cur->setText(QStringLiteral("角色等级为 <font color = magenta>%1级</font> 时可学习").arg(needLv));
	ui.edit_cur->append("");		//blank

	QString strTmp;
	switch (skill->type)
	{
	case 1:strTmp = QStringLiteral("主动攻击技能"); break;
	case 2:strTmp = QStringLiteral("增益技能"); break;
	case 3:strTmp = QStringLiteral("减益技能"); break;
	case 4:strTmp = QStringLiteral("召唤"); break;
	case 5:strTmp = QStringLiteral("治疗技能"); break;
	default: strTmp = QStringLiteral("未知");break;
	}

	ui.edit_cur->append(QStringLiteral("<font color = green>技能类型：%1 </font>").arg(strTmp));

	qint32 cd = (studyLevel == 0) ? skill->cd[0] : skill->cd[studyLevel - 1];
	ui.edit_cur->append(QStringLiteral("<font color = green>冷却时间：%1 </font>").arg(cd));
	ui.edit_cur->append(QStringLiteral("<font color = green>技能说明:</font>"));

	switch (skill->type)
	{
	case 1:	strTmp = GeneralDamageInfo(skill->no, studyLevel);break;
	case 2:	strTmp = GeneralBuffInfo(skill->no, studyLevel);break;
	case 3:	strTmp = skill->descr;break;
	case 5:	strTmp = GeneralTreatInfo(skill->no, studyLevel);break;
	default:strTmp = skill->descr;break;
	}
	ui.edit_cur->append(strTmp);
	process_StudyInfo(needLv, studyLevel, skill->level);
}

void role_skill::on_checkBox_used_stateChanged(int state)
{
	qint32 nIndex = ui.lbl_name->whatsThis().toUInt();
	qint32 nTmp, n;

	if (state == Qt::Unchecked)
	{
// 		if (m_skill_study->value(currentSkillID).level <= 0) {
// 			lbl_SI[nIndex]->setText("");
// 		} 
// 		else
 		{
			nTmp = lbl_SI[nIndex]->text().toUInt();
			if (nTmp > 0)
			{				
				lbl_SI[nIndex]->setText("0");

				for (int32_t i = 0; i < lbl_SI.size(); i++)
				{
					n = lbl_SI[i]->text().toUInt();
					if (n >= nTmp)
					{
						lbl_SI[i]->setText(QString::number(n - 1));
					}
				}
				--si;
			}		
		}
	}
	else if (state == Qt::Checked)
	{
		if (lbl_SI[nIndex]->text().toUInt() <= 0)
		{
			lbl_SI[nIndex]->setText(QString::number(si++));
		}
	}
}

void role_skill::process_StudyInfo(qint32 lv, qint32 studyLevel, qint32 maxSkillLv)
{
	QString strTmp = QStringLiteral("晋级");
	qint32 nTmp = Item_Base::FindItem_Item(currentSkillID)->coin;
	ui.btn_study->setDisabled(true);
	if (player->get_lv() < lv)
	{		
		ui.lbl_study->setText(QStringLiteral("角色等级过低"));
	}
	else if (studyLevel <= 0)
	{
		ui.lbl_study->setText(QStringLiteral("尚未学习技能。"));
	}
	else if (studyLevel < maxSkillLv)
	{
		spend = studyLevel * 10 * nTmp;
		ui.lbl_study->setText(QStringLiteral("可花费%1金币将技能提升到%2级。").arg(spend).arg(studyLevel + 1));
		
		if (player->get_coin() < spend) {
			strTmp += QStringLiteral("\n(金币不足)");
		} 
		else {
			ui.btn_study->setDisabled(false);		
		}
	}
	else {
		ui.lbl_study->setText(QStringLiteral("已达到最高级"));
	}
	ui.btn_study->setText(strTmp);
}

QString GeneralDamageInfo(qint32 DamageNo, qint32 studyLevel)
{
	const Info_SkillDamage &sd = g_SkillDamage.value(DamageNo);
	QString strDamageType = sd.type == 1 ? QStringLiteral("物理") : QStringLiteral("魔法");
	QString strDamageVoc = sd.type == 3 ? QStringLiteral("道术") : (sd.type == 2 ? QStringLiteral("魔法") : QStringLiteral("攻击"));

	QString strTmp = QStringLiteral("随机对 %1 个目标各造成 %2 次%3伤害，伤害值为 %4 * %5%  + %6。")
		.arg(sd.targets).arg(sd.times).arg(strDamageType).arg(strDamageVoc)
		.arg(sd.basic + studyLevel * sd.add).arg(sd.extra);
	return strTmp;
}

QString GeneralBuffInfo(qint32 BuffNo, qint32 studyLevel)
{
	const Info_SkillBuff &sb = g_SkillBuff.value(BuffNo);
	QString strTargets, strEffect;
	if (sb.targets == -1)
		strTargets = QStringLiteral("我方全体");
	else if (sb.targets == 0)
		strTargets = QStringLiteral("自身");
	else
		strTargets = QStringLiteral("我方%1个成员").arg(sb.targets);
	
	qint32 nTmp = sb.basic + studyLevel * sb.add;
	switch (sb.et)
	{
	case et_DamageEnhance:strEffect = QStringLiteral("伤害增强%1").arg(nTmp); break;
	case et_DamageSave:strEffect = QStringLiteral("伤害减免%1").arg(nTmp); break;
	case et_ac_fixed:strEffect = QStringLiteral("增加防御:道术*%1").arg(nTmp); break;
	case et_mac_fixed:strEffect = QStringLiteral("增加魔御:道术*%1").arg(nTmp); break;
	case et_ac_percent:strEffect = QStringLiteral("增加防御*%1").arg(nTmp); break;
	case et_mac_percent:strEffect = QStringLiteral("增加魔御*%1").arg(nTmp); break;
	case et_speed:strEffect = QStringLiteral("增加攻击速度%1").arg(nTmp); break;
	default:
		strEffect = QStringLiteral("出错啦");
		break;
	}

	QString strTmp = QStringLiteral("%1%2%,持续%3回合。")
		.arg(strTargets).arg(strEffect).arg(sb.time);
	return strTmp;
}

QString GeneralTreatInfo(qint32 TreatNo, qint32 studyLevel)
{
	const Info_SkillTreat &st = g_SkillTreat.value(TreatNo);
	QString strTargets = (st.targets == -1) ? QStringLiteral("所有") : QStringLiteral("血量最少的%1个").arg(st.targets);

	QString strTmp = QStringLiteral("治疗%1目标，恢复目标最大生命值的%2%。")
		.arg(strTargets).arg(st.hpr_basic + st.hpr_add * studyLevel);
	return strTmp;
}