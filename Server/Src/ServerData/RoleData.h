#ifndef __ROLE_DATA_OBJECT_H__
#define __ROLE_DATA_OBJECT_H__
#include "ServerStruct.h"
#include "SharedMemory.h"
#include "ServerDefine.h"
struct RoleDataObject : public ShareObject
{
	RoleDataObject()
	{
		m_uRoleID			= 0;			//角色ID
		m_uAccountID	= 0;			//账号ID
		m_Level		= 0;				//等级
		m_Exp			= 0;			//经验
		m_nLangID		= 0;			//语言ID
		m_u64Fight		= 0;			//战力
		m_VipLvl		= 0;			//VIP等级
		m_VipExp		= 0;			//VIP经验
		m_CityCopyID	= 0;			//主城副本类型
		m_bDelete       = FALSE;        //是否删除
		m_CarrerID      = 0;
	}

	std::string GetDataName()
	{

	}


	UINT64		m_uRoleID;			//角色ID
	UINT64		m_uAccountID;	//账号ID
	CHAR		m_szName[ROLE_NAME_LEN];	//角色名
	INT32		m_CarrerID;      //职业ID
	INT32		m_Level;			//等级
	INT64		m_Money[MONEY_NUM];		//所有的货币
	INT64		m_Action[ACTION_NUM];		//所有的体力
	INT64		m_Actime[ACTION_NUM];		//体力恢复时间
	INT64		m_Exp;			//经验
	INT32		m_nLangID;		//语言ID
	INT64		m_u64Fight;		//战力
	INT32		m_VipLvl;			//VIP等级
	INT32		m_VipExp;			//VIP经验
	INT32		m_CityCopyID;		//主城副本类型
	BOOL		m_bDelete;        //是否删除
	INT64       m_uQQ;            //QQ号

	//签到数据
	INT32		m_nSignNum;        //签到天数
	UINT32		m_nSignDay;        //最新的签到时间
	UINT32		m_RecvAction;	   //领取体力标记


	BOOL Save(IDataBase* pDB)
	{
		char szSql[SQL_BUFF_LEN];
		sprintf_s(szSql, 1024, "REPLACE INTO player (id, account_id, name, carrerid,level, citycopyid,exp, langid, action1, action2, action3,action4, actime1, actime2, actime3,actime4) VALUES(%lld, %lld, '%s', %d, %d, %d,%lld,%d,%lld,%lld,%lld,%lld,%lld,%lld,%lld,%lld);", \
		          m_uRoleID, m_uAccountID, m_szName, m_CarrerID, m_Level, m_CityCopyID, m_Exp, m_nLangID,
		          m_Action[0], m_Action[1], m_Action[2], m_Action[3],
		          m_Actime[0], m_Actime[1], m_Actime[2], m_Actime[3]
		         );

		pDB->Execut(szSql);

		printf("DB--------------------RoleDataObject::Save");

		return TRUE;
	}

	BOOL Delete(IDataBase* pDB)
	{
		char szSql[SQL_BUFF_LEN];
		sprintf_s(szSql, 1024, "update player set delete = %d");

		pDB->Execut(szSql);

		return TRUE;
	}
};


#endif //__SERVER_STRUCT_H__