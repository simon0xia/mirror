#include "error.h"

const int errorCount = 4;

info_error errrDefine[errorCount] =
{
	{ mirErr_Unknow, "未知错误" },
	{ mirErr_XSpeed, "检测到加速软件" },
	{ mirErr_Modify, "存档被修改" },
	{ mirErr_MD5, "MD5校验未通过" }
};

std::string & error2str(int Ecode)
{
	for (int i = 0; i < errorCount; i++)
	{
		if (errrDefine[i].id == Ecode)
		{
			return errrDefine[i].msg;
		}
	}
	return errrDefine[0].msg;
}