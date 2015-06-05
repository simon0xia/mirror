#ifndef _MIRROR_ERROR_H
#define _MIRROR_ERROR_H

#include <iostream>

#define mirErr_Unknow 0
#define mirErr_XSpeed -99

const int errorCount = 2;

struct info_error
{
	int id;
	std::string msg;
};

std::string & error2str(int Ecode);


#endif //#ifndef _MIRROR_ERROR_H