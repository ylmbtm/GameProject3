#ifndef __ROLE_DATA_OBJECT_H__
#define __ROLE_DATA_OBJECT_H__
#include "DBInterface/DBStoredProc.h"
#include "DBInterface/DBInterface.h"
struct RoleDataObject : public ShareObject
{
	RoleDataObject()
	{
		m_uRoleID		= 0;            //角色ID
		m_uAccountID	= 0;            //账号ID
		m_Level			= 0;            //等级
		m_Exp			= 0;            //经验
		m_nLangID		= 0;            //语言ID
		m_u64Fight		= 0;            //战力
		m_nVipLvl		= 0;            //VIP等级
		m_nVipExp		= 0;            //VIP经验
		m_CityCopyID	= 0;            //主城副本类型
		m_bDelete       = FALSE;        //是否删除
		m_CarrerID      = 0;            //职业
		m_uCreateTime	= 0;            //角色创建时间
		m_uLogonTime	= 0;            //最近一次登录时间
		m_uLogoffTime	= 0;            //最近一次离线时间
		m_nChannel      = 0;            //角色渠道
		memset(m_szName, 0, ROLE_NAME_LEN);
	}

	UINT64		m_uRoleID;              //角色ID
	UINT64		m_uAccountID;           //账号ID
	CHAR		m_szName[ROLE_NAME_LEN];//角色名
	INT32		m_CarrerID;             //职业ID
	INT32		m_Level;                //等级
	INT64		m_Action[ACTION_NUM];   //所有的体力
	INT64		m_Actime[ACTION_NUM];   //体力恢复时间
	INT64		m_Exp;                  //经验
	INT32		m_nLangID;              //语言ID
	INT64		m_u64Fight;             //战力
	INT32		m_nVipLvl;               //VIP等级
	INT32		m_nVipExp;               //VIP经验
	INT32		m_CityCopyID;           //主城副本类型
	INT32		m_nChannel;             //角色渠道
	BOOL		m_bDelete;              //是否删除
	INT64       m_uQQ;                  //QQ号
	UINT64      m_uCreateTime;          //角色创建时间
	UINT64      m_uLogonTime;           //本次登录时间
	UINT64      m_uLogoffTime;          //离线时间
	UINT64      m_uGroupMailTime;       //群邮件接收时间
	UINT64      m_uGuildID;             //公会ID
	UINT32      m_nOnlineTime;          //在线时长
	//签到数据
	INT32		m_nSignNum;             //签到天数
	UINT32		m_nSignDay;             //最新的签到时间
	UINT32		m_RecvAction;           //领取体力标记


	BOOL Create(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO player (id, accountid, name, carrerid,level, citycopyid,exp, langid, viplevel, vipexp, action1, action2, action3,action4, actime1, actime2, actime3,actime4, createtime, logontime, logofftime, grouptime, fightvalue, guildid) \
			VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);");
		csp.set_uint64(0, m_uRoleID);
		csp.set_uint64(1, m_uAccountID);
		csp.set_string(2, m_szName, strlen(m_szName));
		csp.set_int32(3,  m_CarrerID);
		csp.set_int32(4,  m_Level);
		csp.set_int64(5,  m_CityCopyID);
		csp.set_int64(6,  m_Exp);
		csp.set_int32(7,  m_nLangID);
		csp.set_int32(8,  m_nVipLvl);
		csp.set_int32(9,  m_nVipExp);
		csp.set_int64(10,  m_Action[0]);
		csp.set_int64(11,  m_Action[1]);
		csp.set_int64(12, m_Action[2]);
		csp.set_int64(13, m_Action[3]);
		csp.set_int64(14, m_Actime[0]);
		csp.set_int64(15, m_Actime[1]);
		csp.set_int64(16, m_Actime[2]);
		csp.set_int64(17, m_Actime[3]);
		csp.set_int64(18, m_uCreateTime);
		csp.set_int64(19, m_uLogonTime);
		csp.set_int64(20, m_uLogoffTime);
		csp.set_int64(21, m_uGroupMailTime);
		csp.set_int64(22, m_u64Fight);
		csp.set_int64(23, m_uGuildID);
		return pDB->Execute(&csp);
	}

	BOOL Update(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO player (id, accountid, name, carrerid,level, citycopyid,exp, langid, viplevel, vipexp, action1, action2, action3,action4, actime1, actime2, actime3,actime4, createtime, logontime, logofftime, grouptime, fightvalue, guildid) \
		VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);");
		csp.set_uint64(0, m_uRoleID);
		csp.set_uint64(1, m_uAccountID);
		csp.set_string(2, m_szName, strlen(m_szName));
		csp.set_int32(3, m_CarrerID);
		csp.set_int32(4, m_Level);
		csp.set_int64(5, m_CityCopyID);
		csp.set_int64(6, m_Exp);
		csp.set_int32(7, m_nLangID);
		csp.set_int32(8, m_nVipLvl);
		csp.set_int32(9, m_nVipExp);
		csp.set_int64(10, m_Action[0]);
		csp.set_int64(11, m_Action[1]);
		csp.set_int64(12, m_Action[2]);
		csp.set_int64(13, m_Action[3]);
		csp.set_int64(14, m_Actime[0]);
		csp.set_int64(15, m_Actime[1]);
		csp.set_int64(16, m_Actime[2]);
		csp.set_int64(17, m_Actime[3]);
		csp.set_int64(18, m_uCreateTime);
		csp.set_int64(19, m_uLogonTime);
		csp.set_int64(20, m_uLogoffTime);
		csp.set_int64(21, m_uGroupMailTime);
		csp.set_int64(22, m_u64Fight);
		csp.set_int64(23, m_uGuildID);
		return pDB->Execute(&csp);
	}

	BOOL Delete(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("update player set isdelete = 1 where id = ?");
		csp.set_uint64(0, m_uRoleID);
		return pDB->Execute(&csp);
	}
};


#endif //__SERVER_STRUCT_H__