#ifndef _MIRRORVERSION_H_
#define _MIRRORVERSION_H_

#include <stdint.h>

//每段允许最大值为999
const int32_t version_major = 0,
	version_minor = 5,
	version_build = 6;

const qint32 SaveFileVer = 10;						//存档文件版本
const QString SaveFileName = "save.sxv";			//存档文件名字
const QString BackFileName = "save.backup";			//备份存档文件名


#endif //#ifndef _MIRRORVERSION_H_