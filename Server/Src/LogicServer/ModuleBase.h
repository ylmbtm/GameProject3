#ifndef __CMODULE_BASE_H__
#define __CMODULE_BASE_H__
#include "AVLTree.h"
#include "Position.h"
#include "../Message/Msg_LoginDBData.pb.h"
#include "../Message/Msg_LoginCltData.pb.h"
#include "../ServerData/ServerDefine.h"

class CPlayerObject;

class CModuleBase
{
public:
    CModuleBase(CPlayerObject* pOwner);

    virtual ~CModuleBase();

    virtual BOOL OnCreate(UINT64 uRoleID) = 0;

    virtual BOOL OnDestroy() = 0;

    virtual BOOL OnLogin() = 0;

    virtual BOOL OnLogout() = 0;

    virtual BOOL OnNewDay() = 0;

    virtual BOOL NotifyChange() = 0;

    virtual BOOL ReadFromDBLoginData(DBRoleLoginAck& Ack) = 0;

    virtual BOOL SaveToClientLoginData(RoleLoginAck& Ack) = 0;

    virtual BOOL CalcFightValue(INT32 nValue[PROPERTY_NUM], INT32 nPercent[PROPERTY_NUM], INT32& FightValue);

    virtual INT64 GetProperty(ERoleProperty ePropertyID);
public:
    BOOL AddChangeID(UINT64 uID);

    BOOL AddRemoveID(UINT64 uID);

    BOOL SetOwner(CPlayerObject* pOwner);

    CPlayerObject* GetOwner();

public:
    CPlayerObject* m_pOwnPlayer;

    std::set<UINT64> m_setChange;

    std::set<UINT64> m_setRemove;
};


#endif //__CMODULE_BASE_H__
