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

	BOOL DispatchPacket(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);

	BOOL OnUpdate(UINT32 dwTick);


	BOOL SendNewAccountReq(LPCTSTR szAccountName, LPCTSTR szPassword);
	BOOL SendNewCharReq(UINT32 dwAccountID,LPCTSTR szCharName, UINT32 dwFeature);
	BOOL SendDelCharReq(UINT32 dwAccountID,UINT64 dwCharID);
	BOOL SendLeaveGameReq(UINT64 u64CharID);
	BOOL SendRoleLoginReq(UINT64 u64CharID);
	BOOL SendMoveReq(FLOAT x, FLOAT y, FLOAT z, UINT16 nDir);


	//*********************消息处理定义开始******************************
public:
	BOOL OnMsgAccountLoginAck(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);
	BOOL OnMsgSelectServerAck(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);
	BOOL OnMsgRoleListAck(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);
	BOOL OnCmdEnterGameAck(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);
	BOOL OnCmdNewAccountAck(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);



	//CMD_CHAR_NEARBY_ADD,			//添加周围的对象
	//CMD_CHAR_NEARBY_UPDATE,		//更新周围的对象
	//CMD_CHAR_NEARBY_REMOVE,		//删除周围的对象

	BOOL OnCmdNearByAdd(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);
	BOOL OnCmdNearByUpdate(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);
	BOOL OnCmdNearByRemove(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);
	BOOL OnCmdUpdateMyself(UINT32 dwMsgID, CHAR *PacketBuf, INT32 BufLen);
	
	//*********************消息处理定义结束******************************

	CPlayerObject		m_HostPlayer;

	CPlayerObjectMgr	m_PlayerObjMgr;

	CDlgSelect			m_DlgSelect;

	CClientConnector	m_ClientConnector;

	BOOL				m_bLoginOK;

	UINT32				m_dwAccountID;
};


#endif