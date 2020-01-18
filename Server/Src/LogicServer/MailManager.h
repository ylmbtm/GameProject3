#ifndef __GROUP_MAIL_MANAGER_H__
#define __GROUP_MAIL_MANAGER_H__
#include "../ServerData/MailData.h"
#include "DBInterface/CppMysql.h"
class CPlayerObject;
class CMailManager
{
	CMailManager();
	~CMailManager();
public:
	static CMailManager* GetInstancePtr();

	BOOL  SendGroupMail();

	BOOL  SendSingleMail(UINT64 uRoleID);

	BOOL  SendOffOperation(UINT64 uRoleID);

	BOOL  LoadData(CppMySQL3DB& tDBConnection);

	BOOL  LoadGroupMailData(CppMySQL3DB& tDBConnection);

	BOOL  LoadOffMailData(CppMySQL3DB& tDBConnection);

	BOOL  ProcessRoleLogin(CPlayerObject* pPlayer);
public:
	std::map<UINT64, GroupMailDataObject*>  m_mapGroupMailData;

	std::vector<OffMailDataObject*>    m_vtOffMailData;
};
#endif //__GROUP_MAIL_MANAGER_H__