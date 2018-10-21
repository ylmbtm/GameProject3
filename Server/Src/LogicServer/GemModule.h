#ifndef __GEM_MODULE_H__
#define __GEM_MODULE_H__
#include "ModuleBase.h"
#include "../ServerData/ServerDefine.h"
#include "../Message/Game_Define.pb.h"
struct GemDataObject;
class CGemModule : public CModuleBase
{
public:
	CGemModule(CPlayerObject* pOwner);

	~CGemModule();

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
	UINT64 AddGem(UINT32 dwGemID);

	BOOL NotifyChange();

	GemDataObject* GetGemByGuid(UINT64 uGuid);

	UINT32 DressGem(UINT64 uGuid, UINT64 uBagGuid, INT32 Pos);

	UINT32 UnDressGem(UINT64 uGuid);

public:
	//*********************消息处理定义开始******************************
	BOOL OnMsgSetupGemReq(NetPacket* pNetPacket);  //穿装备请求
	BOOL OnMsgUnsetGemReq(NetPacket* pNetPacket);  //穿装备请求
	//*********************消息处理定义结束******************************

public:
	std::map<UINT64, GemDataObject*>m_mapGemData;
	GemDataObject* m_vtDressGem[40];

	std::set<UINT64> m_setChange;
	std::set<UINT64> m_setRemove;


};

#endif //__GEM_MODULE_H__
