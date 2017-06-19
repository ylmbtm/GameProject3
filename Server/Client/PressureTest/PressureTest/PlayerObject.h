#ifndef _PLAYER_OBJECT_H_
#define _PLAYER_OBJECT_H_
#include "CharObject.h"


class CPlayerObject : public CCharObject
{
public:
	CPlayerObject();

	~CPlayerObject();

	virtual UINT32  ReadFromBuffer(CBufferHelper *pBufHelper);
};



class CPlayerObjectMgr : public std::map<UINT64, CPlayerObject*>
{
public:
	CPlayerObjectMgr()
	{

	}

	~CPlayerObjectMgr()
	{

	}


public:
	BOOL			AddPlayer(CPlayerObject *pObject);

	BOOL			RemovePlayer(UINT64 ObjectID);

	CPlayerObject*	GetPlayer(UINT64 ObjectID);
};

#endif //_PLAYER_OBJECT_H_
