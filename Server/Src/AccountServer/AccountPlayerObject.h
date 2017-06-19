#ifndef __DB_PLAYER_OBJECT_H__
#define __DB_PLAYER_OBJECT_H__
#include "GameDefine.h"
#include "Utility/AVLTree.h"

class CDBPlayerObject
{
public:
	CDBPlayerObject();

	~CDBPlayerObject();
	
	BOOL	Init();

	BOOL	Uninit();
	
	UINT32	WriteToDBPacket( CBufferHelper *pWriteBuffer );

public:
	UINT32  WritePlayerBaseInfo( CBufferHelper *pWriteBuffer );

	UINT64		m_u64ObjectID;
	CHAR		m_szObjectName[MAX_NAME_LEN];
	UINT32		m_dwFeature;
	UINT32      m_dwLevel;
	UINT32      m_dwSceneID;
	FLOAT		m_fX;
	FLOAT		m_fY;
	FLOAT		m_fZ;


};


class CDBPlayerObjectMgr : public AVLTree<UINT64, CDBPlayerObject>
{
public:
	CDBPlayerObjectMgr()
	{
	}

	~CDBPlayerObjectMgr()
	{
	}



public:
	CDBPlayerObject*   GetPlayer(UINT64 u64CharID);

	CDBPlayerObject*   CreatePlayerByID(UINT64 u64CharID);

	BOOL			   ReleasePlayer(UINT32 u64CharID);

public:
};

#endif //__DB_PLAYER_OBJECT_H__
