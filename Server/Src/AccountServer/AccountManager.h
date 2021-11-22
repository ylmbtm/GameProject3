#ifndef __DB_ACCOUNT_OBJECT_H__
#define __DB_ACCOUNT_OBJECT_H__
#include "AVLTree.h"
#include "DBInterface/CppMysql.h"
#include "LockFreeQueue.h"

struct CAccountObject
{
    UINT64      m_ID = 0;
    std::string m_strName;
    std::string m_strPassword;
    INT32       m_nLastSvrID[2] = {0};
    INT32       m_nChannel = 0;    //渠道ID
    UINT64      m_uSealTime = 0;    //封禁结束时间
    UINT64      m_uCreateTime = 0;  //创建时间
    INT32       m_nLoginCount = 0;
};


class CAccountObjectMgr : public AVLTree<UINT64, CAccountObject>
{
public:
    CAccountObjectMgr();
    ~CAccountObjectMgr();

public:
    BOOL                LoadCacheAccount();
public:
    CAccountObject*     GetAccountObjectByID(UINT64 m_u64AccountID);

    CAccountObject*     CreateAccountObject(const std::string& strName, const std::string& strPwd, INT32 nChannel);

    BOOL                ReleaseAccountObject(UINT64 m_u64AccountID);

    BOOL                SealAccount(UINT64& uAccountID, const std::string& strName, INT32 nChannel, BOOL bSeal, INT32 nSealTime, INT32& nLastSvrID);

    BOOL                SetLastServer(UINT64 uAccountID, INT32 ServerID);

    CAccountObject*     AddAccountObject(UINT64 uAccountID, const CHAR* pStrName, INT32 nChannel);

    CAccountObject*     GetAccountObject(const std::string& name, INT32 nChannel);

    BOOL                SaveAccountThread();

    BOOL                Init();

    BOOL                Uninit();

    BOOL                IsRun();

    BOOL                CheckAccountName(const std::string& strName, bool bFromChannel);

public:

    std::map<std::string, CAccountObject*>  m_mapNameObj;

    ArrayLockFreeQueue<CAccountObject*>     m_ArrChangedAccount;

    BOOL                m_IsRun;

    std::thread*        m_pThread;

    UINT64              m_u64MaxID;

    BOOL                m_bCrossChannel;   //是否区分渠道
};

#endif //__DB_ACCOUNT_OBJECT_H__
