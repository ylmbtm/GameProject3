#ifndef __PAY_DATA_OBJECT_H__
#define __PAY_DATA_OBJECT_H__

#include "DBInterface/DBInterface.h"
#include "DBInterface/DBStoredProc.h"

enum EOrderStatus
{
    EOS_WAIT = 1,  //等待发货
    EOS_DONE = 2   //完成
};

struct PayDataObject : public ShareObject
{
    PayDataObject()
    {
        m_uRoleID = 0;
        m_nChannel = 0;
        m_nStatus = 0;
        m_uFinishTime = 0;
        m_nProductID = 0;
        m_nBuyID = 0;
        m_fMoney = 0.0f;
        memset(m_szOrderID, 0, PAY_ORDERID_LEN);
        memset(m_szThirdID, 0, PAY_ORDERID_LEN);
    }

    UINT64  m_uRoleID;
    CHAR    m_szOrderID[PAY_ORDERID_LEN];
    CHAR    m_szThirdID[PAY_ORDERID_LEN];
    INT32   m_nChannel = 0;
    INT32   m_nStatus;  //1：表示未发货, 2:表示己发货
    UINT64  m_uFinishTime;
    UINT64  m_uSendTime;
    INT32   m_nProductID; //产品ID
    INT32   m_nBuyID;   //游戏内真实的商品ID
    FLOAT   m_fMoney;

    BOOL Create(IDBInterface* pDB)
    {
        static CDBStoredProcedure csp("INSERT INTO payment (roleid, orderid,thirdid,channel, status, money, finish_time, send_time, productid, buyid) \
			VALUES(?,?,?,?,?,?,?,?,?,?);");
        csp.set_uint64(0, m_uRoleID);
        csp.set_string(1, m_szOrderID, strlen(m_szOrderID));
        csp.set_string(2, m_szThirdID, strlen(m_szThirdID));
        csp.set_int32(3, m_nChannel);
        csp.set_int32(4, m_nStatus);
        csp.set_float(5, m_fMoney);
        csp.set_uint64(6, m_uFinishTime);
        csp.set_uint64(7, m_uSendTime);
        csp.set_int32(8, m_nProductID);
        csp.set_int32(9, m_nBuyID);

        return pDB->Execute(&csp);
    }

    BOOL Update(IDBInterface* pDB)
    {
        static CDBStoredProcedure csp("REPLACE INTO payment (roleid, orderid,thirdid, channel, status, money, finish_time, send_time, productid, buyid) \
			VALUES(?,?,?,?,?,?,?,?,?,?);");
        csp.set_uint64(0, m_uRoleID);
        csp.set_string(1, m_szOrderID, strlen(m_szOrderID));
        csp.set_string(2, m_szThirdID, strlen(m_szThirdID));
        csp.set_int32(3, m_nChannel);
        csp.set_int32(4, m_nStatus);
        csp.set_float(5, m_fMoney);
        csp.set_uint64(6, m_uFinishTime);
        csp.set_uint64(7, m_uSendTime);
        csp.set_int32(8, m_nProductID);
        csp.set_int32(9, m_nBuyID);
        return pDB->Execute(&csp);
    }

    BOOL Delete(IDBInterface* pDB)
    {
        static CDBStoredProcedure csp("delete from payment where roleid = ? and orderid=?");
        csp.set_uint64(0, m_uRoleID);
        csp.set_string(1, m_szOrderID, strlen(m_szOrderID));
        return pDB->Execute(&csp);
    }
};

#endif //__GUILD_DATA_OBJECT_H__