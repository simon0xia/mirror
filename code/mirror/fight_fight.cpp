#include "fight_fight.h"
#include <QFile>
#include <QMessageBox>
#include <QThread>

const int interval = 100;

fight_fight::fight_fight(QWidget* parent, qint32 id, RoleInfo *info)
: QDialog(parent), father(parent), mapID(id), myRole(info)
{
	ui.setupUi(this);

	ui.edit_display->setText(QString::number(mapID));

	Cacl_Display_Role_Value();

	monster_count = Boss_count = 0;
	bKeepFight = bFighting = false;
	monster_cur = &monsterArr[0];

	LoadDistribute();
	LoadMonster();
	LoadBoss();
	Load_Display_Monster_Value();

	ui.progressBar_monster_hp->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
	ui.progressBar_monster_mp->setStyleSheet("QProgressBar::chunk { background-color: rgb(0, 0, 255) }");
	ui.progressBar_monster_ap->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 255, 0) }");
}

fight_fight::~fight_fight()
{
	
}

void fight_fight::on_btn_quit_clicked(void)
{
	if (!bFighting)
	{
		this->close();
	}
	else
	{
		QString title = QString::fromLocal8Bit("��ʾ");
		QString message = QString::fromLocal8Bit("��ǰ����ս���У����ܽ���ʧ10%��������Ǯ��");
		if (QMessageBox::Yes == QMessageBox::question(this, title, message))
		{
			myRole->coin -= myRole->coin * 0.1;
			myRole->reputation -= myRole->reputation * 0.1;
			this->close();
		}
	}
}

void fight_fight::on_btn_start_clicked(void)
{
	time_remain = time_remain_role = time_remain_monster = 1000;
	nShowStatusRound = 5;

	//����һ���������ʾ������Ϣ��
	qint32 n = qrand() % monster_count;
	monster_cur = &monsterArr[n];
	Load_Display_Monster_Value();

	ui.edit_display->setText(QString::fromLocal8Bit("ս����ʼ"));

	nFightTimer = startTimer(interval);
	bFighting = true;
}

void fight_fight::Cacl_Display_Role_Value()
{
	ui.edit_role_name->setText(myRole->name);
	ui.edit_role_level->setText(QString::number(myRole->level));

	ui.progressBar_role_exp->setMaximum(myRole->lvExp);
	if (myRole->exp >= ui.progressBar_role_exp->maximum())
		ui.progressBar_role_exp->setValue(ui.progressBar_role_exp->maximum());
	else
		ui.progressBar_role_exp->setValue(myRole->exp);

	role_Speed = qMin(2.5, 1 + myRole->agility * 0.01);
	ui.edit_role_AttackSpeed->setText(QString::number(role_Speed));

	role_DC = myRole->level * 1.0 + myRole->strength * 1;
	ui.edit_role_PhysicsAttack->setText(QString::number(role_DC));

	role_MC = myRole->level * 0.9 + myRole->wisdom * 1;
	ui.edit_role_MagicAttack->setText(QString::number(role_MC));

	role_SC = myRole->level * 0.8 + myRole->spirit * 1;
	ui.edit_role_SpellAttack->setText(QString::number(role_SC));

	role_AC = 0;
	ui.edit_role_PhysicsGuard->setText(QString::number(role_AC));

	role_MAC = 0;
	ui.edit_role_ElementGuard->setText(QString::number(role_MAC));

	role_extrarate = 0;
	ui.edit_role_extrarate->setText(QString::number(role_extrarate));

	role_extrahurt = 0;
	ui.edit_role_extrahurt->setText(QString::number(role_extrahurt));

	ui.progressBar_role_hp->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
	ui.progressBar_role_hp->setMaximum(myRole->hp_m);
	ui.progressBar_role_hp->setValue(myRole->hp_m);
	role_hp_c = myRole->hp_m;

	ui.progressBar_role_mp->setStyleSheet("QProgressBar::chunk { background-color: rgb(0, 0, 255) }");
	ui.progressBar_role_mp->setMaximum(myRole->mp_m);
	ui.progressBar_role_mp->setValue(myRole->mp_m);
	role_mp_c = myRole->mp_m;

	ui.progressBar_role_ap->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 255, 0) }");
	ui.progressBar_role_ap->setMaximum(myRole->ap_m);
	ui.progressBar_role_ap->setValue(0);
	role_ap_c = 0;

	role_rhp = 0;
	ui.edit_role_rhp->setText(QString::number(role_rhp));

	role_rmp = 0;
	ui.edit_role_rmp->setText(QString::number(role_rmp));

	role_rap = 1;
	ui.edit_role_rap->setText(QString::number(role_rap));
}

void fight_fight::LoadDistribute()
{
	QString db_distribute = "distribute.db";
	QFile file(db_distribute);
	if (!file.open(QIODevice::ReadOnly))
	{
		QString message = QString::fromLocal8Bit("���ع���ֲ��б�ʧ�ܣ�������������Ϸ��");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);

		exit(0);
	}

	quint32 id;
	QDataStream out(file.readAll());
	while (!out.atEnd())
	{
		out >> id >> monster_id_start >> monster_id_stop >> boss_id_start >> boss_id_stop;
		if (id == mapID)
		{
			break;
		}
	}

	file.close();
}


void fight_fight::LoadMonster()
{
	QString db_monster = "monster";
	if (mapID < 30) {
		db_monster += "1";
	}
	else if (mapID < 70) {
		db_monster += "2";
	}
	else {
		db_monster += "3";
	}
	db_monster += ".db";

	QFile file(db_monster);
	if (!file.open(QIODevice::ReadOnly))
	{
		QString message = QString::fromLocal8Bit("����ʧ�ܣ�������������Ϸ��");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);

		exit(0);
	}

	MonsterInfo *mon;
	qint32 i = 0;
	QDataStream out(file.readAll());
	while (!out.atEnd() && i < Max_monster)
	{
		mon = &monsterArr[i];

		out >> mon->ID >> mon->name >> mon->Head >> mon->level >> mon->exp;
		out >> mon->hp_m >> mon->hp_r >> mon->mp_m >> mon->mp_r >> mon->ap_m >> mon->ap_r;
		out >> mon->DC >> mon->MC >> mon->SC >> mon->AC >> mon->MAC;
		out >> mon->extrarate >> mon->extrahurt >> mon->penetrate >> mon->Speed;

		//ֻ��id������Ч����Ĳż�¼�������Ա����ǡ�
		if (mon->ID > monster_id_stop)
		{
			break;
		}
		if (mon->ID >= monster_id_start)
		{
			++i;
			++monster_count;
		}
	}

	file.close();
}

void fight_fight::LoadBoss()
{
	QString db_boss = "boss.db";
	QFile file(db_boss);
	if (!file.open(QIODevice::ReadOnly))
	{
		QString message = QString::fromLocal8Bit("����ʧ�ܣ�������������Ϸ��");
		QMessageBox::critical(this, tr("QMessageBox::critical()"), message);

		exit(0);
	}

	MonsterInfo *boss;
	qint32 i = 0;
	QDataStream out(file.readAll());
	while (!out.atEnd() && i < Max_monster)
	{
		boss = &BossArr[i];

		out >> boss->name >> boss->ID >> boss->level >> boss->exp;
		out >> boss->hp_m >> boss->hp_r >> boss->mp_m >> boss->mp_r >> boss->ap_m >> boss->ap_r;
		out >> boss->DC >> boss->MC >> boss->SC >> boss->AC >> boss->MAC;
		out >> boss->extrarate >> boss->extrahurt >> boss->penetrate >> boss->Speed;

		//ֻ��id������Ч����Ĳż�¼�������Ա����ǡ�
		if (boss->ID > boss_id_start)
		{
			break;
		}
		if (boss->ID >= boss_id_stop)
		{
			++i;
			++Boss_count;
		}
	}

	file.close();
}

void fight_fight::Load_Display_Monster_Value()
{
	//�����塢ħ�������ֵ��
	ui.progressBar_monster_hp->setMaximum(monster_cur->hp_m);
	ui.progressBar_monster_mp->setMaximum(monster_cur->mp_m);
	ui.progressBar_monster_ap->setMaximum(monster_cur->ap_m);
	
	//�ظ��塢ħΪ���ֵ,��Ϊ0
	monster_cur->hp_c = monster_cur->hp_m;
	monster_cur->mp_c = monster_cur->mp_m;
	monster_cur->ap_c = 0;

	//��ʾ��ǰ�塢ħ����
	ui.progressBar_monster_hp->setValue(monster_cur->hp_c);
	ui.progressBar_monster_mp->setValue(monster_cur->mp_c);
	ui.progressBar_monster_ap->setValue(monster_cur->ap_c);

	//����ͷ��
	ui.label_monster_head->setPixmap(QPixmap::fromImage(monster_cur->Head));

	//������������
	ui.edit_monster_name->setText(monster_cur->name);
	ui.edit_monster_level->setText(QString::number(monster_cur->level));
	ui.edit_monster_PhysicsAttack->setText(QString::number(monster_cur->DC));
	ui.edit_monster_MagicAttack->setText(QString::number(monster_cur->MC));
	ui.edit_monster_SpellAttack->setText(QString::number(monster_cur->SC));
	ui.edit_monster_PhysicsGuard->setText(QString::number(monster_cur->AC));
	ui.edit_monster_ElementGuard->setText(QString::number(monster_cur->MAC));
	ui.edit_monster_rhp->setText(QString::number(monster_cur->hp_r));
	ui.edit_monster_rmp->setText(QString::number(monster_cur->mp_r));
	ui.edit_monster_rap->setText(QString::number(monster_cur->ap_r));
	ui.edit_monster_extrarate->setText(QString::number(monster_cur->extrarate));
	ui.edit_monster_extrahurt->setText(QString::number(monster_cur->extrahurt));
	ui.edit_monster_AttackSpeed->setText(QString::number(monster_cur->Speed));
	ui.edit_monster_penetrate->setText(QString::number(monster_cur->penetrate));
}

void fight_fight::Action_role(void)
{
	//���ٽ�ɫ��ʣ��ʱ�䡣
	time_remain_role -= 1 / role_Speed;

	//��ɫ����ǰ����һ�����˺�ֵ = (��ɫ������-����������������
	qint32 nTmp = role_DC - monster_cur->AC;
	monster_cur->hp_c -= nTmp;
	if (monster_cur->hp_c <= 0)
	{
		monster_cur->hp_c = 0;
	}
	ui.progressBar_monster_hp->setValue(monster_cur->hp_c);

	if (!ui.checkBox_concise->isChecked())
	{
		QString strTmp = QString::fromLocal8Bit("�㿳��") + monster_cur->name + QString::fromLocal8Bit("һ�������")
			+ QString::number(nTmp) + QString::fromLocal8Bit("���˺���");
		ui.edit_display->append(strTmp);
	}
	
	if (monster_cur->hp_c <= 0)
	{
		bFighting = false;

		//������������ɫ���Ӿ��顣
		myRole->exp += monster_cur->exp;

		if (myRole->exp >= ui.progressBar_role_exp->maximum())
			ui.progressBar_role_exp->setValue(ui.progressBar_role_exp->maximum());
		else
			ui.progressBar_role_exp->setValue(myRole->exp);

		ui.edit_display->append(QString::fromLocal8Bit("ս��ʤ��!"));
		ui.edit_display->append(QString::fromLocal8Bit("��þ���:") + QString::number(monster_cur->exp));
	}
}
void fight_fight::Action_monster(void)
{
	//���ٹ����ʣ��ʱ�䡣
	time_remain_monster -= 1 / monster_cur->Speed;

	//���￳��ɫһ�����˺�ֵ = (��������������-��ɫ������������
	qint32 nTmp = monster_cur->DC - role_AC;
	role_hp_c -= nTmp;
	if (role_hp_c <= 0)
	{
		role_hp_c = 0;
	}
	ui.progressBar_role_hp->setValue(role_hp_c);

	//�ǡ����ģʽ������ʾ�˺���Ϣ��
	if (!ui.checkBox_concise->isChecked())
	{
		QString strTmp = monster_cur->name + QString::fromLocal8Bit("������һ�������")
			+ QString::number(nTmp) + QString::fromLocal8Bit("���˺���");
		ui.edit_display->append(strTmp);
	}

	if (role_hp_c <= 0)
	{
		//����ս��״̬Ϊ��ս�������ҽ�ɫ�����󲻿��ٴ�ս����
		bFighting = false;
		ui.btn_start->setEnabled(false);

		//��ɫ��������ʧ���顢��������Ҹ�10%
		quint32 nExp = myRole->exp * 0.1;
		quint32 nCoin = myRole->coin * 0.1;
		quint32 nRep = myRole->reputation * 0.1;
		myRole->exp -= nExp;
		myRole->coin -= nCoin;
		myRole->reputation -= nRep;

		ui.progressBar_role_exp->setValue(myRole->exp);
		ui.edit_display->append(QString::fromLocal8Bit("ս��ʧ��!"));
		ui.edit_display->append(QString::fromLocal8Bit("��ʧ���飺") + QString::number(nExp));
		ui.edit_display->append(QString::fromLocal8Bit("��ʧ��ң�") + QString::number(nCoin));
		ui.edit_display->append(QString::fromLocal8Bit("��ʧ������") + QString::number(nRep));
	}
}

void fight_fight::timerEvent(QTimerEvent *event)
{
	//ÿһ��timerEventΪһ���غϡ� 
	//��ǰδ����ս��״̬������ʱ��ʾ��һ�ε�ս����Ϣ��ͬʱ����ʱ������ֹͣ��ʱ����������Ƿ�ѡ���Զ�ս�������������ٴ�����ս����
	if (bFighting == false)
	{
		--nShowStatusRound;
		if (nShowStatusRound <= 0)
		{
			killTimer(nFightTimer);
			if (ui.checkBox_auto->isChecked())
			{
				on_btn_start_clicked();
			}
		}	
		return;
	}

	//�غ�ʱ�������꣬�ж�ս����ʱ����ֹͣ����ս���������Զ�ս����
	if (time_remain <= 0)
	{
		ui.edit_display->append(QString::fromLocal8Bit("ս����ʱ��"));
		killTimer(nFightTimer);
		ui.checkBox_auto->setCheckState(Qt::Unchecked);
		return;
	}
	
	//���غ�ʱ��С�ڽ�ɫʱ�䣬���ɫ�һ�غϡ����жϣ����غ�ʱ��С�ڹ���ʱ�䣬�����һ�غϡ�
	if (time_remain < time_remain_role)
	{
		Action_role();
	}
	else if (time_remain < time_remain_monster)
	{
		Action_monster();
	}
	
	//����һ�غ�ʱ�䡣(ͳһ��λ����)
	time_remain -= interval / 1000.0;
}