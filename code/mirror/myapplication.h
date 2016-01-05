#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include <QApplication>
#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>
#include <qwidget.h>

class MyApplication : public QApplication
{
	Q_OBJECT

public:
	MyApplication(int argc, char **argv);
	~MyApplication();

	bool isRunning();
	QWidget *w;                        // MainWindow指针

private slots:
	void _newLocalConnection();

private:
	// 初始化本地连接
	void _initLocalConnection();
	// 创建服务端
	void _newLocalServer();
	// 激活窗口
	void _activateWindow();

private:
	QLocalServer *_localServer;
	QString _serverName;

	bool _isRunning;
	
};

#endif // MYAPPLICATION_H
