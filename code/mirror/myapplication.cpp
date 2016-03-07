#include "myapplication.h"

#define  TIME_OUT 500

MyApplication::MyApplication(int argc, char **argv)
	:QApplication(argc, argv)
	, w(nullptr), _isRunning(false), _localServer(nullptr)
{
	_serverName = "277060BA-18B7-4923-89C4-F50BAC6ABC91";
	_initLocalConnection();
}

MyApplication::~MyApplication()
{
	delete _localServer;
}

bool MyApplication::isRunning()
{
	return _isRunning;
}

////////////////////////////////////////////////////////////////////////////////
// 说明：
// 通过socket通讯实现程序单实例运行，监听到新的连接时触发该函数
////////////////////////////////////////////////////////////////////////////////
void MyApplication::_newLocalConnection() {
	QLocalSocket *socket = _localServer->nextPendingConnection();
	if (socket) {
		socket->waitForReadyRead(2 * TIME_OUT);
		delete socket;

		// 其他处理，如：读取启动参数
		//不需要

		_activateWindow();
	}
}

////////////////////////////////////////////////////////////////////////////////
// 说明：
// 通过socket通讯实现程序单实例运行，
// 初始化本地连接，如果连接不上server，则创建，否则退出
////////////////////////////////////////////////////////////////////////////////
void MyApplication::_initLocalConnection() {
	_isRunning = false;

	QLocalSocket socket;
	socket.connectToServer(_serverName);
	if (socket.waitForConnected(TIME_OUT)) {
		fprintf(stderr, "%s already running.\n",
			_serverName.toLocal8Bit().constData());
		_isRunning = true;
		// 其他处理，如：将启动参数发送到服务端
		return;
	}

	//连接不上服务器，就创建一个
	_newLocalServer();
}

////////////////////////////////////////////////////////////////////////////////
// 说明：
// 创建LocalServer
////////////////////////////////////////////////////////////////////////////////
void MyApplication::_newLocalServer() {
	_localServer = new QLocalServer(this);
	connect(_localServer, SIGNAL(newConnection()), this, SLOT(_newLocalConnection()));
	if (!_localServer->listen(_serverName)) {
		// 此时监听失败，可能是程序崩溃时,残留进程服务导致的,移除之
		if (_localServer->serverError() == QAbstractSocket::AddressInUseError) {
			QLocalServer::removeServer(_serverName); // <-- 重点
			_localServer->listen(_serverName); // 再次监听
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// 说明：
// 激活主窗口
////////////////////////////////////////////////////////////////////////////////
void MyApplication::_activateWindow() {
	if (w) {
		w->show();
		w->raise();
		w->activateWindow(); // 激活窗口
	}
}