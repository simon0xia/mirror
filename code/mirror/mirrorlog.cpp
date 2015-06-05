#include "mirrorlog.h"

void MirrorLog::init(int _logLevel, std::string _logFileName)
{
	m_loglevel = _logLevel;
	m_logfilename = _logFileName;
	delete_oldLog();
}

void MirrorLog::append(int _logLevel, std::string FunctionName, int errorcode)
{	
	//写日志先判断运行级别
	if (_logLevel < m_loglevel)
	{
		return;
	}

	lock.lock();
	std::fstream out(m_logfilename.c_str(), std::ios::out | std::ios::app);
	out << get_time(TIME);
	out << std::string(" ") << _logLevel << std::string(" ") << FunctionName << std::string("\t");
	out << errorcode << std::string("\t") << error2str(errorcode) << std::endl;

	out.close();
	lock.unlock();
}

void MirrorLog::delete_oldLog()
{
	std::ifstream in(m_logfilename.c_str()); // 打开日志文件
	std::stringstream ss;
	std::string s, sTmp;//存放日志文件读出的内容和删除前一天之前的日志后的内容
	std::size_t w;//定位前一天的日志位置

	ss << in.rdbuf(); // 将文件整个buffer放到stringstream中
	s = ss.str(); // 把ss整个作为一个string放入s，文件所有内容就在s中

	in.close();

	// ... 操作s
	if (!s.size())
		return;

	w = s.find(get_time(GET_TIME_OF_DAY));
	if (w == std::string::npos)
	{
		w = s.find(get_time(GET_TIME_OF_YESTERDAY));
		if (w == std::string::npos)
		{
			sTmp = "";
		}
		else
		{
			sTmp = s.substr(w);
		}
	}
	else
	{
		sTmp = s.substr(w);
	}

	// 把s重新写回文件
	std::ofstream out(m_logfilename.c_str());
	out << sTmp;
	out.close();
}

std::string MirrorLog::get_time(int flag)
{
	char tmp[64];
	tm tm;
	
	if (flag == TIME)
	{
		time_t t = time(0);
		localtime_s(&tm, &t);
		strftime(tmp, sizeof(tmp), "%m-%d %H:%M:%S", &tm);
	}
	else if (flag == GET_TIME_OF_DAY)
	{
		time_t t = time(0);
		localtime_s(&tm, &t);
		strftime(tmp, sizeof(tmp), "%m-%d", &tm);
	}
	else if (flag == GET_TIME_OF_YESTERDAY)
	{
		time_t t = time(0) - 24 * 3600;
		localtime_s(&tm, &t);
		strftime(tmp, sizeof(tmp), "%m-%d", &tm);
	}
	else
	{
		time_t t = time(0);
		localtime_s(&tm, &t);
		strftime(tmp, sizeof(tmp), "%m-%d %H:%M:%S", &tm);
	}

	return std::string(tmp);
}


