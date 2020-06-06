#ifndef __GAME_LOG_MANAGER_H__
#define __GAME_LOG_MANAGER_H__

#include "../Message/Msg_ID.pb.h"
#include "GameService.h"
class CPlayerObject;

class CGameLogManager
{
public:
	CGameLogManager();
	~CGameLogManager();

public:
	static CGameLogManager* GetInstancePtr();

	template<typename T>
	BOOL WriteGameLog(T& Data)
	{
		ERROR_RETURN_FALSE(CGameService::GetInstancePtr()->GetLogSvrConnID() > 0);

		ServiceBase::GetInstancePtr()->SendMsgRawData(CGameService::GetInstancePtr()->GetLogSvrConnID(), MSG_LOG_DATA_NTF, 0, 0, (const char*)&Data, sizeof(T));

		return TRUE;
	}


public:
	BOOL LogRoleCreate(CPlayerObject* pPlayer);

	BOOL LogRoleLogin(CPlayerObject* pPlayer);
};

#endif //__GAME_LOG_MANAGER_H__
