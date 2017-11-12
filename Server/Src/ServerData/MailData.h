#ifndef __MAIL_DATA_OBJECT_H__
#define __MAIL_DATA_OBJECT_H__
#include "DBInterface/DBInterface.h"
#include "../ServerEngine/SharedMemory.h"
#include "ServerDefine.h"


struct GroupMailDataObject : public ShareObject
{
	GroupMailDataObject()
	{

	}
	UINT64 m_uGuid;							//邮件ID
	CHAR   m_szTitle[MAIL_TITLE_LEN];		//邮件标题
	CHAR   m_szContent[MAIL_CONTNET_LEN];	//邮件内容
	UINT64 m_dwTime;						//邮件时间
	UINT32 m_dwMailType;					//邮件类型
	UINT32 m_dwItem[MAIL_ITEM_COUNT];		//道具列表
	UINT32 m_nItemCnt[MAIL_ITEM_COUNT];		//道具个数
	UINT32 m_dwChannel;						//目标渠道

	BOOL Create(IDBInterface* pDB)
	{
		return TRUE;
	}

	BOOL Update(IDBInterface* pDB)
	{
		return TRUE;
	}

	BOOL Delete(IDBInterface* pDB)
	{
		return TRUE;
	}
};



struct MailDataObject : public ShareObject
{
	MailDataObject()
	{

	}

	UINT64 m_uRoleID;						//角色ID
	UINT64 m_uGuid;							//邮件ID
	UINT64 m_dwTime;						//邮件时间
	UINT64 m_uSenderID;						//发送者ID
	UINT32 m_dwItem[MAIL_ITEM_COUNT];		//道具列表
	UINT32 m_nItemCnt[MAIL_ITEM_COUNT];		//道具个数
	CHAR   m_szTitle[MAIL_TITLE_LEN];		//邮件标题
	CHAR   m_szContent[MAIL_CONTNET_LEN];	//邮件内容
	CHAR   m_szSender[ROLE_NAME_LEN];		//发送者名字

	BOOL Create(IDBInterface* pDB)
	{
		return TRUE;
	}

	BOOL Update(IDBInterface* pDB)
	{
		return TRUE;
	}

	BOOL Delete(IDBInterface* pDB)
	{
		return TRUE;
	}
};



#endif //__MAIL_DATA_OBJECT_H__