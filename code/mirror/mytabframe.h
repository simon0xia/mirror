#ifndef MYTABFRAME_H
#define MYTABFRAME_H

#include <QWidget>
#include "RoleDefine.h"

class myTabFrame : public QWidget
{
	Q_OBJECT

public:
	myTabFrame(QWidget *parent = 0);
	~myTabFrame();
	virtual void updateRoleInfo(void) = 0;

private:
	
};

#endif // MYTABFRAME_H
