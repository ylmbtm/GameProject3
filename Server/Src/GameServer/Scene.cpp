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
#include "../Message/Msg_ID.pb.h"
#include "../Message/Msg_Login.pb.h"
#include "SceneLogic/SceneLogic_Type.h"
#include "SceneLogic/SceneLogic_Normal.h"
#include "../Message/Msg_RetCode.pb.h"
CScene::CScene()
{
	
}

CScene::~CScene()
{

}

BOOL CScene::Init(UINT32 dwCopyType, UINT32 dwCopyID, UINT32 dwLogicType)
{
	m_dwCopyID = dwCopyID;

	m_dwCopyType = dwCopyType;

	CreateSceneLogic(dwLogicType);

	m_GridManager.Init(0, 0, 100, 100);

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
		PROCESS_MESSAGE_ITEM(MSG_TRANS_ROLE_DATA_REQ,   OnMsgTransRoleDataReq);
		PROCESS_MESSAGE_ITEM(MSG_ENTER_SCENE_REQ,		OnMsgEnterSceneReq);
		PROCESS_MESSAGE_ITEM(MSG_LEAVE_SCENE_REQ,		OnMsgLeaveSceneReq);
		PROCESS_MESSAGE_ITEM(MSG_ROLE_MOVE_REQ,			OnMsgRoleMoveReq);
		
		default:
		{
			
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


BOOL CScene::OnMsgRoleMoveReq(NetPacket *pNetPacket)
{
	RoleMoveReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	
	CSceneObject *pSceneObj = m_SceneObjectMgr.GetSceneObject(Req.roleid());
	if(pSceneObj == NULL)
	{
		ASSERT_FAIELD;
		return TRUE;
	}

	INT32 nDestIndex = m_GridManager.GetIndexByPos(Req.x(), Req.z());
	if(nDestIndex == -1)
	{
		//不合法的目标地址
		ASSERT_FAIELD;
		return TRUE;
	}

	INT32 nCurIndex  = m_GridManager.GetIndexByPos(pSceneObj->m_ObjectPos.x, pSceneObj->m_ObjectPos.z);

	ASSERT(m_GridManager.IsObjectExist(pSceneObj, nCurIndex));

	if(nDestIndex != nCurIndex)  //如果还在一个格子里，只需要修改一下坐标就行了。
	{
		ASSERT(!m_GridManager.IsObjectExist(pSceneObj, nDestIndex));

		m_GridManager.RemoveObjectFromGrid(pSceneObj, nCurIndex);

		m_GridManager.AddObjectToGrid(pSceneObj, nDestIndex);
	}
	
	pSceneObj->m_ObjectPos.x = Req.x();
	pSceneObj->m_ObjectPos.y = Req.y();
	pSceneObj->m_ObjectPos.z = Req.z();
	pSceneObj->m_ObjectPos.d = Req.d();

	pSceneObj->SetUpdate(UT_Update);

	CLog::GetInstancePtr()->AddLog("响应客户端坐标请求[%lld], 坐标 x =%f, z=%f", pSceneObj->GetObjectID(), pSceneObj->m_ObjectPos.x, pSceneObj->m_ObjectPos.z);

	return TRUE;
}

BOOL CScene::OnMsgRoleAttack(NetPacket *pNetPacket)
{
	return TRUE;
}


BOOL CScene::SendNewObjectToGrids(CWorldObject *pWorldObject, INT32 Grids[9])
{
	//先把玩家的完整包组装好
	NearByAddNty Nty;


	//pWorldObject->WriteToBuffer(&WriteHelper, UPDATE_FLAG_CREATE, UPDATE_TO_OTHERS);

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
				if(pIterObj->GetObjectType() == OT_Player)
				{
					CSceneObject *pSceneObj = (CSceneObject*)pIterObj;
					pSceneObj->SendProtoBuf(MSG_NEARBY_ADD_NTY, Nty);

				}
			}

			pIterObj = pIterObj->m_pGridNext;
		}

		i++;
	}

	return TRUE;
}

BOOL CScene::SendNewGridsToObject( INT32 Grids[9], CSceneObject *pSceneObj)
{
	//先把玩家的完整包组装好
	NearByAddNty Nty;
	//CBufferHelper WriteHelper(TRUE, 1024);
	//WriteHelper.BeginWrite(CMD_CHAR_NEARBY_ADD, 0, pPlayerObj->GetObjectID());

	//UINT32 *pCount = (UINT32*)WriteHelper.GetCurrentPoint();
	//*pCount = 0;
	//WriteHelper.Write(*pCount);

	INT32 nCount = 0;
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
			if(pIterObj->GetObjectID() != pSceneObj->GetObjectID())
			{
				//pIterObj->WriteToBuffer(&WriteHelper, UPDATE_FLAG_CREATE, UPDATE_TO_OTHERS);

				nCount++;
			}

			pIterObj = pIterObj->m_pGridNext;
		}

		i++;
	}

	if(nCount <= 0)
	{
		return TRUE;
	}

	pSceneObj->SendProtoBuf(MSG_NEARBY_ADD_NTY, Nty);

	return TRUE;
}

BOOL CScene::SendUpdateObjectToGrids(CWorldObject *pWorldObj, INT32 Grids[9] )
{
	//先把玩家的变化包组装好
	NearByUpdateNty Nty;
	
	//pWorldObj->WriteToBuffer(&WriteHelper, UPDATE_FLAG_CHANGE, UPDATE_TO_OTHERS);
	
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
				if(pIterObj->GetObjectType() == OT_Player)
				{
					CSceneObject *pSceneObject = (CSceneObject*)pIterObj;
					pSceneObject->SendProtoBuf(MSG_NEARBY_UPDATE_NTY, Nty);
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
	NearByDelNty Nty;
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
			CSceneObject *pScenePlayer = (CSceneObject*)pIterObj;

			if(pScenePlayer->GetObjectID() == u64RoleID)
			{
				ASSERT_FAIELD;
			}

			CLog::GetInstancePtr()->AddLog("自己[%lld]--Remove--From---[%lld], ",  u64RoleID, pScenePlayer->GetObjectID());

			pScenePlayer->SendProtoBuf(MSG_NEARBY_DEL_NTY, Nty);

			pIterObj = pIterObj->m_pGridNext;
		}

		i++;
	}


	return TRUE;
}

BOOL CScene::SendRemoveGridsToPlayer( INT32 Grids[9], CSceneObject *pSceneObj)
{
	NearByDelNty Nty;

	INT32 nCount = 0;
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
			Nty.add_rolelist(pIterObj->GetObjectID());

			if(pIterObj->GetObjectID() == pSceneObj->GetObjectID())
			{
				ASSERT_FAIELD;
			}
			nCount++;
			CLog::GetInstancePtr()->AddLog("自己[%lld]--Remove--From---[%lld], ",  pSceneObj->GetObjectID(), pIterObj->GetObjectID());

			pIterObj = pIterObj->m_pGridNext;
		}

		i++;
	}

	if(nCount <= 0)
	{
		return TRUE;
	}

	pSceneObj->SendProtoBuf(MSG_NEARBY_DEL_NTY, Nty);

	return TRUE;
}

BOOL CScene::OnMsgLeaveSceneReq(NetPacket *pNetPacket)
{
	LeaveSceneReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();

	if(Req.reason() == LGR_Disconnect)
	{

	}
	else if(Req.reason() == LGR_Quit)
	{

	}

	CSceneObject *pSceneObject = m_SceneObjectMgr.GetSceneObject(Req.roleid());
	if(pSceneObject == NULL)
	{
		ASSERT_FAIELD;
		return TRUE;
	}

	RemoveFromMap(pSceneObject);

	m_SceneObjectMgr.RemovePlayer(pSceneObject->GetObjectID());

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

BOOL CScene::CreateSceneLogic(UINT32 dwLogicType)
{
	switch (dwLogicType)
	{
	case SLT_NORMAL:
		m_pSceneLogic = new SceneLogic_Normal();
		break;

	default:
		{
			return false;
		}
	}

	return (m_pSceneLogic != NULL);
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

		if(pWorldObject->GetObjectType() == OT_Player)
		{
			SendNewGridsToObject(Grids, (CSceneObject*)pWorldObject);
		}

		pWorldObject->m_UpdateObjPos = pWorldObject->m_ObjectPos;

		pWorldObject->m_UpdateStatus   = UT_None;
	}
	else if(pWorldObject->m_UpdateStatus == UT_Delete)
	{
		INT32 Grids[10];

		m_GridManager.GetSurroundingGrids(nCurIndex, Grids);

		SendRemoveObjectToGrids(pWorldObject->GetObjectID(), Grids);

		if(pWorldObject->GetObjectType() == OT_Player)
		{
			delete (CSceneObject*)pWorldObject;
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

		SendNewGridsToObject(AddGrid, (CSceneObject*)pWorldObject);

		SendRemoveObjectToGrids(pWorldObject->GetObjectID(), RemoveGrid);

		SendRemoveGridsToPlayer(RemoveGrid, (CSceneObject*)pWorldObject);

		pWorldObject->m_UpdateObjPos = pWorldObject->m_ObjectPos;

		pWorldObject->m_UpdateStatus   = UT_None;
	}
	
	return TRUE;
}

BOOL CScene::SendUpdateObjectToMyself( CWorldObject *pWorldObj )
{
	CSceneObject *pPlayerObject = (CSceneObject *)pWorldObj;

	//先把玩家的变化包组装好
	//CBufferHelper WriteHelper(TRUE, 1024);
	//WriteHelper.BeginWrite(CMD_CHAR_UPDATE_MYSELF, GetSceneID(), pPlayerObject->GetObjectID());
	//WriteHelper.WriteCheckBufferCode();
	//pWorldObj->WriteToBuffer(&WriteHelper, UPDATE_FLAG_CHANGE, UPDATE_TO_MYSELF);
	//WriteHelper.WriteCheckBufferCode();
	//WriteHelper.EndWrite();

	//ServiceBase::GetInstancePtr()->SendCmdToConnection(pPlayerObject->GetConnectID(), &m_WriteBuffer);

	return TRUE;
}

BOOL CScene::OnMsgTransRoleDataReq(NetPacket *pNetPacket)
{
	TransRoleDataReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ASSERT(pHeader->u64TargetID != 0);

	//根据数据创建宠物，英雄
	CSceneObject *pSceneObject = new CSceneObject;
	m_SceneObjectMgr.AddPlayer(pSceneObject);
	if(AddToMap(pSceneObject))
	{
		TransRoleDataAck Ack;
		Ack.set_retcode(MRC_SUCCESSED);
		ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetLogicConnID(), MSG_TRANS_ROLE_DATA_ACK, pHeader->u64TargetID, 0, Ack);
	}
	else
	{
		ASSERT_FAIELD;
		return TRUE;
	}

	return TRUE;
}

BOOL CScene::OnMsgEnterSceneReq(NetPacket *pNetPacket)
{
	EnterSceneReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	EnterSceneAck Ack;
	Ack.set_copyid(m_dwCopyID);
	Ack.set_copytype(m_dwCopyType);
	Ack.set_retcode(MRC_SUCCESSED);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pNetPacket->m_dwConnID, MSG_ENTER_SCENE_ACK, Req.roleid(), 0, Ack);
	return TRUE;
}

BOOL CScene::SetSceneMapCoords( INT32 nLeft, INT32 nRight, INT32 nTop, INT32 nBottom )
{
	return TRUE;
}




