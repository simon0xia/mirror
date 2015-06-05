#ifndef __MIRRORLOG_H__
#define __MIRRORLOG_H__

#include <iostream>
#include <mutex>
#include <fstream>
#include <sstream>
#include <ctime>

#include "error.h"

//日志级别，共4种，分别为fatal, error, warning, info
#define LEVEL_FATAL		3
#define LEVEL_ERROR		2
#define LEVEL_WARNING	1
#define LEVEL_INFO		0

//获取时间变量，到天，为GET_TIME_OF_DAT,到秒为TIME获取昨天为GET_TIME_OF_YESTERDAY
#define GET_TIME_OF_YESTERDAY	2
#define GET_TIME_OF_DAY			1
#define TIME					0

class MirrorLog
{
public:
	static class MirrorLog& get_logClass()
	{
		static MirrorLog mirLog;   //局部静态变量  
		return mirLog;
	}

	void init(int _logLevel, std::string _logFileName = "mirror.log");
	void append(int _logLevel, std::string FunctionName, int errorcode);

private:
	MirrorLog() { ; }	//构造函数私有
	MirrorLog(const MirrorLog &);
	MirrorLog & operator = (const MirrorLog &);

	void delete_oldLog();
	std::string get_time(int flag = TIME);

	std::string m_logfilename;
	int m_loglevel;
	std::mutex lock;
};

#define LogIns MirrorLog::get_logClass()

#endif //__MIRRORLOG_H__
