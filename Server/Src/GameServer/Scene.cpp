#include "stdafx.h"
#include "CommandDef.h"
#include "GameDefine.h"
#include "ObjectID.h"
#include "GameService.h"
#include "Scene.h"
#include "PacketHeader.h"
#include "Utility/Log/Log.h"
#include "Utility/CommonFunc.h"
#include "Utility/CommonEvent.h"
#include "DataBuffer.h"
CScene::CScene()
{
	
}

CScene::~CScene()
{

}

BOOL CScene::Init(UINT32 dwSceneID, INT32 nLeft, INT32 nRight, INT32 nTop, INT32 nBottom)
{
	m_dwSceneID = dwSceneID;

	m_GridManager.Init(nLeft, nRight, nTop, nBottom);

	return TRUE;
}

BOOL CScene::Uninit()
{
	return TRUE;
}

BOOL CScene::DispatchPacket(NetPacket *pNetPacket)
{
	PacketHeader *pPacketHeader = (PacketHeader *)pNetPacket->m_pDataBuffer->GetBuffer();
	if(pPacketHeader == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	switch(pPacketHeader->dwMsgID)
	{
		PROCESS_MESSAGE_ITEM(CMD_CHAR_LEAVE_GAME_REQ,	OnCmdLeaveGameReq);
		PROCESS_MESSAGE_ITEM(CMD_CHAR_MOVE_REQ,			OnCmdPlayerMove);
		PROCESS_MESSAGE_ITEM(MSG_TRANS_ROLE_DATA_REQ,   OnMsgTransRoleDataReq);
		default:
		{
			CPlayerObject *pPlayerObject = m_PlayerObjectMgr.GetPlayer(pPacketHeader->u64RoleID);
			if(pPlayerObject != NULL)
			{
				pPlayerObject->DispatchPacket(pNetPacket);
			}
		}
		break;
	}

	return TRUE;
}



BOOL CScene::AddToMap(CWorldObject *pWorldObject)
{
	INT32 dwIndex = m_GridManager.GetIndexByPos(pWorldObject->m_ObjectPos.x, pWorldObject->m_ObjectPos.z);
	if(dwIndex == -1)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	CGrid *pGrid = m_GridManager.GetGridByIndex(dwIndex);
	if(pGrid == NULL)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	ASSERT(!pGrid->IsObjectExist(pWorldObject));

	pGrid->AddObject(pWorldObject);

	pWorldObject->SetOwnerScene(this);

	pWorldObject->SetUpdate(UT_New);

	return TRUE;
}

BOOL CScene::AddToMapPos( CWorldObject *pWorldObject, FLOAT x, FLOAT z )
{
	pWorldObject->m_ObjectPos.x = x;
	pWorldObject->m_ObjectPos.z = z;

	return AddToMap(pWorldObject);
}


BOOL CScene::OnMsgPlayerMove(NetPacket *pNetPack)
{
	StCharMoveReq CharMoveReq;
	CBufferHelper bh(FALSE, pNetPack->m_pDataBuffer);
	bh.Read(CharMoveReq);

	CPlayerObject *pPlayerObj = m_PlayerObjectMgr.GetPlayer(bh.GetPacketHeader()->u64RoleID);
	if(pPlayerObj == NULL)
	{
		ASSERT_FAIELD;
		return TRUE;
	}

	INT32 nDestIndex = m_GridManager.GetIndexByPos(CharMoveReq.x, CharMoveReq.z);
	if(nDestIndex == -1)
	{
		//不合法的目标地址
		ASSERT_FAIELD;
		return TRUE;
	}

	INT32 nCurIndex  = m_GridManager.GetIndexByPos(pPlayerObj->m_ObjectPos.x, pPlayerObj->m_ObjectPos.z);

	ASSERT(m_GridManager.IsObjectExist(pPlayerObj, nCurIndex));

	if(nDestIndex != nCurIndex)  //如果还在一个格子里，只需要修改一下坐标就行了。
	{
		ASSERT(!m_GridManager.IsObjectExist(pPlayerObj, nDestIndex));

		m_GridManager.RemoveObjectFromGrid(pPlayerObj, nCurIndex);

		m_GridManager.AddObjectToGrid(pPlayerObj, nDestIndex);
	}
	
	pPlayerObj->m_ObjectPos.x = CharMoveReq.x;
	pPlayerObj->m_ObjectPos.y = CharMoveReq.y;
	pPlayerObj->m_ObjectPos.z = CharMoveReq.z;
	pPlayerObj->m_ObjectStatus.nDir = CharMoveReq.sDir;

	pPlayerObj->SetUpdate(UT_Update);

	CLog::GetInstancePtr()->AddLog("响应客户端坐标请求[%lld], 坐标 x =%f, z=%f", pPlayerObj->GetObjectID(), pPlayerObj->m_ObjectPos.x, pPlayerObj->m_ObjectPos.z);

	return TRUE;
}

BOOL CScene::OnMsgTransRoleDataReq(NetPacket *pNetPacket)
{
	
}

BOOL CScene::SendNewObjectToGrids(CWorldObject *pWorldObject, INT32 Grids[9])
{
	//先把玩家的完整包组装好
	CBufferHelper WriteHelper(TRUE, 1024);
	WriteHelper.BeginWrite(CMD_CHAR_NEARBY_ADD, 0, 0);
	UINT8 objType = pWorldObject->GetObjectType();
	UINT32 dwCount = 1;
	WriteHelper.Write(dwCount);
	WriteHelper.Write(objType);
	pWorldObject->WriteToBuffer(&WriteHelper, UPDATE_FLAG_CREATE, UPDATE_TO_OTHERS);
	WriteHelper.EndWrite();
	//变化包组装完成
	
	UINT32 i = 0;
	while(Grids[i] != -1)
	{	
		CGrid *pGrid = m_GridManager.GetGridByIndex(Grids[i]);
		if(pGrid == NULL)
		{
			ASSERT_FAIELD;
			return FALSE;
		}
		
		CWorldObject *pIterObj = pGrid->m_pHead;
		//生成完整其它玩家包, 发送当前玩家
		while(pIterObj != NULL)
		{
			//开始发送给所有的玩家
			if(pIterObj->GetObjectID() != pWorldObject->GetObjectID())
			{
				if(pIterObj->GetObjectType() == OBJECT_PLAYER)
				{
					ServiceBase::GetInstancePtr()->SendCmdToConnection(((CPlayerObject*)pIterObj)->GetConnectID(),pIterObj->GetObjectID(), 0, &m_WriteBuffer);
				}
			}

			pIterObj = pIterObj->m_pGridNext;
		}

		i++;
	}

	return TRUE;
}

BOOL CScene::SendNewGridsToObject( INT32 Grids[9], CPlayerObject *pPlayerObj )
{
	//先把玩家的完整包组装好
	CBufferHelper WriteHelper(TRUE, 1024);
	WriteHelper.BeginWrite(CMD_CHAR_NEARBY_ADD, 0, pPlayerObj->GetObjectID());

	UINT32 *pCount = (UINT32*)WriteHelper.GetCurrentPoint();
	*pCount = 0;
	WriteHelper.Write(*pCount);

	UINT32 i = 0;
	while(Grids[i] != -1)
	{	
		CGrid *pGrid = m_GridManager.GetGridByIndex(Grids[i]);
		if(pGrid == NULL)
		{
			ASSERT_FAIELD;
			return FALSE;
		}

		CWorldObject *pIterObj = pGrid->m_pHead;
		//生成完整其它玩家包，发送当前玩家
		while(pIterObj != NULL)
		{
			if(pIterObj->GetObjectID() != pPlayerObj->GetObjectID())
			{
				UINT8 objType = pIterObj->GetObjectType();

				WriteHelper.Write(objType);

				pIterObj->WriteToBuffer(&WriteHelper, UPDATE_FLAG_CREATE, UPDATE_TO_OTHERS);

				(*pCount)++;
			}

			pIterObj = pIterObj->m_pGridNext;
		}

		i++;
	}

	WriteHelper.EndWrite();

	if((*pCount) <= 0)
	{
		return TRUE;
	}

	ServiceBase::GetInstancePtr()->SendCmdToConnection(pPlayerObj->GetConnectID(), pPlayerObj->GetObjectID(), 0,  &m_WriteBuffer);

	return TRUE;
}

BOOL CScene::SendUpdateObjectToGrids(CWorldObject *pWorldObj, INT32 Grids[9] )
{
	//先把玩家的变化包组装好
	CBufferHelper WriteHelper(TRUE, 1024);
	WriteHelper.BeginWrite(CMD_CHAR_NEARBY_UPDATE, 0, 0);
	UINT32 dwCount = 1;
	WriteHelper.Write(dwCount);
	WriteHelper.Write(pWorldObj->GetObjectID());
	pWorldObj->WriteToBuffer(&WriteHelper, UPDATE_FLAG_CHANGE, UPDATE_TO_OTHERS);
	WriteHelper.EndWrite();
	//变化包组装完成

	int i = 0;
	while(Grids[i] != -1)
	{
		CGrid *pGrid = m_GridManager.GetGridByIndex(Grids[i]);
		if(pGrid == NULL)
		{
			ASSERT_FAIELD;
			return FALSE;
		}
		
		CWorldObject *pIterObj = pGrid->m_pHead;

		while(pIterObj != NULL)
		{
			//开始发送给所有的玩家
			if(pIterObj->GetObjectID() != pWorldObj->GetObjectID())
			{
				if(pIterObj->GetObjectType() == OBJECT_PLAYER)
				{
					CPlayerObject *pIterPlayer = (CPlayerObject*)pIterObj;
					ServiceBase::GetInstancePtr()->SendCmdToConnection(pIterPlayer->GetConnectID(), pIterPlayer->GetObjectID(), 0, &m_WriteBuffer);
				}
			}

			pIterObj = pIterObj->m_pGridNext;
		}

		i++;
	}

	return TRUE;
}

BOOL CScene::SendRemoveObjectToGrids( UINT64 u64RoleID, INT32 Grids[9] )
{
	//先把玩家的变化包组装好
	CBufferHelper WriteHelper(TRUE, 1024);
	WriteHelper.BeginWrite(CMD_CHAR_NEARBY_REMOVE, 0, 0);
	UINT32 dwCount = 1;
	WriteHelper.Write(dwCount);
	WriteHelper.Write(u64RoleID);
	WriteHelper.EndWrite();
	//变化包组装完成

	UINT32 i = 0;
	while(Grids[i] != -1)
	{
		CGrid *pGrid = m_GridManager.GetGridByIndex(Grids[i]);
		if(pGrid == NULL)
		{
			ASSERT_FAIELD;
			return FALSE;
		}

		CWorldObject *pIterObj = pGrid->m_pHead;
		while(pIterObj != NULL)
		{
			//开始发送给所有的玩家
			CPlayerObject *pIterPlayer = (CPlayerObject*)pIterObj;

			if(pIterPlayer->GetObjectID() == u64RoleID)
			{
				ASSERT_FAIELD;
			}

			CLog::GetInstancePtr()->AddLog("自己[%lld]--Remove--From---[%lld], ",  u64RoleID, pIterObj->GetObjectID());

			ServiceBase::GetInstancePtr()->SendCmdToConnection(pIterPlayer->GetConnectID(), pIterPlayer->GetObjectID(), 0, &m_WriteBuffer);

			pIterObj = pIterObj->m_pGridNext;
		}

		i++;
	}


	return TRUE;
}

BOOL CScene::SendRemoveGridsToPlayer( INT32 Grids[9], CPlayerObject *pPlayerObj)
{
	CBufferHelper WriteHelper(TRUE, 1024);
	WriteHelper.BeginWrite(CMD_CHAR_NEARBY_REMOVE , 0, 0);
	
	UINT32 *pCount = (UINT32*)WriteHelper.GetCurrentPoint();
	UINT32 Value = 0;
	WriteHelper.Write(Value);

	UINT32 i = 0;
	while(Grids[i] != -1)
	{
		CGrid *pGrid = m_GridManager.GetGridByIndex(Grids[i]);
		if(pGrid == NULL)
		{
			ASSERT_FAIELD;
			return FALSE;
		}
		
		CWorldObject *pIterObj = pGrid->m_pHead;
		while(pIterObj != NULL)
		{
			//开始发送给所有的玩家
			WriteHelper.Write(pIterObj->GetObjectID());

			if(pIterObj->GetObjectID() == pPlayerObj->GetObjectID())
			{
				ASSERT_FAIELD;
			}

			CLog::GetInstancePtr()->AddLog("自己[%lld]--Remove--From---[%lld], ",  pPlayerObj->GetObjectID(), pIterObj->GetObjectID());

			*pCount = *pCount + 1;

			pIterObj = pIterObj->m_pGridNext;
		}

		i++;
	}

	WriteHelper.EndWrite();

	if((*pCount) <= 0)
	{
		return TRUE;
	}

	ServiceBase::GetInstancePtr()->SendCmdToConnection(pPlayerObj->GetConnectID(), pPlayerObj->GetObjectID(), 0, &m_WriteBuffer);

	return TRUE;
}

BOOL CScene::OnMsgLeaveGameReq(NetPacket *pNetPack)
{
	StCharLeaveGameReq CharLeaveGameReq;
	CBufferHelper bh(FALSE, pNetPack->m_pDataBuffer);

	bh.Read(CharLeaveGameReq);

	if(CharLeaveGameReq.dwLeaveReason == LGR_Disconnect)
	{

	}
	else if(CharLeaveGameReq.dwLeaveReason == LGR_Quit)
	{

	}

	CPlayerObject *pPlayerObject = m_PlayerObjectMgr.GetPlayer(bh.GetPacketHeader()->u64RoleID);
	if(pPlayerObject == NULL)
	{
		ASSERT_FAIELD;
		return TRUE;
	}

	RemoveFromMap(pPlayerObject);

	m_PlayerObjectMgr.RemovePlayer(pPlayerObject->GetObjectID());

	return TRUE;
}

BOOL CScene::RemoveFromMap( CWorldObject *pWorldObject )
{
	INT32 dwIndex = m_GridManager.GetIndexByPos(pWorldObject->GetPosition().x, pWorldObject->GetPosition().z);
	if(dwIndex == -1)
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	CGrid *pGrid = m_GridManager.GetGridByIndex(dwIndex);
	if(pGrid == NULL)
	{
		ASSERT_FAIELD;
		
	}

	if(!pGrid->RemoveObject(pWorldObject))
	{
		ASSERT_FAIELD;
		return FALSE;
	}

	pWorldObject->SetUpdate(UT_Delete);

	return TRUE;

}

BOOL CScene::OnUpdate( UINT32 dwTick )
{
	HandleUpdateList();
	
	return TRUE;
}

BOOL CScene::AddToUpdateList( CWorldObject *pWorldObject )
{
	return m_UpdateObjectMgr.AddUpdateObject(pWorldObject);
}

BOOL CScene::RemoveFromUpList( CWorldObject *pWorldObject )
{
	return m_UpdateObjectMgr.RemoveUpdateObject(pWorldObject);
}

BOOL CScene::HandleUpdateList()
{
	CWorldObject *pWorldObject = m_UpdateObjectMgr.GetFisrtOjbect();
	while(pWorldObject != NULL)
	{
		HandleUpdateObject(pWorldObject);

		pWorldObject = m_UpdateObjectMgr.GetFisrtOjbect();
	}
	
	return TRUE;
}

BOOL CScene::HandleUpdateObject(CWorldObject *pWorldObject)
{
	if(pWorldObject == NULL)
	{
		ASSERT_FAIELD;
		return TRUE;
	}

	INT32 nCurIndex = m_GridManager.GetIndexByPos(pWorldObject->m_ObjectPos.x, pWorldObject->m_ObjectPos.z);
	if(nCurIndex == -1)
	{
		ASSERT_FAIELD;//不合法的地址

		return TRUE;
	}

	if(pWorldObject->m_UpdateStatus == UT_None)
	{
		ASSERT_FAIELD;
	}
	else if(pWorldObject->m_UpdateStatus == UT_New)
	{
		INT32 Grids[10];

		m_GridManager.GetSurroundingGrids(nCurIndex, Grids);

		SendNewObjectToGrids(pWorldObject, Grids);

		if(pWorldObject->GetObjectType() == OBJECT_PLAYER)
		{
			SendNewGridsToObject(Grids, (CPlayerObject*)pWorldObject);
		}

		pWorldObject->m_UpdateObjPos = pWorldObject->m_ObjectPos;

		pWorldObject->m_UpdateStatus   = UT_None;
	}
	else if(pWorldObject->m_UpdateStatus == UT_Delete)
	{
		INT32 Grids[10];

		m_GridManager.GetSurroundingGrids(nCurIndex, Grids);

		SendRemoveObjectToGrids(pWorldObject->GetObjectID(), Grids);

		if(pWorldObject->GetObjectType() == OBJECT_PLAYER)
		{
			delete (CPlayerObject*)pWorldObject;
		}
	}
	else if(pWorldObject->m_UpdateStatus == UT_Update)
	{
		INT32 nSrcIndex  = m_GridManager.GetIndexByPos(pWorldObject->m_UpdateObjPos.x, pWorldObject->m_UpdateObjPos.z);

		SendUpdateObjectToMyself(pWorldObject);

		if(nCurIndex == nSrcIndex)  //如果还在一个格子里，只需要修改一下坐标就行了。
		{
			INT32 Grids[10];

			m_GridManager.GetSurroundingGrids(nCurIndex, Grids);

			SendUpdateObjectToGrids(pWorldObject, Grids);
			
			pWorldObject->m_UpdateObjPos = pWorldObject->m_ObjectPos;
		
			return TRUE;
		}

		INT32 AddGrid[10], RemoveGrid[10], StayGrid[10];

		m_GridManager.CalDiffGrids(nSrcIndex, nCurIndex, AddGrid, RemoveGrid, StayGrid);

		//将玩家发的更新发给周围的格子里
		SendUpdateObjectToGrids(pWorldObject, StayGrid);

		SendNewObjectToGrids(pWorldObject, AddGrid);

		SendNewGridsToObject(AddGrid, (CPlayerObject*)pWorldObject);

		SendRemoveObjectToGrids(pWorldObject->GetObjectID(), RemoveGrid);

		SendRemoveGridsToPlayer(RemoveGrid, (CPlayerObject*)pWorldObject);

		pWorldObject->m_UpdateObjPos = pWorldObject->m_ObjectPos;

		pWorldObject->m_UpdateStatus   = UT_None;
	}
	
	return TRUE;
}

BOOL CScene::SendUpdateObjectToMyself( CWorldObject *pWorldObj )
{
	CPlayerObject *pPlayerObject = (CPlayerObject *)pWorldObj;

	//先把玩家的变化包组装好
	CBufferHelper WriteHelper(TRUE, 1024);
	WriteHelper.BeginWrite(CMD_CHAR_UPDATE_MYSELF, GetSceneID(), pPlayerObject->GetObjectID());
	WriteHelper.WriteCheckBufferCode();
	pWorldObj->WriteToBuffer(&WriteHelper, UPDATE_FLAG_CHANGE, UPDATE_TO_MYSELF);
	WriteHelper.WriteCheckBufferCode();
	WriteHelper.EndWrite();

	ServiceBase::GetInstancePtr()->SendCmdToConnection(pPlayerObject->GetConnectID(), &m_WriteBuffer);

	return TRUE;
}

BOOL CScene::OnMsgTransRoleDataReq(NetPacket *pNetPack)
{
	TransRoleDataReq Req;
	Req.ParsePartialFromArray(pNetPack->m_pDataBuffer->GetData(), pNetPack->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPack->m_pDataBuffer->GetBuffer();
	ASSERT(pHeader->u64TargetID != 0);

	//根据数据创建宠物，英雄

	CPlayerObject *pPlayerObject = new CPlayerObject;

	m_PlayerObjectMgr.AddPlayer(pPlayerObject);

	if(AddToMap(pPlayerObject))
	{
		ServiceBase::GetInstancePtr()->SendCmdToConnection(SvrEnterSceneReq.dwProxySvrID, &m_WriteBuffer);
	}
	else
	{
		ASSERT_FAIELD;
		return TRUE;
	}

	return TRUE;
}

BOOL CScene::SetSceneMapCoords( INT32 nLeft, INT32 nRight, INT32 nTop, INT32 nBottom )
{
	return TRUE;
}




