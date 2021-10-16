#ifndef __SEAL_MANAGER_H__
#define __SEAL_MANAGER_H__

#include "HttpParameter.h"
#include "DBInterface/CppMysql.h"
#include "SealData.h"

class CSealManager
{
    CSealManager();
    ~CSealManager();
public:
    static CSealManager* GetInstancePtr();

public:
    BOOL LoadData(CppMySQL3DB& tDBConnection);

    BOOL SealRole(UINT64 uRoleID, UINT64 uSealTime, INT32 nSealAction, INT32 nSealReason);

    BOOL UnSealRole(UINT64 uRoleID);

    BOOL IsSealRole(UINT64 uRoleID, INT32 nSealAction);

    SealDataObject* GetSealData(UINT64 uRoleID);
public:
    std::map<UINT64, SealDataObject*> m_mapSealData;
};

#endif //__SEAL_MANAGER_H__