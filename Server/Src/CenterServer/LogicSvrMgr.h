#ifndef _LOGIC_SEVER_MANAGERH_CENTER_
#define _LOGIC_SEVER_MANAGERH_CENTER_

struct LogicServerNode
{
	LogicServerNode()
	{
		m_nConnID = 0;
		m_dwServerID = 0;
	}

	UINT32		m_nConnID;   //连接ID
	UINT32		m_dwServerID;
	std::string m_strSvrName;
};

class LogicSvrMgr
{
	LogicSvrMgr(void);
	~LogicSvrMgr(void);
public:
	static LogicSvrMgr* GetInstancePtr();
public:
	BOOL	Init();

	BOOL	RegisterLogicServer(INT32 nConnID, UINT32 dwServerID, std::string strSvrName);

	BOOL	UnregisterLogicServer(UINT32 dwServerID);

	BOOL    OnCloseConnect(INT32 nConnID);

	UINT32	GetLogicConnID(UINT32 dwServerID);

	BOOL	SendMsgProtoBuf(UINT32 dwServerID, INT32 nMsgID, const google::protobuf::Message& pdata);

	BOOL    BroadMsgToAll(INT32 nMsgID, const google::protobuf::Message& pdata, INT32 nNoConnID = 0);

	BOOL	SendMsgRawData(UINT32 dwServerID, INT32 nMsgID, const char* pdata, UINT32 dwLen);

	LogicServerNode* GetLogicServerInfo(UINT32 dwServerID);
public:

	std::map<UINT32, LogicServerNode*> m_mapServer;
};

#endif //_LOGIC_SEVER_MANAGERH_CENTER_