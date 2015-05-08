#include "role_skill.h"

extern QVector<Info_skill> g_skillList;

role_skill::role_skill(QWidget *parent, const VecRoleSkill *skill_study, VecRoleSkill *skill_fight)
	: QDialog(parent)
	, m_skill_study(skill_study), m_skill_fight(skill_fight)
{
	ui.setupUi(this);
	ui.btn_pgDn->setVisible(false);
	ui.btn_pgUp->setVisible(false);

	QString strTmp;
	for (VecRoleSkill::const_iterator iter = skill_study->begin(); iter != skill_study->end(); iter++)
	{
		const Info_skill *info = FindSkill(iter->id);
		if (info != nullptr)
		{
			strTmp = info->name + QStringLiteral("\t等级:") + QString::number(iter->level) + QStringLiteral("\tCD:") + QString::number(info->cd);
			ui.listWidget->addItem(new QListWidgetItem(info->icon1, strTmp));
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

void role_skill::on_btn_ok_clicked(void)
{
	if (tmpSkill_fight.size() < MaxFightSkill)
	{
		ui.lbl_display->setText(QStringLiteral("还有技能格未配置。"));
	}
	else
	{
		m_skill_fight->clear();
		for (VecRoleSkill::const_iterator iter = tmpSkill_fight.begin(); iter != tmpSkill_fight.end(); iter++)
		{
			m_skill_fight->append(*iter);
		}
		done(QDialog::Accepted);
	}
}
void role_skill::on_btn_close_clicked(void)
{
	//将普通攻击也视为技能，所以需要保留至少一个技能，否则战斗时会看着怪物发呆。
// 	if (tmpSkill_fight.size() <= 0)
// 	{
// 		roleSkill skill;
// 		skill.id = g_skillList.at(0).ID;
// 		skill.level = 1;
// 		tmpSkill_fight.append(skill);
// 	}
	
	done(QDialog::Rejected);
}
void role_skill::on_btn_skill_1_clicked(void)
{
	for (qint32 i = 0; i <= 5 - 1; i++)
	{
		if (!tmpSkill_fight.isEmpty())
		{
			tmpSkill_fight.removeLast();
		}
	}
	DisplaySkillSequence();
}
void role_skill::on_btn_skill_2_clicked(void)
{
	for (qint32 i = 0; i <= 5 - 2; i++)
	{
		if (!tmpSkill_fight.isEmpty())
		{
			tmpSkill_fight.removeLast();
		}
	}
	DisplaySkillSequence();
}
void role_skill::on_btn_skill_3_clicked(void)
{
	for (qint32 i = 0; i <= 5 - 3; i++)
	{
		if (!tmpSkill_fight.isEmpty())
		{
			tmpSkill_fight.removeLast();
		}
	}
	DisplaySkillSequence();
}
void role_skill::on_btn_skill_4_clicked(void)
{
	for (qint32 i = 0; i <= 5 - 4; i++)
	{
		if (!tmpSkill_fight.isEmpty())
		{
			tmpSkill_fight.removeLast();
		}
	}	
	DisplaySkillSequence();
}
void role_skill::on_btn_skill_5_clicked(void)
{
	for (qint32 i = 0; i <= 5 - 5; i++)
	{
		if (!tmpSkill_fight.isEmpty())
		{
			tmpSkill_fight.removeLast();
		}
	}
	DisplaySkillSequence();
}

void role_skill::SetSkillInFighting(QListWidgetItem * item)
{
	qint32 nTmp, nIndex;
	bool bRepeat = false;

	if (tmpSkill_fight.size() < MaxFightSkill)
	{
		qint32 index = ui.listWidget->currentRow();
		const roleSkill &skill_study = m_skill_study->at(index);
		for (quint32 i = 0; i < tmpSkill_fight.size(); i++)
		{
			if (tmpSkill_fight[i].id == skill_study.id)
			{
				//获得重复技能最后一次出现的位置。
				nIndex = i;
				bRepeat = true;
			}
		}
		if (bRepeat)
		{
			const Info_skill *skill = FindSkill(skill_study.id);

			//获取重复技能出现位置与当前欲放置位置之差，即可知中间的CD回合数。
			nTmp = skill->cd - (tmpSkill_fight.size() - nIndex);
		}
		else
		{
			nTmp = 0;
		}
		
		if (nTmp <= 0)
		{
			tmpSkill_fight.append(skill_study);

			ui.lbl_display->setText(QStringLiteral(""));
			DisplaySkillSequence();
		}
		else
		{
			ui.lbl_display->setText(QStringLiteral("CD不匹配！"));
		}
	}
	else
	{
		ui.lbl_display->setText(QStringLiteral("可配置技能已达上限！"));
	}
}

void role_skill::DisplaySkillSequence(void)
{
	for (qint32 i = 0; i < MaxFightSkill && i < tmpSkill_fight.size(); i++)
	{
		const Info_skill *info = FindSkill(tmpSkill_fight.at(i).id);
		if (info != nullptr)
		{
			skillSequence[i]->setIcon(info->icon1);
		}
	}
	for (qint32 i = tmpSkill_fight.size(); i < MaxFightSkill; i++)
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