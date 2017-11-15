#ifndef __DB_ACCOUNT_OBJECT_H__
#define __DB_ACCOUNT_OBJECT_H__
#include "AVLTree.h"
#include "CommonThreadFunc.h"
#include "DBInterface/CppMysql.h"

Th_RetName _SaveAccountThread( void* pParam );

struct CAccountObject
{
	UINT64		m_ID;
	std::string m_strName;
	std::string m_strPassword;
	UINT32      m_dwLastSvrID;
	UINT32      m_dwChannel;	//渠道ID
	UINT32		m_SealStatue;	//封禁状态
	UINT64		m_uSealTime;    //封禁结束时间
	UINT64      m_uCreateTime;  //创建时间
};


class CAccountObjectMgr : public AVLTree<UINT64, CAccountObject>
{
public:
	CAccountObjectMgr();
	~CAccountObjectMgr();

public:
	BOOL				LoadCacheAccount();
public:
	CAccountObject*		GetAccountObjectByID(UINT64 m_u64AccountID);

	CAccountObject*		CreateAccountObject(std::string strName, std::string strPwd, UINT32 dwChannel);

	BOOL				ReleaseAccountObject(UINT64 m_u64AccountID);

	BOOL				AddAccountObject(UINT64 u64ID, std::string strName, std::string strPwd, UINT32 dwChannel);

	CAccountObject*		GetAccountObjectByName(std::string name);

	CAccountObject*		GetAccountObject(std::string name, UINT32 dwChannel);

	BOOL				SaveAccountChange();

	BOOL				Close();

	BOOL				IsRun();

public:

	std::multimap<std::string, CAccountObject*>	m_mapNameObj;

	ArrayLockFreeQueue<CAccountObject*>		m_ArrChangedAccount;

	CppMySQL3DB 		m_DBConnection;

	BOOL				m_IsRun;

	HANDLE				m_hThread;

	UINT64				m_u64MaxID;
};

#endif //__DB_ACCOUNT_OBJECT_H__
