#ifndef __DB_PROC_MANAGER__
#define __DB_PROC_MANAGER__
#include "Sqlite/CppSQLite3.h"
#include "DBConnection.h"
#include "DBStoredProcMgr.h"
#include "../ServerData/serverStruct.h"
#include "../Message/Msg_LoginDBData.pb.h"
#include "../Message/Msg_Game.pb.h"

class CDBManager : public IDataBase
{
public:
	CDBManager(void);

	~CDBManager(void);

	BOOL Init();

	BOOL Uninit();

	BOOL Execut(std::string sql);

	BOOL  GetRoleList(UINT64 u64AccountID, RoleListAck& Ack);

	BOOL  GetRoleData(UINT64 u64ID, DBRoleLoginAck& Ack);
	BOOL  GetBagData(UINT64 u64ID, DBRoleLoginAck& Ack);
	BOOL  GetCopyData(UINT64 u64ID, DBRoleLoginAck& Ack);
	BOOL  GetEquipData(UINT64 u64ID, DBRoleLoginAck& Ack);
	BOOL  GetPetData(UINT64 u64ID, DBRoleLoginAck& Ack);
	BOOL  GetPartnerData(UINT64 u64ID, DBRoleLoginAck& Ack);
	BOOL  GetTaskData(UINT64 u64ID, DBRoleLoginAck& Ack);
	BOOL  GetMountData(UINT64 u64ID, DBRoleLoginAck& Ack);
	BOOL  GetActivtyData(UINT64 u64ID, DBRoleLoginAck& Ack);

	CppSQLite3DB	m_DBConnection;
	//真实的mysql
	//CDBConnection   m_DBConnection;

	//CDBStoredProcedureMgr m_DBProceduceMgr;
};
#endif