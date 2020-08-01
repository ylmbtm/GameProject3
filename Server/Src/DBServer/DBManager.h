#ifndef __DB_PROC_MANAGER__
#define __DB_PROC_MANAGER__
#include "../Message/Msg_LoginDBData.pb.h"
#include "../Message/Msg_Game.pb.h"
#include "DBInterface/CppMysql.h"
#include "../Message/Msg_Role.pb.h"

class CDBManager
{
public:
	CDBManager(void);

	~CDBManager(void);

	BOOL	Init();

	BOOL	Uninit();

	BOOL	GetRoleList(UINT64 u64AccountID, RoleListAck& Ack);
	BOOL	GetRoleData(UINT64 u64ID, DBRoleLoginAck& Ack);
	BOOL	GetBagData(UINT64 u64ID, DBRoleLoginAck& Ack);
	BOOL	GetCopyData(UINT64 u64ID, DBRoleLoginAck& Ack);
	BOOL	GetEquipData(UINT64 u64ID, DBRoleLoginAck& Ack);
	BOOL	GetGemData(UINT64 u64ID, DBRoleLoginAck& Ack);
	BOOL	GetPetData(UINT64 u64ID, DBRoleLoginAck& Ack);
	BOOL	GetPartnerData(UINT64 u64ID, DBRoleLoginAck& Ack);
	BOOL	GetTaskData(UINT64 u64ID, DBRoleLoginAck& Ack);
	BOOL	GetMountData(UINT64 u64ID, DBRoleLoginAck& Ack);
	BOOL	GetActivtyData(UINT64 u64ID, DBRoleLoginAck& Ack);
	BOOL	GetMailData(UINT64 u64ID, DBRoleLoginAck& Ack);
	BOOL	GetCounterData(UINT64 u64ID, DBRoleLoginAck& Ack);
	BOOL	GetFriendData(UINT64 u64ID, DBRoleLoginAck& Ack);
	BOOL	GetSkillData(UINT64 u64ID, DBRoleLoginAck& Ack);

	CppMySQL3DB     m_DBConnection;
};
#endif