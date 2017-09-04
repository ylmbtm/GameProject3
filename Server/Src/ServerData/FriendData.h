#ifndef __FRIEND_DATA_OBJECT_H__
#define __FRIEND_DATA_OBJECT_H__
#include "ServerStruct.h"

struct FriendDataObject : public ShareObject
{
	FriendDataObject()
	{

	}

	UINT64 m_uRoleID;		//角色ID
	UINT64 m_uFriendID;     //好友ID

	BOOL Save(IDataBase* pDB)
	{
		return TRUE;
	}

	BOOL Delete(IDataBase* pDB)
	{

		return TRUE;
	}
};

#endif //__FRIEND_DATA_OBJECT_H__