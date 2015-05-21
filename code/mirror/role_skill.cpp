#include "role_skill.h"

extern QVector<Info_skill> g_skillList;

role_skill::role_skill(QWidget *parent, const VecRoleSkill *skill_study, VecRoleSkill *skill_fight)
	: QDialog(parent)
	, m_skill_study(skill_study), m_skill_fight(skill_fight)
{
	ui.setupUi(this);
	ui.btn_pgDn->setVisible(false);
	ui.btn_pgUp->setVisible(false);
	ui.textEdit->setStyleSheet("QTextEdit{ background:transparent} ");

	QString strTmp, strDescr;
	QListWidgetItem item;
	for (VecRoleSkill::const_iterator iter = skill_study->begin(); iter != skill_study->end(); iter++)
	{
		const Info_skill *info = FindSkill(iter->id);
		if (info != nullptr)
		{
			item.setIcon(info->icon);
			item.setText(info->name);

			strDescr = info->name;
			strDescr += QStringLiteral("\nCD:") + QString::number(info->cd);
			strDescr += QStringLiteral("\nLv:") + QString::number(iter->level);
			strDescr += QStringLiteral("\n说明：\n  ") + info->descr;
			item.setToolTip(strDescr);
			ui.listWidget->addItem(new QListWidgetItem(item));
		}
	}

	for (VecRoleSkill::const_iterator iter = m_skill_fight->begin(); iter != m_skill_fight->end(); iter++)
	{
		tmpSkill_fight.append(*iter);
	}

	skillSequence.append(ui.btn_skill_1);
	skillSequence.append(ui.btn_skill_2);
	skillSequence.append(ui.btn_skill_3);
	skillSequence.append(ui.btn_skill_4);
	skillSequence.append(ui.btn_skill_5);
	skillSequence.append(ui.btn_skill_6);
	skillSequence.append(ui.btn_skill_7);
	skillSequence.append(ui.btn_skill_8);
	skillSequence.append(ui.btn_skill_9);
	skillSequence.append(ui.btn_skill_10);
	DisplaySkillSequence();

	btnIndex = 1;

	connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(SetSkillInFighting(QListWidgetItem *)));
	
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
	m_skill_fight->clear();
	if (tmpSkill_fight.size() == 0)
	{
		roleSkill skill = { g_skillList.at(0).ID, g_skillList.at(0).level };
		m_skill_fight->append(skill);
	}
	else
	{
		for (VecRoleSkill::const_iterator iter = tmpSkill_fight.begin(); iter != tmpSkill_fight.end(); iter++)
		{
			m_skill_fight->append(*iter);
		}
	}
	done(QDialog::Accepted);
}
void role_skill::on_btn_close_clicked(void)
{
	done(QDialog::Rejected);
}

void role_skill::ProcessSkillBtn()
{
	if (btnIndex <= tmpSkill_fight.size())
	{
		tmpSkill_fight.removeAt(btnIndex - 1);
		DisplaySkillSequence();
	}
}

void role_skill::SetSkillInFighting(QListWidgetItem * item)
{
	qint32 nTmp, nIndex;

	if (tmpSkill_fight.size() >= MaxFightSkill)
	{
		return;
	}

	qint32 index = ui.listWidget->currentRow();
	const roleSkill &skill_study = m_skill_study->at(index);
	for (quint32 i = 0; i < tmpSkill_fight.size(); i++)
	{
		if (tmpSkill_fight[i].id == skill_study.id)
		{
			//不允许重复
			return;
		}
	}

	tmpSkill_fight.append(skill_study);
	DisplaySkillSequence();
}

void role_skill::DisplaySkillSequence(void)
{
	for (qint32 i = 0; i < MaxFightSkill && i < tmpSkill_fight.size(); i++)
	{
		const Info_skill *info = FindSkill(tmpSkill_fight.at(i).id);
		if (info != nullptr)
		{
			skillSequence[i]->setIcon(info->icon);
			skillSequence[i]->setToolTip(info->name);
		}
	}
	for (qint32 i = tmpSkill_fight.size(); i < MaxFightSkill; i++)
	{
		skillSequence[i]->setIcon(QIcon(""));
		skillSequence[i]->setToolTip("");
	}
}

inline const Info_skill *role_skill::FindSkill(skillID id)
{
	foreach(const Info_skill &info, g_skillList)
	{
		if (info.ID == id)
		{
			return &info;
		}
	}
	return nullptr;
}