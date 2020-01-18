#ifndef __ACTIVITY_MANAGER_H__
#define __ACTIVITY_MANAGER_H__

#include "DBInterface/CppMysql.h"

class CActivityManager
{
	CActivityManager();
	~CActivityManager();
public:
	static CActivityManager* GetInstancePtr();

public:
	BOOL Init();

	BOOL LoadData(CppMySQL3DB& tDBConnection);

	BOOL OnSecondTimer();




};
#endif