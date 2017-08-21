#ifndef __ROLE_MODULE_H__
#define __ROLE_MODULE_H__
#include "ModuleBase.h"
#include "../ServerData/ServerStruct.h"

struct RoleDataObject;
class CRoleModule : public CModuleBase
{
public:
	CRoleModule(CPlayerObject* pOwner);

	~CRoleModule();

public:
	BOOL    OnCreate(UINT64 u64RoleID);

	BOOL    OnDestroy();

	BOOL    OnLogin();

	BOOL    OnLogout();

	BOOL    OnNewDay();

	BOOL    InitBaseData(UINT64 u64RoleID, std::string Name, UINT32 dwCarrerID, UINT64 u64AccountID, UINT32 dwChannel);

	BOOL    ReadFromDBLoginData(DBRoleLoginAck& Ack);

	BOOL	SaveToClientLoginData(RoleLoginAck& Ack);

	BOOL	CalcFightValue(INT32 nValue[MAX_PROPERTY_NUM], INT32 nPercent[MAX_PROPERTY_NUM], INT32& FightValue);
public:
	//扣除货币， 如果返回成功，就是扣除成功， 如果返回失败，就是货币不足
	BOOL    CostMoney(UINT32 dwMoneyID, INT32 nMoneyNum);

	BOOL    CheckMoneyEnough(UINT32 dwMoneyID, INT32 nMoneyNum);

	UINT64  GetMoney(UINT32 dwMoneyID);

	UINT64  AddMoney(UINT32 dwMoneyID, INT32 nMoneyNum);

	//扣除行动力， 如果返回成功，就是扣除成功， 如果返回失败，就是行动力不足
	BOOL    CostAction(UINT32 dwActionID, INT32 nActionNum);

	BOOL    CheckActionEnough(UINT32  dwActionID, INT32 nActionNum);

	UINT64  GetAction(UINT32 dwActionID);

	UINT64  AddAction( UINT32 dwActionID, INT32 nActionNum);

	BOOL    UpdateAction(UINT32 dwActionID);

	BOOL    SetDelete(BOOL bDelete);

	UINT32  GetActorID();

	UINT64  AddExp(INT32 nExp);

public:
	UINT32           m_dwActorID;

	RoleDataObject*  m_pRoleDataObject;
};

#endif //__ROLE_MODULE_H__
