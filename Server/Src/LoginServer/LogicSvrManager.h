#pragma once
#include "Utility\AVLTree.h"

struct LogicServerNode 
{
	UINT32 dwConnID;   //Á¬½ÓID
	UINT32 dwServerID;
	string strServerName;
	string strIpAddr;
	UINT32 dwPort;   ///¶Ë¿ÚºÅ
	UINT32 m_State;
	UINT32 m_Flag;
};

class LogicSvrManager : public AVLTree<UINT32, LogicServerNode>
{
public:
	LogicSvrManager(void);
	~LogicSvrManager(void);


public:
	BOOL Init();

	BOOL RegisterLogicServer(UINT32 dwConnID, UINT32 dwServerID);

	BOOL UnregisterLogicServer(UINT32 dwConnID, UINT32 dwServerID);

	UINT32 GetLogicConnID(UINT32 dwServerID);

	LogicServerNode* GetLogicServerInfo(UINT32 dwServerID);
};

