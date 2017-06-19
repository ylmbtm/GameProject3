#include "stdafx.h"
#include "GameSvrMgr.h"
#include "CommandDef.h"
#include "../LogicServer/GameService.h"

CGameSvrMgr::CGameSvrMgr(void)
{
}

CGameSvrMgr::~CGameSvrMgr(void)
{
}

UINT32 CGameSvrMgr::GetServerIDBySceneID( UINT32 dwSceneID )
{
	return 101;
}

BOOL CGameSvrMgr::CreateScene( UINT32 dwMapID, UINT32 CreateParam )
{
	// 选择一个最可用的服务器
	UINT32 dwSceneID = MakeNewSceneID(dwMapID);

	//选择一个可用的副本服务器
	UINT32 dwServerID = GetFreeGameServerID();
	if(dwServerID == INVALIDE_SERVERID)
	{
		ASSERT_FAIELD;
		//没有找到可用的场景服务器，或者说没有找到可用的副本服务器
		return FALSE;
	}

	//向副本服务器发送创建副本的消息
	if(SendCreateSceneCmd(dwServerID, dwSceneID, CreateParam))
	{
		//发送创建副本的消息失败

		ASSERT_FAIELD;

		return FALSE;
	}

	return TRUE;
}

BOOL CGameSvrMgr::SendCreateSceneCmd( UINT32 dwServerID, UINT32 dwSceneID, UINT32 CreateParam )
{
	
	return TRUE;
}

UINT32 CGameSvrMgr::MakeNewSceneID( UINT32 dwMapID )
{
	return dwMapID + 1000;
}

UINT32 CGameSvrMgr::GetFreeGameServerID()
{
	return 0;
}
