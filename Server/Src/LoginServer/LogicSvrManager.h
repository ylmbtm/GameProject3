#ifndef _LOGIC_SEVER_MANAGERH_
#define _LOGIC_SEVER_MANAGERH_

#include "AVLTree.h"

struct LogicServerNode
{
	LogicServerNode()
	{
		dwConnID = 0;
		dwServerID = 0;
		dwPort = 0;
		m_State = 0;
		m_Flag = 0;
	}
	UINT32 dwConnID;   //Á¬½ÓID
	UINT32 dwServerID;
	std::string strServerName;
	std::string strIpAddr;
	UINT32 dwPort;   ///¶Ë¿ÚºÅ
	UINT32 m_State;
	UINT32 m_Flag;
};

class LogicSvrManager : public std::map<UINT32, LogicServerNode>
{
public:
	LogicSvrManager(void);
	~LogicSvrManager(void);

public:
	BOOL Init();

	BOOL RegisterLogicServer(UINT32 dwConnID, UINT32 dwServerID, std::string strIpAddr, UINT32 dwPort);

	BOOL UnregisterLogicServer(UINT32 dwConnID, UINT32 dwServerID);

	UINT32 GetLogicConnID(UINT32 dwServerID);

	LogicServerNode* GetRecommendServerInfo();

	LogicServerNode* GetLogicServerInfo(UINT32 dwServerID);

public:
	UINT32 m_dwRecommendSvrID;
};

#endif //_LOGIC_SEVER_MANAGERH_