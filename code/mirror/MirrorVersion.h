#ifndef _MIRRORVERSION_H_
#define _MIRRORVERSION_H_

#include <stdint.h>

//每段允许最大值为999
const int32_t version_major = 1,
	version_minor = 2,
	version_build = 2;

//const qint32 SaveFileVer = 99;						//测试
const qint32 SaveFileVer = 15;					//存档文件版本
const QString SaveFileName = "save.sxv";			//存档文件名字
const QString BackFileName = "save.backup";			//备份存档文件名
const QString RecoveryFileName = "save.recovery";	//待恢复的存档
const QString SettingFileName = "./setting.ini";	//配置文档的路径。

#endif //#ifndef _MIRRORVERSION_H_