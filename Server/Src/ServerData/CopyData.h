#ifndef __COPY_DATA_OBJECT_H__
#define __COPY_DATA_OBJECT_H__
#include "DBInterface/DBInterface.h"
struct CopyDataObject : public ShareObject
{
	CopyDataObject()
	{
		m_uRoleID = 0;
		m_dwBattleCnt = 0;
		m_dwResetCnt = 0;
		m_dwStarNum = 0;
		m_dwCopyID = 0;
		m_uBattleTime = 0;
		m_uResetTime = 0;
	}

	UINT64 m_uRoleID;
	UINT32 m_dwCopyID;
	UINT32 m_dwBattleCnt;
	UINT32 m_dwResetCnt;
	UINT32 m_dwStarNum;
	UINT64 m_uBattleTime;
	UINT64 m_uResetTime;

	BOOL Create(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO copy (copyid, roleid, star, battlecnt, resetcnt, battletime, resettime) \
			VALUES(?,?,?,?,?,?,?);");
		csp.set_uint32(0, m_dwCopyID);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint32(2, m_dwStarNum);
		csp.set_int32(3, m_dwBattleCnt);
		csp.set_int32(4, m_dwResetCnt);
		csp.set_uint64(5, m_uBattleTime);
		csp.set_uint64(6, m_uResetTime);
		return pDB->Execute(&csp);
	}

	BOOL Update(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO copy (copyid, roleid, star, battlecnt, resetcnt, battletime, resettime) \
			VALUES(?,?,?,?,?,?,?);");
		csp.set_uint32(0, m_dwCopyID);
		csp.set_uint64(1, m_uRoleID);
		csp.set_uint32(2, m_dwStarNum);
		csp.set_int32(3, m_dwBattleCnt);
		csp.set_int32(4, m_dwResetCnt);
		csp.set_uint64(5, m_uBattleTime);
		csp.set_uint64(6, m_uResetTime);
		return pDB->Execute(&csp);
	}

	BOOL Delete(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("delete from copy where copyid = ? and roleid = ?");
		csp.set_uint32(0, m_dwCopyID);
		csp.set_uint64(1, m_uRoleID);
		return pDB->Execute(&csp);
	}
};


struct ChapterDataObject : public ShareObject
{
	ChapterDataObject()
	{
		m_uRoleID = 0;
		m_dwCopyType = 0;
		m_dwChapter = 0;
		m_dwStarAward = 0;
		m_dwSceneAward = 0;
	}

	UINT64 m_uRoleID;
	UINT32 m_dwCopyType;
	UINT32 m_dwChapter;
	UINT32 m_dwStarAward;
	UINT32 m_dwSceneAward;

	BOOL Create(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO chapter (chapterid, copytype, roleid, staraward, sceneaward) \
			VALUES(?,?,?,?,?);");
		csp.set_uint32(0, m_dwChapter);
		csp.set_uint32(1, m_dwCopyType);
		csp.set_uint64(2, m_uRoleID);
		csp.set_uint32(3, m_dwStarAward);
		csp.set_uint32(4, m_dwSceneAward);
		return pDB->Execute(&csp);
	}

	BOOL Update(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("REPLACE INTO chapter (chapterid, copytype, roleid, staraward, sceneaward) \
			VALUES(?,?,?,?,?);");
		csp.set_uint32(0, m_dwChapter);
		csp.set_uint32(1, m_dwCopyType);
		csp.set_uint64(2, m_uRoleID);
		csp.set_uint32(3, m_dwStarAward);
		csp.set_uint32(4, m_dwSceneAward);
		return pDB->Execute(&csp);
	}

	BOOL Delete(IDBInterface* pDB)
	{
		static CDBStoredProcedure csp("delete from chapter where chapterid = ? and roleid = ?");
		csp.set_uint32(0, m_dwChapter);
		csp.set_uint64(1, m_uRoleID);
		return pDB->Execute(&csp);
	}
};


#endif //__COPY_DATA_OBJECT_H__