#ifndef _LOGIC_SEVER_MANAGERH_CENTER_
#define _LOGIC_SEVER_MANAGERH_CENTER_

struct LogicServerNode
{
	LogicServerNode()
	{
		m_dwConnID = 0;
		m_dwServerID = 0;
	}

	UINT32		m_dwConnID;   //连接ID
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

	BOOL	RegisterLogicServer(UINT32 dwConnID, UINT32 dwServerID, std::string strSvrName);

	BOOL	UnregisterLogicServer(UINT32 dwConnID, UINT32 dwServerID);

	UINT32	GetLogicConnID(UINT32 dwServerID);

	BOOL	SendMsgProtoBuf(UINT32 dwServerID, UINT32 dwMsgID, const google::protobuf::Message& pdata);

	BOOL	SendMsgRawData(UINT32 dwServerID, UINT32 dwMsgID, const char* pdata, UINT32 dwLen);

	LogicServerNode* GetLogicServerInfo(UINT32 dwServerID);
public:

	std::map<UINT32, LogicServerNode*> m_mapServer;
};

#endif //_LOGIC_SEVER_MANAGERH_CENTER_