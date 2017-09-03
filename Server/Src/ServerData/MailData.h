#ifndef __MAIL_DATA_OBJECT_H__
#define __MAIL_DATA_OBJECT_H__
#include "ServerStruct.h"
#include "../ServerEngine/SharedMemory.h"


struct GroupMailDataObject : public ShareObject
{
	GroupMailDataObject()
	{

	}
	UINT64 m_uGuid;         //邮件ID
	CHAR   m_szParam[256];  //邮件参数
	UINT32 m_dwTime;        //邮件时间
	UINT32 m_dwMailType;    //邮件类型
	UINT32 m_dwItem[10];    //道具列表
	UINT32 m_nItemCnt[10];  //道具个数

	BOOL Save(IDataBase* pDB)
	{


		return TRUE;
	}

	BOOL Delete(IDataBase* pDB)
	{


		return TRUE;
	}
};



struct MailDataObject : public ShareObject
{
	MailDataObject()
	{

	}

	UINT64 m_uRoleID;		//角色ID
	UINT64 m_uGuid;         //邮件ID
	CHAR   m_szParam[256];  //邮件参数
	UINT32 m_dwTime;        //邮件时间
	UINT32 m_dwMailType;    //邮件类型
	UINT32 m_dwItem[10];    //道具列表
	UINT32 m_nItemCnt[10];  //道具个数

	BOOL Save(IDataBase* pDB)
	{


		return TRUE;
	}

	BOOL Delete(IDataBase* pDB)
	{


		return TRUE;
	}
};



#endif //__MAIL_DATA_OBJECT_H__