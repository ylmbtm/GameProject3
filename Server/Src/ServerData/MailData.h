#ifndef __MAIL_DATA_OBJECT_H__
#define __MAIL_DATA_OBJECT_H__
#include "ServerDefine.h"
#include "SharedMemory.h"
#include "DBInterface/DBStoredProc.h"
#include "DBInterface/DBInterface.h"

struct GroupMailDataObject : public ShareObject
{
	GroupMailDataObject()
	{
		m_uGuid = 0;
		m_uTime = 0;
		m_dwMailType = 0;
		m_dwChannel = 0;
		memset(m_szTitle, 0, sizeof(CHAR) * MAIL_TITLE_LEN);
		memset(m_szContent, 0, sizeof(CHAR) * MAIL_CONTNET_LEN);
		memset(m_dwItemID, 0, sizeof(UINT32) * MAIL_ITEM_COUNT);
		memset(m_nItemCnt, 0, sizeof(UINT32) * MAIL_ITEM_COUNT);
	}
	UINT64 m_uGuid;							//邮件ID
	CHAR   m_szTitle[MAIL_TITLE_LEN];		//邮件标题
	CHAR   m_szContent[MAIL_CONTNET_LEN];	//邮件内容
	UINT64 m_uTime;							//邮件时间
	UINT32 m_dwMailType;					//邮件类型
	UINT32 m_dwChannel;						//目标渠道
	UINT32 m_dwItemID[MAIL_ITEM_COUNT];		//道具列表
	UINT32 m_nItemCnt[MAIL_ITEM_COUNT];		//道具个数

	BOOL Create(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO groupmail (id, title, content, mail_time, mail_type, channel, itemid, itemcnt) \
			VALUES(?,?,?,?,?,?,?,?);");
		csp.set_uint64(0, m_uGuid);
		csp.set_string(1, m_szTitle, strlen(m_szTitle));
		csp.set_string(2, m_szContent, strlen(m_szContent));
		csp.set_uint64(3, m_uTime);
		csp.set_uint32(4, m_dwMailType);
		csp.set_uint32(5, m_dwChannel);
		csp.set_tinyblob(6, m_dwItemID, sizeof(UINT32)*MAIL_ITEM_COUNT);
		csp.set_tinyblob(7, m_nItemCnt, sizeof(UINT32)*MAIL_ITEM_COUNT);
		pDB->Execute(&csp);
		return TRUE;
	}

	BOOL Update(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO groupmail (id, title, content, mail_time, mail_type, channel, itemid, itemcnt) \
			VALUES(?,?,?,?,?,?,?,?);");
		csp.set_uint64(0, m_uGuid);
		csp.set_string(1, m_szTitle, strlen(m_szTitle));
		csp.set_string(2, m_szContent, strlen(m_szContent));
		csp.set_uint64(3, m_uTime);
		csp.set_uint32(4, m_dwMailType);
		csp.set_uint32(5, m_dwChannel);
		csp.set_tinyblob(6, m_dwItemID, sizeof(UINT32)*MAIL_ITEM_COUNT);
		csp.set_tinyblob(7, m_nItemCnt, sizeof(UINT32)*MAIL_ITEM_COUNT);
		pDB->Execute(&csp);
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
		m_uRoleID = 0;
		m_uGuid = 0;
		m_uTime = 0;
		m_uSenderID = 0;
		memset(m_szSender, 0, sizeof(CHAR) * ROLE_NAME_LEN);
		memset(m_szTitle, 0, sizeof(CHAR) * MAIL_TITLE_LEN);
		memset(m_szContent, 0, sizeof(CHAR) * MAIL_CONTNET_LEN);
		memset(m_dwItemID, 0, sizeof(UINT32) * MAIL_ITEM_COUNT);
		memset(m_nItemCnt, 0, sizeof(UINT32) * MAIL_ITEM_COUNT);
	}

	UINT64 m_uRoleID;						//角色ID
	UINT64 m_uGuid;							//邮件ID
	UINT64 m_uTime;							//邮件时间
	UINT64 m_uSenderID;						//发送者ID
	UINT32 m_dwItemID[MAIL_ITEM_COUNT];		//道具列表
	UINT32 m_nItemCnt[MAIL_ITEM_COUNT];		//道具个数
	CHAR   m_szTitle[MAIL_TITLE_LEN];		//邮件标题
	CHAR   m_szContent[MAIL_CONTNET_LEN];	//邮件内容
	CHAR   m_szSender[ROLE_NAME_LEN];		//发送者名字

	BOOL Create(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO mail (id, roleid, senderid, sendername, title, content, mail_time, itemid, itemcnt) \
			VALUES(?,?,?,?,?,?,?,?,?);");
		csp.set_uint64(0, m_uGuid);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint64(2, m_uSenderID);
		csp.set_string(3, m_szSender, strlen(m_szSender));
		csp.set_string(4, m_szTitle, strlen(m_szTitle));
		csp.set_string(5, m_szContent, strlen(m_szContent));
		csp.set_uint64(6, m_uTime);
		csp.set_tinyblob(7, m_dwItemID, sizeof(UINT32)*MAIL_ITEM_COUNT);
		csp.set_tinyblob(8, m_nItemCnt, sizeof(UINT32)*MAIL_ITEM_COUNT);
		pDB->Execute(&csp);
		return TRUE;
	}

	BOOL Update(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO mail (id, roleid, senderid, sendername, title, content, mail_time, itemid, itemcnt) \
			VALUES(?,?,?,?,?,?,?,?,?);");
		csp.set_uint64(0, m_uGuid);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint64(2, m_uSenderID);
		csp.set_string(3, m_szSender, strlen(m_szSender));
		csp.set_string(4, m_szTitle, strlen(m_szTitle));
		csp.set_string(5, m_szContent, strlen(m_szContent));
		csp.set_uint64(6, m_uTime);
		csp.set_tinyblob(7, m_dwItemID, sizeof(UINT32)*MAIL_ITEM_COUNT);
		csp.set_tinyblob(8, m_nItemCnt, sizeof(UINT32)*MAIL_ITEM_COUNT);
		pDB->Execute(&csp);
		return TRUE;
	}

	BOOL Delete(IDBInterface* pDB)
	{
		return TRUE;
	}
};



#endif //__MAIL_DATA_OBJECT_H__