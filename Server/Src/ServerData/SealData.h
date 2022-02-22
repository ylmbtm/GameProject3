#ifndef __SEAL_DATA_OBJECT_H__
#define __SEAL_DATA_OBJECT_H__

#include "DBInterface/DBInterface.h"
#include "DBInterface/DBStoredProc.h"

enum ESealReason
{
    ESR_GM_OPER    = 1,  //通过GM后台封禁
    ESR_CHANGE_MSG = 2,  //协议MD5验证失败
};

enum ESealAction
{
    ESA_SEAL_LOGIN = 1,  //封禁登录
    ESA_SEAL_CHAT  = 2,  //封禁聊天
};

struct SealDataObject : public ShareObject
{
    SealDataObject()
    {
        m_uRoleID = 0;
        m_uSealEndTime = 0;
        m_nSealAction = 0;
        m_uSealBeginlTime = 0;
        m_nSealReason = 0;
    }

    UINT64  m_uRoleID;
    INT32   m_nSealAction;
    UINT64  m_uSealEndTime;
    UINT64  m_uSealBeginlTime;
    INT32   m_nSealReason;

    BOOL Create(IDBInterface* pDB)
    {
        static CDBStoredProcedure csp("INSERT INTO seal_role (roleid, sealendtime,sealbegintime, sealaction, sealreason) \
			VALUES(?,?,?,?,?);");
        csp.set_uint64(0, m_uRoleID);
        csp.set_uint64(1, m_uSealEndTime);
        csp.set_uint64(2, m_uSealBeginlTime);
        csp.set_int32(3, m_nSealAction);
        csp.set_int32(4, m_nSealReason);
        return pDB->Execute(&csp);
    }

    BOOL Update(IDBInterface* pDB)
    {
        static CDBStoredProcedure csp("REPLACE INTO seal_role (roleid, sealendtime,sealbegintime, sealaction, sealreason) \
			VALUES(?,?,?,?,?);");
        csp.set_uint64(0, m_uRoleID);
        csp.set_uint64(1, m_uSealEndTime);
        csp.set_uint64(2, m_uSealBeginlTime);
        csp.set_int32(3, m_nSealAction);
        csp.set_int32(4, m_nSealReason);
        return pDB->Execute(&csp);
    }

    BOOL Delete(IDBInterface* pDB)
    {
        static CDBStoredProcedure csp("delete from seal_role where roleid = ?");
        csp.set_uint64(0, m_uRoleID);
        return pDB->Execute(&csp);
    }
};

#endif //__SEAL_DATA_OBJECT_H__