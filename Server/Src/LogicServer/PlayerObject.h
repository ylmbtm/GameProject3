#ifndef __WS_PLAYER_OBJECT_H__
#define __WS_PLAYER_OBJECT_H__
#include "GameDefine.h"
#include "Utility/AVLTree.h"
#include "Utility/Position.h"

class CPlayerObject
{
public:
	CPlayerObject();

	~CPlayerObject();
	
	//  初始化玩家对象
	BOOL		Init();
	//  反初始化玩家对家
	BOOL		Uninit();

public:
	UINT64		GetObjectID();

public:
	UINT64		m_u64ObjectID;
	CHAR		m_szObjectName[MAX_NAME_LEN];
	UINT32		m_dwSceneID;
	CPosition	m_ObjectPos;
	UINT32		m_dwFeature;
	UINT32      m_dwLevel;


};


#endif //__WS_PLAYER_OBJECT_H__
