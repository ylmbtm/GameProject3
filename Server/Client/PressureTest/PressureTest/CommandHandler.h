#ifndef __CLIENT_CMD_HANDLER_H__
#define __CLIENT_CMD_HANDLER_H__

#include "IBufferHandler.h"
#include "CommandDef.h"
#include "PlayerObject.h"
#include "ClientConnector\ClientConnector.h"

#define ST_NONE			0
#define ST_Logining		1
#define ST_Logined		2
#define ST_Entering		3
#define ST_EnterGame	4
#define ST_Disconnected 5


class CClientCmdHandler : public IMessageHandler
{
public:
	CClientCmdHandler(void);

	~CClientCmdHandler(void);


	BOOL OnCommandHandle(UINT16 wCommandID, UINT64 u64ConnID, CBufferHelper *pBufferHelper);

	BOOL OnUpdate(UINT32 dwTick);

	BOOL SendNewAccountReq(LPCTSTR szAccountName, LPCTSTR szPassword);
	BOOL SendNewCharReq(UINT32 dwAccountID,LPCTSTR szCharName, UINT32 dwFeature);
	BOOL SendDelCharReq(UINT32 dwAccountID,UINT64 dwCharID);
	BOOL SendPickCharReq(UINT64 u64CharID);
	BOOL SendLeaveGameReq(UINT64 u64CharID);
	BOOL SendMoveReq( FLOAT x, FLOAT y, FLOAT z, UINT16 nDir );

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

	std::vector<UINT64>	m_RoleIDList;

	CPlayerObject		m_HostPlayer;

	CPlayerObjectMgr	m_PlayerObjMgr;

	CClientConnector	m_ClientConnector;

	UINT32				m_dwHostState;

	std::string         m_strAccountName;
	std::string			m_strPassword;
	std::string			m_strRoleName;

	FLOAT               m_x;
	FLOAT				m_y;

public:
	VOID  MoveHost();
};


#endif