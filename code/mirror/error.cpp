#include "error.h"

info_error errrDefine[errorCount] =
{
	{ mirErr_Unknow, "未知错误" },
	{ mirErr_XSpeed, "检测到加速软件" }
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