#ifndef __MOUNT_DATA_OBJECT_H__
#define __MOUNT_DATA_OBJECT_H__
#include "ServerStruct.h"
#include "SharedMemory.h"
struct MountDataObject : public ShareObject
{
	MountDataObject ()
	{

	}

	UINT64 m_uRoleID;			//角色ID
	UINT64 m_uGuid;			//guid
	UINT32 m_MountID;			//坐骑ID
	INT32  m_StrengthLvl;   //强化等级
	INT32  m_RefineLevel;   //精炼等级
	INT32  m_StarLevel;		//星级
	INT32  m_RefineExp;		//精验的经验
	INT32  m_StarExp;		//星级经验
	BOOL   m_IsUsing;       //当前使用中

	BOOL Save(IDataBase* pDB)
	{
		return TRUE;
	}

	BOOL Delete(IDataBase* pDB)
	{
		return TRUE;
	}
};


#endif //__MOUNT_DATA_OBJECT_H__