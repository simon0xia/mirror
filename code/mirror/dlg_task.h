#ifndef DLG_TASK_H
#define DLG_TASK_H

#include <QDialog>
#include "ui_dlg_task.h"
#include <QTreeWidgetItem>
#include "gamemanager.h"

class dlg_task : public QDialog
{
	Q_OBJECT

public:
	dlg_task(QWidget *parent = 0);
	~dlg_task();

	void initUI();

	void init_currentTask();

private slots:
	void itemClicked(QTreeWidgetItem * item, int column);
	void on_btn_accpet_clicked(void);
	void on_btn_refresh_clicked(void);

private:
	qint32 GeneralTaskInfo(qint32 id, QString &str);
	void GeneralTaskInfo_HoldRound(const task::taskItem& item, QString &str);
	void GeneralTaskInfo_KillMonster(const task::taskItem& item, QString &str);
	void GeneralTaskInfo_Item(qint32 tID, qint32 tCount, QString &str);
	
	qint32 GeneralReward(qint32 id, QString &str);

	void checkTaskStatus(qint32 id);

	bool wasComplete(const task::taskItem &item);

	void updateInfo(task::RewardType rType);

private:
	Ui::dlg_task ui;
};

#endif // DLG_TASK_H
