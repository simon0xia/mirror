#include "dlg_sign.h"
#include "gamemanager.h"
#include "Player.h"

dlg_sign::dlg_sign(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	trys = 0;
	udpsocket = nullptr;

	ui.btn_retry->setVisible(false);

	sl.append("1.cn.pool.ntp.org");
	sl.append("210.72.145.44");
	sl.append("ntp.sjtu.edu.cn");
	sl.append("s2c.time.edu.cn");
	sl.append("s2d.time.edu.cn");
	sl.append("s2e.time.edu.cn");
	sl.append("s2f.time.edu.cn");
	sl.append("s2g.time.edu.cn");
	sl.append("time.nist.gov");
	sl.append("time.windows.com");

	qint32 keepSign = GameMgrIns.get_keepSign();
	ui.lbl_total->setText(QStringLiteral("连续签到天数：%1").arg(keepSign));

	QString strTmp;
	strTmp = QStringLiteral("1、重置日常任务次数。");
	strTmp += QStringLiteral("\n2、金币%1万").arg(keepSign);
	ui.lbl_info->setText(strTmp);

	QString strF("yyyy-MM-dd HH:mm:ss");
	QDateTime dt;
	dt.setTime_t(GameMgrIns.get_preSignTime());
	strTmp = QStringLiteral("上次签到时间:%1").arg(dt.toString(strF));
	ui.lbl_status->setText(strTmp);
}

dlg_sign::~dlg_sign()
{

}

void dlg_sign::connectsucess()
{
	ui.lbl_status->append(QStringLiteral("已连接，正在获取网络时间"));

	qint8 LI = 0;
	qint8 VN = 3;
	qint8 MODE = 3;
	qint8 STRATUM = 0;
	qint8 POLL = 4;
	qint8 PREC = -6;
	QDateTime Epoch(QDate(1900, 1, 1));
	qint32 second = quint32(Epoch.secsTo(QDateTime::currentDateTime()));
	qint32 temp = 0;
	QByteArray timeRequest(48, 0);
	timeRequest[0] = (LI << 6) | (VN << 3) | (MODE);
	timeRequest[1] = STRATUM;
	timeRequest[2] = POLL;
	timeRequest[3] = PREC & 0xff;
	timeRequest[5] = 1;
	timeRequest[9] = 1;
	timeRequest[40] = (temp = (second & 0xff000000) >> 24);
	temp = 0;
	timeRequest[41] = (temp = (second & 0x00ff0000) >> 16);
	temp = 0;
	timeRequest[42] = (temp = (second & 0x0000ff00) >> 8);
	temp = 0;
	timeRequest[43] = ((second & 0x000000ff));
	udpsocket->flush();
	udpsocket->write(timeRequest);
	udpsocket->flush();
}

void dlg_sign::readingDataGrams()
{
	killTimer(retryTimer);
	ui.lbl_status->append(QStringLiteral("正在读取网络时间..."));
	QByteArray newTime;
	QDateTime Epoch(QDate(1900, 1, 1));
	QDateTime unixStart(QDate(1970, 1, 1));
	do
	{
		newTime.resize(udpsocket->pendingDatagramSize());
		udpsocket->read(newTime.data(), newTime.size());
	} while (udpsocket->hasPendingDatagrams());
	QByteArray TransmitTimeStamp;
	TransmitTimeStamp = newTime.right(8);
	quint32 seconds = TransmitTimeStamp[0];
	quint8 temp = 0;
	for (int j = 1; j <= 3; j++)
	{
		seconds = seconds << 8;
		temp = TransmitTimeStamp[j];
		seconds = seconds + temp;
	}

	QDateTime time;
	time.setTime_t(seconds - Epoch.secsTo(unixStart));
	this->udpsocket->disconnectFromHost();
	this->udpsocket->close();
	Sign(time);
}

void dlg_sign::on_btn_ok_clicked()
{
	ui.btn_ok->setEnabled(false);
	retryTimer = startTimer(600);
	TryConnect();
	return;
}

void dlg_sign::TryConnect()
{
	if (trys >= sl.count())
	{
		ui.lbl_status->append(QStringLiteral("所有服务器皆已尝试"));
		return;
	}

	delete udpsocket;
	udpsocket = new QUdpSocket(this);
	if (udpsocket == nullptr)
	{
		ui.lbl_status->append(QStringLiteral("初始化网络失败"));
		return;
	}
	connect(udpsocket, SIGNAL(connected()), this, SLOT(connectsucess()));
	connect(udpsocket, SIGNAL(readyRead()), this, SLOT(readingDataGrams()));
	connect(udpsocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError()));

	udpsocket->connectToHost(sl.at(trys), 123);

	ui.lbl_status->append(QStringLiteral("尝试连接时间服务器:%1").arg(sl.at(trys)));
	trys++;
}

void dlg_sign::socketError()
{
	QString string = udpsocket->errorString();
	ui.lbl_status->append(string);
}

void dlg_sign::Sign(QDateTime curTime)
{
	QDateTime pre;
	pre.setTime_t(GameMgrIns.get_preSignTime());

	//记录本次签到时间
	GameMgrIns.set_SignTime(curTime.toTime_t());

	qint64 nTmp = pre.daysTo(curTime);

	if (nTmp > 1){
		//上次签到已经是一天以前
		GameMgrIns.reset_keepSign();
		GameMgrIns.reset_DaysTaskCount();
		ui.lbl_total->setText(QStringLiteral("连续签到天数：%1").arg(GameMgrIns.get_keepSign()));
		ui.lbl_status->append(QStringLiteral("签到成功"));
	}
	else if (nTmp == 1)
	{
		//上次签到是昨天
		PlayerIns.add_coin(GameMgrIns.get_keepSign() * 10000);
		GameMgrIns.add_keepSign();
		GameMgrIns.reset_DaysTaskCount();
		ui.lbl_total->setText(QStringLiteral("连续签到天数：%1").arg(GameMgrIns.get_keepSign()));
		ui.lbl_status->append(QStringLiteral("签到成功"));
	}else if (nTmp == 0) {
		//上次签到是今天
		ui.lbl_status->append(QStringLiteral("你今天已经签到了。"));
	} else {
		//上次签到是明天！！！
		ui.lbl_status->append(QStringLiteral("您好像穿越了。"));
	}
}

void dlg_sign::timerEvent(QTimerEvent *event)
{
	delete this->udpsocket;
	udpsocket = nullptr;

	TryConnect();
}