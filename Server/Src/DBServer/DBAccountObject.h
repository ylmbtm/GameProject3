#ifndef __DB_ACCOUNT_OBJECT_H__
#define __DB_ACCOUNT_OBJECT_H__
#include "Utility/AVLTree.h"
#include "GameDefine.h"
#include "Utility/Position.h"

class CAccountObject
{
	UINT32 m_dwAccountID;
	UINT64 m_u64LoginCharID;
	UINT64 m_u64ConnID;		
};


class CAccountObjectMgr : public AVLTree<UINT32, CAccountObject>
{
public:
	CAccountObjectMgr();
	~CAccountObjectMgr();

public:
	CAccountObject*   GetAccountObject(UINT32 dwAccountID);

	CAccountObject*   CreateAccountObject(UINT32 dwAccountID);

	BOOL			  ReleaseAccountObject(UINT32 dwAccountID);
public:
};

#endif //__DB_ACCOUNT_OBJECT_H__
