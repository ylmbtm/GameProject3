#ifndef __DB_PROC_MANAGER__
#define __DB_PROC_MANAGER__

#include "Sqlite/CppSQLite3.h"
#include "PacketDef/ClientPacket.h"

class CDBProcManager
{
public:
	CDBProcManager(void);
	~CDBProcManager(void);

	BOOL Init();

	BOOL Uninit();

	UINT32 GetMaxAccountID();

	//create new a account
	BOOL CreateAccount(char *szAccount, char *szPassword);

	//VerifyAccount
	UINT32 VerifyAccount(char *szAccount, char *szPassword);

	UINT64 GetCharID(char *szCharName);

	UINT64 GenarateCharID(UINT32 dwAccountID);

	UINT32 GetAccountID(char *szAccount);

	//load account char info
	BOOL   LoadAccountCharInfo(UINT32 dwAccountID, StCharLoginAck &Ack);

	BOOL   CreateNewChar(StCharNewCharReq &Req, StCharNewCharAck &Ack);

	BOOL   DelChar(StCharDelCharReq &Req);

	CppSQLite3DB	m_DBConnection;

	UINT32			m_dwMaxAccountID;
};
#endif