#include "error.h"

const int errorCount = 11;

info_error errrDefine[errorCount] =
{
	{ mirErr_Unknow, "未知错误" },
	{ mirErr_XSpeed, "检测到加速软件" },
	{ mirErr_Modify, "存档被修改" },
	{ mirErr_MD5, "MD5校验未通过" },
	{ mirErr_para, "参数错误" },
	{ mirErr_FileOpen, "打开文件失败" },
	{ mirErr_FileRead, "读取文件失败" },
	{ mirErr_FileWrite, "写入文件失败" },
	{ mirErr_UnknowEquipID, "未知装备ID" },
	{ mirErr_UnknowItemID, "未知道具ID" },
	{ mirErr_UnknowSkillID, "未知技能ID" }
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