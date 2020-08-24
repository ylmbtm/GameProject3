#ifndef __MAIL_DATA_OBJECT_H__
#define __MAIL_DATA_OBJECT_H__
#include "DBInterface/DBStoredProc.h"
#include "DBInterface/DBInterface.h"

struct StMailItem
{
	StMailItem(UINT32 nID = 0, UINT32 nNum = 0)
	{
		m_nItemID = nID;
		m_nItemNum = nNum;
	}
	UINT32 m_nItemID;
	UINT32 m_nItemNum;
};

struct GroupMailDataObject : public ShareObject
{
	GroupMailDataObject()
	{
		m_uGuid = 0;
		m_uTime = 0;
		m_dwMailType = 0;
		m_dwChannel = 0;
		memset(m_szTitle, 0, sizeof(CHAR) * MAIL_TITLE_LEN);
		memset(m_szContent, 0, sizeof(CHAR) * MAIL_CONTENT_LEN);
		memset(m_Items, 0, sizeof(StMailItem) * MAIL_ITEM_COUNT);
		memset(m_szSender, 0, sizeof(CHAR) * ROLE_NAME_LEN);
	}
	UINT64 m_uGuid;							//邮件ID
	CHAR   m_szTitle[MAIL_TITLE_LEN];		//邮件标题
	CHAR   m_szContent[MAIL_CONTENT_LEN];	//邮件内容
	CHAR   m_szSender[ROLE_NAME_LEN];		//发送者名字
	UINT64 m_uTime;							//邮件时间
	UINT32 m_dwMailType;					//邮件类型
	UINT32 m_dwChannel;						//目标渠道
	StMailItem m_Items[MAIL_ITEM_COUNT];	//道具列表

	BOOL Create(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO mail_group (id, title, content,sender, mail_time, mailtype, channel, itemdata) \
			VALUES(?,?,?,?,?,?,?,?);");
		csp.set_uint64(0, m_uGuid);
		csp.set_string(1, m_szTitle, strlen(m_szTitle));
		csp.set_string(2, m_szContent, strlen(m_szContent));
		csp.set_string(3, m_szSender, strlen(m_szSender));
		csp.set_uint64(4, m_uTime);
		csp.set_uint32(5, m_dwMailType);
		csp.set_uint32(6, m_dwChannel);
		csp.set_tinyblob(7, m_Items, sizeof(StMailItem)*MAIL_ITEM_COUNT);
		return pDB->Execute(&csp);
	}

	BOOL Update(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO mail_group (id, title, content,sender,  mail_time, mailtype, channel, itemdata) \
			VALUES(?,?,?,?,?,?,?,?);");
		csp.set_uint64(0, m_uGuid);
		csp.set_string(1, m_szTitle, strlen(m_szTitle));
		csp.set_string(2, m_szContent, strlen(m_szContent));
		csp.set_string(3, m_szSender, strlen(m_szSender));
		csp.set_uint64(4, m_uTime);
		csp.set_uint32(5, m_dwMailType);
		csp.set_uint32(6, m_dwChannel);
		csp.set_tinyblob(7, m_Items, sizeof(StMailItem)*MAIL_ITEM_COUNT);
		return pDB->Execute(&csp);
	}

	BOOL Delete(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("delete from mail_group where id = ?;");
		csp.set_uint64(0, m_uGuid);
		return pDB->Execute(&csp);
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
		m_dwMailType = 0; //邮件类型
		m_dwStatus = 0;   //邮件状态
		m_uGroupGuid = 0; //对应的群邮件ID
		memset(m_szSender, 0, sizeof(CHAR) * ROLE_NAME_LEN);
		memset(m_szTitle, 0, sizeof(CHAR) * MAIL_TITLE_LEN);
		memset(m_szContent, 0, sizeof(CHAR) * MAIL_CONTENT_LEN);
		memset(m_Items, 0, sizeof(StMailItem) * MAIL_ITEM_COUNT);
	}

	UINT64 m_uGuid;							//邮件ID
	UINT64 m_uRoleID;						//角色ID
	UINT64 m_uGroupGuid;                    //群邮件ID
	UINT64 m_uTime;							//邮件时间
	UINT64 m_uSenderID;						//发送者ID
	UINT32 m_dwMailType;                    //邮件类型
	UINT32 m_dwStatus;                      //邮件状态
	CHAR   m_szSender[ROLE_NAME_LEN];		//发送者名字
	CHAR   m_szTitle[MAIL_TITLE_LEN];		//邮件标题
	CHAR   m_szContent[MAIL_CONTENT_LEN];	//邮件内容
	StMailItem m_Items[MAIL_ITEM_COUNT];	//道具列表


	BOOL Create(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO mail (id, roleid,groupid, mailtype, mailstatus, senderid, sendername, title, content, mail_time, itemdata) \
			VALUES(?,?,?,?,?,?,?,?,?,?,?);");
		csp.set_uint64(0, m_uGuid);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint64(2, m_uGroupGuid);
		csp.set_uint32(3, m_dwMailType);
		csp.set_uint32(4, m_dwStatus);
		csp.set_uint64(5, m_uSenderID);
		csp.set_string(6, m_szSender, strlen(m_szSender));
		csp.set_string(7, m_szTitle, strlen(m_szTitle));
		csp.set_string(8, m_szContent, strlen(m_szContent));
		csp.set_uint64(9, m_uTime);
		csp.set_tinyblob(10, m_Items, sizeof(StMailItem)*MAIL_ITEM_COUNT);
		return pDB->Execute(&csp);
	}

	BOOL Update(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO mail (id, roleid,groupid, mailtype, mailstatus, senderid, sendername, title, content, mail_time, itemdata) \
			VALUES(?,?,?,?,?,?,?,?,?,?,?);");
		csp.set_uint64(0, m_uGuid);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint64(2, m_uGroupGuid);
		csp.set_uint32(3, m_dwMailType);
		csp.set_uint32(4, m_dwStatus);
		csp.set_uint64(5, m_uSenderID);
		csp.set_string(6, m_szSender, strlen(m_szSender));
		csp.set_string(7, m_szTitle, strlen(m_szTitle));
		csp.set_string(8, m_szContent, strlen(m_szContent));
		csp.set_uint64(9, m_uTime);
		csp.set_tinyblob(10, m_Items, sizeof(StMailItem)*MAIL_ITEM_COUNT);
		return pDB->Execute(&csp);
	}

	BOOL Delete(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("delete from mail where id = ?;");
		csp.set_uint64(0, m_uGuid);
		return pDB->Execute(&csp);
	}
};


struct OffDataObject : public ShareObject
{
	OffDataObject()
	{
		m_uRoleID = 0;
		m_nOpType = 0;

	}

	UINT32 m_nOpType;
	UINT64 m_uRoleID;						//角色ID
	union
	{
		UINT64          m_Parm64[4];
		UINT32          m_Parm32[8];
	} m_Param; //事件参数



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