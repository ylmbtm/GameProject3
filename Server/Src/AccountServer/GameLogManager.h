#ifndef __GAME_LOG_MANAGER_H__
#define __GAME_LOG_MANAGER_H__

#include "../Message/Msg_Account.pb.h"
#include "../Message/Msg_ID.pb.h"
#include "GameService.h"
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
	BOOL LogAccountCreate(UINT64 uAccountID, std::string strAccountName,  const AccountLog& Log);
	BOOL LogAccountLogin(UINT64 uAccountID, std::string strAccountName, const AccountLog& Log);
};

#endif //__GAME_LOG_MANAGER_H__
