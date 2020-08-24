#ifndef __GROUP_MAIL_MANAGER_H__
#define __GROUP_MAIL_MANAGER_H__
#include "MailData.h"
#include "DBInterface/CppMysql.h"
#include "../Message/Game_Define.pb.h"
class CPlayerObject;
class CMailManager
{
	CMailManager();
	~CMailManager();
public:
	static CMailManager* GetInstancePtr();

	BOOL  SendGroupMail(std::string strSender, std::string strTitle, std::string strContent, std::vector<StMailItem>& vtItems, INT32 nRecvGroup);

	BOOL  SendSingleMail(UINT64 uRoleID, EMailType eMailType, std::string strContent, std::vector<StMailItem>& vtItems, std::string strSender = "", std::string strTitle = "");

	BOOL  SendOffOperation(UINT64 uRoleID);

	BOOL  DeleteGroupMail(UINT64 uGuid);

	BOOL  LoadData(CppMySQL3DB& tDBConnection);

	BOOL  LoadGroupMailData(CppMySQL3DB& tDBConnection);

	BOOL  ProcessRoleLogin(CPlayerObject* pPlayer);

	MailDataObject* PickUpMailData(UINT64 uGuid);

public:
	std::map<UINT64, GroupMailDataObject*>  m_mapGroupMailData;

	std::map<UINT64, MailDataObject*> m_mapOffMailData;
};
#endif //__GROUP_MAIL_MANAGER_H__