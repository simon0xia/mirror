#include "role_skill.h"

extern QVector<Info_skill> g_skillList;

role_skill::role_skill(QWidget *parent, const VecRoleSkill *skill_study, VecRoleSkill *skill_fight)
	: QDialog(parent)
	, m_skill_study(skill_study), m_skill_fight(skill_fight)
{
	ui.setupUi(this);
	ui.btn_close->setVisible(false);
	ui.btn_pgDn->setVisible(false);
	ui.btn_pgUp->setVisible(false);

	for (VecRoleSkill::const_iterator iter = skill_study->begin(); iter != skill_study->end(); iter++)
	{
		const Info_skill *info = FindSkill(iter->id);
		if (info != nullptr)
		{
			ui.listWidget->addItem(new QListWidgetItem(info->icon1, info->name + QStringLiteral("\t等级:") + QString::number(iter->level)));
		}
	}

	skillSequence.append(ui.btn_skill_1);
	skillSequence.append(ui.btn_skill_2);
	skillSequence.append(ui.btn_skill_3);
	skillSequence.append(ui.btn_skill_4);
	skillSequence.append(ui.btn_skill_5);
	DisplaySkillSequence();

	connect(ui.listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(SetSkillInFighting(QListWidgetItem *)));
	
}

role_skill::~role_skill()
{

}

void role_skill::closeEvent(QCloseEvent *event)
{
	on_btn_close_clicked();
}

void role_skill::on_btn_close_clicked(void)
{
	//将普通攻击也视为技能，所以需要保留至少一个技能，否则战斗时会看着怪物发呆。
	if (m_skill_fight->size() <= 0)
	{
		roleSkill skill;
		skill.id = g_skillList.at(0).ID;
		skill.level = 1;
		m_skill_fight->append(skill);
	}
	
	done(QDialog::Rejected);
}
void role_skill::on_btn_skill_1_clicked(void)
{
	if (m_skill_fight->size() > 0)
	{
		m_skill_fight->remove(0);
		DisplaySkillSequence();
	}
}
void role_skill::on_btn_skill_2_clicked(void)
{
	if (m_skill_fight->size() > 1)
	{
		m_skill_fight->remove(1);
		DisplaySkillSequence();
	}
}
void role_skill::on_btn_skill_3_clicked(void)
{
	if (m_skill_fight->size() > 2)
	{
		m_skill_fight->remove(2);
		DisplaySkillSequence();
	}
}
void role_skill::on_btn_skill_4_clicked(void)
{
	if (m_skill_fight->size() > 3)
	{
		m_skill_fight->remove(3);
		DisplaySkillSequence();
	}
}
void role_skill::on_btn_skill_5_clicked(void)
{
	if (m_skill_fight->size() > 4)
	{
		m_skill_fight->remove(4);
		DisplaySkillSequence();
	}
}

void role_skill::SetSkillInFighting(QListWidgetItem * item)
{
	if (m_skill_fight->size() < MaxFightSkill)
	{
		qint32 index = ui.listWidget->currentRow();
		const roleSkill &skill_study = m_skill_study->at(index);
		for (VecRoleSkill::const_iterator iter = m_skill_fight->begin(); iter != m_skill_fight->end(); iter++)
		{
			if (iter->id == skill_study.id)
			{
				//当前战斗技能列表中已有此技能，不允许重复添加。
				return;
			}
		}
		m_skill_fight->append(skill_study);
		DisplaySkillSequence();
	}
	else
	{
		//nothing
	}
}

void role_skill::DisplaySkillSequence(void)
{
	for (qint32 i = 0; i < MaxFightSkill && i < m_skill_fight->size(); i++)
	{
		const Info_skill *info = FindSkill(m_skill_fight->at(i).id);
		if (info != nullptr)
		{
			skillSequence[i]->setIcon(info->icon1);
		}
	}
	for (qint32 i = m_skill_fight->size(); i < MaxFightSkill; i++)
	{
		skillSequence[i]->setIcon(QIcon(":/mirror/Resources/bg02_1.png"));
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