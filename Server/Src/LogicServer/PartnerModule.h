#ifndef __PARTNER_MODULE_H__
#define __PARTNER_MODULE_H__
#include "ModuleBase.h"
#include "DBInterface/DBStoredProc.h"
#include "../ServerData/PartnerData.h"

struct PartnerDataObject;
class CPartnerModule  : public CModuleBase
{
public:
	CPartnerModule(CPlayerObject* pOwner);

	~CPartnerModule();

public:
	BOOL OnCreate(UINT64 u64RoleID);

	BOOL OnDestroy();

	BOOL OnLogin();

	BOOL OnLogout();

	BOOL OnNewDay();

	BOOL ReadFromDBLoginData(DBRoleLoginAck& Ack);

	BOOL SaveToClientLoginData(RoleLoginAck& Ack);

	BOOL CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue);

	BOOL DispatchPacket(NetPacket* pNetPacket);

public:
	UINT64 AddPartner(UINT32 dwPartnerID);

	BOOL NotifyChange();

	PartnerDataObject* GetPartnerByGuid(UINT64 uGuid);

public:
	//*********************消息处理定义开始******************************
	BOOL OnMsgSetupPartnerReq(NetPacket* pNetPacket);
	BOOL OnMsgUnsetPartnerReq(NetPacket* pNetPacket);
	//*********************消息处理定义结束******************************

public:
	std::map<UINT64, PartnerDataObject*>m_mapPartnerData;
	PartnerDataObject* m_vtSetupPartner[PARTNER_MAX_NUM];

	std::set<UINT64> m_setChange;
	std::set<UINT64> m_setRemove;


};

#endif //__PARTNER_MODULE_H__
