#ifndef __ACTIVITY_MANAGER_H__
#define __ACTIVITY_MANAGER_H__


class CActivityManager
{
	CActivityManager();
	~CActivityManager();
public:
	static CActivityManager* GetInstancePtr();
public:

	BOOL Init();

	BOOL ActivityTimer(UINT32 dwData);


};
#endif