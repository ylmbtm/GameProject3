#ifndef __MAIL_MODULE_H__
#define __MAIL_MODULE_H__
#include "ModuleBase.h"
#include "../ServerData/MailData.h"
#include "../ServerData/ServerDefine.h"

struct MailDataObject;
class CMailModule  : public CModuleBase
{
public:
	CMailModule(CPlayerObject* pOwner);

	~CMailModule();

public:
	BOOL OnCreate(UINT64 u64RoleID);

	BOOL OnDestroy();

	BOOL OnLogin();

	BOOL OnLogout();

	BOOL OnNewDay();

	BOOL ReadFromDBLoginData(DBRoleLoginAck& Ack);

	BOOL SaveToClientLoginData(RoleLoginAck& Ack);

	BOOL CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue);

public:




public:
	std::map<UINT64, MailDataObject*> m_mapMailData;


};

#endif //__MAIL_MODULE_H__
