#ifndef __CLIENT_CMD_HANDLER_H__
#define __CLIENT_CMD_HANDLER_H__

#include "IBufferHandler.h"
#include "CommandDef.h"
#include "PlayerObject.h"
#include "DlgSelect.h"
#include "ClientConnector\ClientConnector.h"

class CClientCmdHandler : public IMessageHandler
{
private:
	CClientCmdHandler(void);

	~CClientCmdHandler(void);

public:
	static CClientCmdHandler* GetInstancePtr();

	BOOL OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL OnUpdate(UINT32 dwTick);


	BOOL SendNewAccountReq(LPCTSTR szAccountName, LPCTSTR szPassword);
	BOOL SendNewCharReq(UINT32 dwAccountID,LPCTSTR szCharName, UINT32 dwFeature);
	BOOL SendDelCharReq(UINT32 dwAccountID,UINT64 dwCharID);
	BOOL SendPickCharReq(UINT64 u64CharID);
	BOOL SendLeaveGameReq(UINT64 u64CharID);
	BOOL SendMoveReq(FLOAT x, FLOAT y, FLOAT z, UINT16 nDir);


	//*********************消息处理定义开始******************************
public:
	BOOL OnCmdLoginGameAck(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	BOOL OnCmdEnterGameAck(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL OnCmdNewAccountAck(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	BOOL OnCmdNewCharAck(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	BOOL OnCmdDelCharAck(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);


	//CMD_CHAR_NEARBY_ADD,			//添加周围的对象
	//CMD_CHAR_NEARBY_UPDATE,		//更新周围的对象
	//CMD_CHAR_NEARBY_REMOVE,		//删除周围的对象

	BOOL OnCmdNearByAdd(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	BOOL OnCmdNearByUpdate(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	BOOL OnCmdNearByRemove(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	BOOL OnCmdUpdateMyself(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);
	
	//*********************消息处理定义结束******************************

	CPlayerObject		m_HostPlayer;

	CPlayerObjectMgr	m_PlayerObjMgr;

	CDlgSelect			m_DlgSelect;

	CClientConnector	m_ClientConnector;

	BOOL				m_bLoginOK;

	UINT32				m_dwAccountID;
};


#endif