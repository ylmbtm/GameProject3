#ifndef _LOGIC_SEVER_MANAGERH_CENTER_
#define _LOGIC_SEVER_MANAGERH_CENTER_

struct LogicServerNode
{
    LogicServerNode()
    {
        m_nConnID = 0;
        m_nServerID = 0;
    }

    INT32       m_nConnID;   //连接ID
    INT32       m_nServerID;
    std::string m_strSvrName;
};

class CLogicSvrMgr
{
    CLogicSvrMgr(void);
    ~CLogicSvrMgr(void);
public:
    static CLogicSvrMgr* GetInstancePtr();
public:
    BOOL    Init();

    BOOL    Uninit();

    BOOL    RegisterLogicServer(INT32 nConnID, INT32 nServerID, std::string strSvrName);

    BOOL    UnregisterLogicServer(INT32 nServerID);

    BOOL    OnCloseConnect(INT32 nConnID);

    INT32   GetLogicConnID(INT32 nServerID);

    BOOL    SendMsgProtoBuf(INT32 nServerID, INT32 nMsgID, const google::protobuf::Message& pdata, UINT64 uTargetID = 0, UINT32 userData = 0);

    BOOL    BroadMsgToAll(INT32 nMsgID, const google::protobuf::Message& pdata, INT32 nNoConnID = 0);

    BOOL    SendMsgRawData(INT32 nServerID, INT32 nMsgID, const char* pdata, INT32 nLen);

    LogicServerNode* GetLogicServerInfo(INT32 nServerID);
public:

    std::map<INT32, LogicServerNode*> m_mapServer;
};

#endif //_LOGIC_SEVER_MANAGERH_CENTER_