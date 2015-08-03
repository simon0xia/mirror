#ifndef _MIRROR_ERROR_H
#define _MIRROR_ERROR_H

#include <iostream>

#define mirErr_Unknow 0
#define mirErr_XSpeed -1000
#define mirErr_Modify -1001
#define mirErr_MD5 -1002
#define mirErr_para -1003
#define mirErr_FileOpen -1004
#define mirErr_FileRead -1005
#define mirErr_FileWrite -1006
#define mirErr_UnknowEquipID -1007
#define mirErr_UnknowItemID -1008
#define mirErr_UnknowSkillID -1009

struct info_error
{
	int id;
	std::string msg;
};

std::string & error2str(int Ecode);


#endif //#ifndef _MIRROR_ERROR_H